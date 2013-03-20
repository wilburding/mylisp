/*
 * =====================================================================================
 *
 *       Filename:  eval_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2013 21:30:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <gtest/gtest.h>
#include <mylisp.h>

#include <vector>
#include <algorithm>
#include <memory>


void clear_exception()
{
    delete get_exception();
    set_exception(nullptr);
}


bool startswith(const char* str, const char* prefix)
{
    while(*str && *prefix)
    {
        if(*str == *prefix)
        {
            ++str;
            ++prefix;
        }
        else
        {
            break;
        }
    }
    return *prefix == '\0';
}


TEST(startswithTest, shouldWork)
{
    EXPECT_TRUE(startswith("ab", "a"));
    EXPECT_TRUE(startswith("ab", ""));
    EXPECT_TRUE(startswith("ab", "ab"));
    EXPECT_FALSE(startswith("a", "ab"));
    EXPECT_FALSE(startswith("", "ab"));
}


struct ListDeleter
{
    void operator()(ListObject* l)
    {
        // only delete ListObject, not include car
        // naive
        ListObject* cur = l;
        while(cur)
        {
            ListObject* next = try_list_cdr(cur);
            delete cur;
            cur = next;
        }
    }
};


class EvalTest: public testing::Test
{
public:
    typedef std::unique_ptr<ListObject, ListDeleter> ListPtr;
    typedef std::unique_ptr<Object> ObjectPtr;

    EvalTest()
        :env_(nullptr)
    {}

    virtual void SetUp() override
    {
        eval_init();
    }

    virtual void TearDown() override
    {
        eval_fini();
        clear_exception();
    }

    Environment env_;
};


TEST_F(EvalTest, test_good_variable)
{
    std::unique_ptr<Symbol> sym(make_symbol("a"));
    ObjectPtr val(new BoolObject);
    env_.set(sym->id(), val.get());

    EXPECT_EQ(val.get(), eval(sym.get(), &env_));
}


TEST_F(EvalTest, test_not_found_variable_should_raise)
{
    ObjectPtr sym(make_symbol("a"));
    EXPECT_EQ(nullptr, get_exception());
    EXPECT_EQ(nullptr, eval(sym.get(), &env_));
    EXPECT_NE(nullptr, get_exception());
}


TEST_F(EvalTest, test_bad_if_should_raise)
{
    std::vector<ObjectPtr> values;
    std::generate_n(back_inserter(values), 5, [](){ return ObjectPtr(new BoolObject); });   
    ObjectPtr if_sym(make_symbol("if"));

    {
        ListPtr if_expr1(list(if_sym.get()));
        EXPECT_EQ(nullptr, eval(if_expr1.get(), &env_));
        EXPECT_NE(nullptr, get_exception());
        clear_exception();
    }

    {
        ListPtr if_expr2(list(if_sym.get(), values[0].get()));
        EXPECT_EQ(nullptr, eval(if_expr2.get(), &env_));
        EXPECT_NE(nullptr, get_exception());
        clear_exception();
    }

    {
        ListPtr if_expr3(list(if_sym.get(), values[0].get(), values[1].get(), 
                    values[2].get(), values[3].get()));
        EXPECT_EQ(nullptr, eval(if_expr3.get(), &env_));
        EXPECT_NE(nullptr, get_exception());
        clear_exception();
    }
}


TEST_F(EvalTest, test_if_true_cond)
{
    ObjectPtr if_sym(make_symbol("if"));
    std::unique_ptr<BoolObject> pred_expr(new BoolObject(true));
    ObjectPtr consequence_expr(new BoolObject());

    ListPtr if_no_alter(list(if_sym.get(), pred_expr.get(), consequence_expr.get()));
    EXPECT_EQ(consequence_expr.get(), eval(if_no_alter.get(), &env_));
}


TEST_F(EvalTest, test_if_false_cond)
{
    ObjectPtr if_sym(make_symbol("if"));
    std::unique_ptr<BoolObject> pred_expr(new BoolObject(false));
    ObjectPtr consequence_expr(new BoolObject());
    ObjectPtr alternative_expr(new BoolObject());

    {
        ListPtr if_no_alter(list(if_sym.get(), pred_expr.get(), consequence_expr.get()));
        ObjectPtr result(eval(if_no_alter.get(), &env_));
        EXPECT_TRUE(ListObject::is_null_list(result.get()));
    }

    {
        ListPtr if_with_alter(list(if_sym.get(), pred_expr.get(), consequence_expr.get(), alternative_expr.get()));
        EXPECT_EQ(alternative_expr.get(), eval(if_with_alter.get(), &env_));
    }
}


TEST_F(EvalTest, test_bad_quote)
{
    std::vector<ObjectPtr> values;
    std::generate_n(back_inserter(values), 2, [](){ return std::unique_ptr<BoolObject>(new BoolObject); });   
    ObjectPtr quote_sym(make_symbol("quote"));

    {
        ListPtr quote_expr1(
                list(quote_sym.get()));
        EXPECT_EQ(nullptr, eval(quote_expr1.get(), &env_));
        clear_exception();
    }

    {
        ListPtr quote_expr1(list(quote_sym.get(), values[0].get(), values[1].get()));
        EXPECT_EQ(nullptr, eval(quote_expr1.get(), &env_));
        clear_exception();
    }
}


TEST_F(EvalTest, test_quote_return_cdr)
{
    ObjectPtr quote_sym(make_symbol("quote"));
    ObjectPtr value(new BoolObject);

    ListPtr quote_expr(list(quote_sym.get(), value.get()));
    EXPECT_EQ(value.get(), eval(quote_expr.get(), &env_));
}


class AssignmentTest: public EvalTest
{
public:
    virtual void SetUp() override
    {
        EvalTest::SetUp();
        assignment_sym_.reset(make_symbol("set!"));
    }

    ObjectPtr assignment_sym_;
};


TEST_F(AssignmentTest, unbound_variable_should_error)
{
    std::unique_ptr<Symbol> variable_name(make_symbol("a"));
    ObjectPtr value(new BoolObject);

    ListPtr expr(list(assignment_sym_.get(), variable_name.get(), value.get()));
    ObjectPtr result(eval(expr.get(), &env_));
    EXPECT_EQ(nullptr, result.get());
    /*EXPECT_EQ(value.get(), env_.look_up_variable(variable_name->id()));*/
    EXPECT_TRUE(startswith(get_exception()->message().c_str(), "bad assignment expr! unbound variable! "));
}


