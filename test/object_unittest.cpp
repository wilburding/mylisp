/*
 * =====================================================================================
 *
 *       Filename:  testobject_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/07/2013 22:55:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "gtest/gtest.h"
#include "object.h"


TEST(BoolObjectTest, is_bool)
{
    BoolObject b;
    EXPECT_TRUE(BoolObject::is_bool(&b));
}
