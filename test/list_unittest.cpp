/*
 * =====================================================================================
 *
 *       Filename:  list_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/11/2013 22:07:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <gtest/gtest.h>
#include <list.h>
#include <memory>


Object* ListObject::eval(Environment* ){return nullptr;}


TEST(EmptyListTest, SmokingTest)
{
    std::unique_ptr<ListObject> alist(null_list());
    EXPECT_EQ(0, alist->length());
    EXPECT_TRUE(alist->empty());
    EXPECT_EQ(nullptr, alist->car());
    EXPECT_EQ(nullptr, alist->cdr());

    EXPECT_EQ("()", alist->to_string());
}


class ListTest: public testing::Test
{
public:
    ListTest()
        :list_(nullptr),
        obj1_(nullptr),
        obj2_(nullptr),
        obj3_(nullptr)
    {
    }

protected:
    virtual void SetUp()
    {
        obj1_ = new BoolObject();
        obj2_ = new BoolObject();
        obj3_ = new BoolObject();
        list_ = list(obj1_, obj2_, obj3_);
    }

    virtual void TearDown()
    {
        delete list_;
        delete obj1_;
        delete obj2_;
        delete obj3_;
    }

public:
    ListObject* list_;
    Object* obj1_;
    Object* obj2_;
    Object* obj3_;
};


TEST_F(ListTest, smoking_test)
{
    EXPECT_EQ(3, list_->length());
    EXPECT_TRUE(list_->car() == obj1_);
    EXPECT_TRUE(ListObject::is_list(list_->cdr()));
    EXPECT_EQ("(" + obj1_->to_string() + " " + obj2_->to_string() + " " + \
            obj3_->to_string() + " )", list_->to_string());

    ListObject* cdr = try_list_cdr(list_);
    EXPECT_EQ(2, cdr->length());
    EXPECT_TRUE(cdr->car() == obj2_);

    ListObject* cddr = try_list_cdr(cdr);
    EXPECT_EQ(1, cddr->length());
    EXPECT_TRUE(cddr->car() == obj3_);
    EXPECT_TRUE(ListObject::is_null_list(cddr->cdr()));
    EXPECT_EQ("(" + obj3_->to_string() + " )", cddr->to_string());
}
