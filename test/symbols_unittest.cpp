/*
 * =====================================================================================
 *
 *       Filename:  symbols_unittest.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2013 21:58:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <gtest/gtest.h>
#include "symbols.h"
#include  <memory>


Object* Symbol::eval(Environment* ){ return nullptr; }


TEST(SymbolTest, test_name)
{
    Symbols symbols;
    std::unique_ptr<Symbol> sym(new Symbol("sym1", &symbols));

    EXPECT_STREQ("sym1", sym->name());
}


TEST(SymbolsTest, test_add)
{
    Symbols symbols;
    const char* name = "test";
    SymbolID id1 = symbols.add(name);

    const char* old = symbols.get(id1);
    EXPECT_NE(name, old);

    EXPECT_EQ(id1, symbols.add(name));
    EXPECT_EQ(id1, symbols.add(old));
}


TEST(SymbolsTest, test_find)
{
    Symbols symbols;
    const char* s1 = "test1";
    const char* s2 = "test2";
    SymbolID id1 = symbols.add(s1);
    SymbolID id2 = symbols.add(s2);

    EXPECT_EQ(id1, symbols.find(s1));
    EXPECT_EQ(id2, symbols.find(s2));
    EXPECT_EQ(INVALID_SYMBOL_ID, symbols.find("not found symbol"));
}