TEST_F(AssignmentTest, smoking_test)
{
    std::unique_ptr<Symbol> variable_name(make_symbol("a"));
    ObjectPtr old_value(new BoolObject);
    ObjectPtr new_value(new BoolObject);
    env_.define_variable(variable_name->id(), old_value.get());

    ListPtr expr(list(assignment_sym_.get(), variable_name.get(), new_value.get()));
    ObjectPtr result(eval(expr.get(), &env_));
    EXPECT_EQ(new_value.get(), env_.look_up_variable(variable_name->id()));
    EXPECT_TRUE(ListObject::is_null_list(result.get()));
}


TEST_F(AssignmentTest, test_too_short_should_error)
{
    ObjectPtr dumb_value(new BoolObject());
    ListPtr expr(list(assignment_sym_.get(), dumb_value.get()));
    EXPECT_EQ(nullptr, eval(expr.get(), &env_));
    EXPECT_TRUE(startswith(get_exception()->message().c_str(), "bad assignment expr!"));
}


TEST_F(AssignmentTest, test_too_long_should_error)
{
    ObjectPtr dumb_value1(new BoolObject());
    ObjectPtr dumb_value2(new BoolObject());
    ObjectPtr dumb_value3(new BoolObject());
    ListPtr expr(list(assignment_sym_.get(), dumb_value1.get(), dumb_value2.get(), dumb_value3.get()));
    EXPECT_EQ(nullptr, eval(expr.get(), &env_));
    EXPECT_TRUE(startswith(get_exception()->message().c_str(), "bad assignment expr!"));
}


TEST_F(AssignmentTest, test_bad_variable_name)
{
    ObjectPtr dumb_value1(new BoolObject());
    ObjectPtr dumb_value2(new BoolObject());
    ListPtr expr(list(assignment_sym_.get(), dumb_value1.get(), dumb_value2.get()));
    EXPECT_EQ(nullptr, eval(expr.get(), &env_));
    EXPECT_TRUE(startswith(get_exception()->message().c_str(), "bad assignment expr! cadr must be a variable name! "));
}


class DefinitionTest: public EvalTest
{
public:
    virtual void SetUp() override
    {
        EvalTest::SetUp();
        definition_sym_.reset(make_symbol("define")); 
    } 

    ObjectPtr definition_sym_;
};


TEST_F(DefinitionTest, smoking_teste)
{
    std::unique_ptr<Symbol> variable_name(make_symbol("a"));
    ObjectPtr value(new BoolObject);

    ListPtr expr(list(definition_sym_.get(), variable_name.get(), value.get()));
    ObjectPtr result(eval(expr.get(), &env_));
    EXPECT_TRUE(ListObject::is_null_list(result.get()));
    EXPECT_EQ(value.get(), env_.get(variable_name->id()));
}
