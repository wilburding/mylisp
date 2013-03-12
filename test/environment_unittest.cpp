/*
 * =====================================================================================
 *
 *       Filename:  environment_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/2013 22:23:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <gtest/gtest.h>
#include <environment.h>
#include <object.h>

#include <memory>


class EnvironmentTest: public testing::Test
{
public:
    EnvironmentTest()
        :env_(nullptr)
    {

    }

    virtual void SetUp()
    {
        id1_ = 1;
        id2_ = 2;
        id3_ = 3;

        obj1_ = new BoolObject();
        obj2_ = new BoolObject();

        env_.set(id1_, obj1_);
        env_.set(id2_, obj2_);
    }

    virtual void TearDown()
    {
        delete obj1_;
        delete obj2_;
    }

    Environment env_;
    SymbolID id1_;
    SymbolID id2_;
    SymbolID id3_;
    Object* obj1_;
    Object* obj2_;
};


TEST_F(EnvironmentTest, test_get_and_set)
{
    EXPECT_EQ(obj1_, env_.get(id1_));

    const Environment& cenv = env_;
    EXPECT_EQ(obj2_, cenv.get(id2_));

    EXPECT_EQ(nullptr, env_.get(id3_));
}


TEST_F(EnvironmentTest, test_variable_access)
{
    Environment outer(nullptr);
    SymbolID outer_id = 10;
    std::unique_ptr<Object> obj_outer(new BoolObject());
    outer.set(outer_id, obj_outer.get());
    env_.set_outer(&outer);

    EXPECT_EQ(obj1_, env_.look_up_variable(id1_));
    EXPECT_EQ(obj_outer.get(), static_cast<const Environment*>(&env_)->look_up_variable(outer_id));
    EXPECT_EQ(nullptr, env_.look_up_variable(id3_));

    std::unique_ptr<Object> obj_set(new BoolObject());

    EXPECT_TRUE(env_.set_variable(id2_, obj_set.get()));
    EXPECT_EQ(obj_set.get(), env_.look_up_variable(id2_));

    EXPECT_TRUE(env_.set_variable(outer_id, obj_set.get()));
    EXPECT_EQ(obj_set.get(), env_.look_up_variable(outer_id));

    EXPECT_FALSE(env_.set_variable(id3_, obj_set.get()));
}
