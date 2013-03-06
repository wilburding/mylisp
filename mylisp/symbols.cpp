/*
 * =====================================================================================
 *
 *       Filename:  symbols.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/22/2013 20:40:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "symbols.h"
#include "environment.h"

#include <stdlib.h>


bool Symbol::is_symbol(Object* obj)
{
    return typeid(*obj) == typeid(Symbol);
}


Symbol::Symbol(const char* value)
    :Object(),
    id_(Symbols::instance().add_symbol(value))
{
}


std::string Symbol::to_string() const
{
    return {this->name()};
}


const char* Symbol::name() const
{
    return Symbols::instance().get(id_);    
}


Symbols& Symbols::instance()
{
    static Symbols symbols;
    return symbols;
}


Symbols::~Symbols()
{
    for(auto str: symbol_strs_)
    {
        free(str);
    }
}


SymbolID Symbols::add_symbol(const char* symbol)
{
    auto result = symbol_map_.find(symbol);
    if(result != symbol_map_.end())
    {
        return result->second;
    }
    else
    {
        char* str = strdup(symbol);

        symbol_strs_.push_back(str);
        symbol_map_.emplace(str, symbol_strs_.size() - 1);
        return symbol_strs_.size() - 1;
    }
}


SymbolID Symbols::find_symbol(const char* symbol)
{
    auto result = symbol_map_.find(symbol);
    if(result != symbol_map_.end())
    {
        return result->second;
    }
    else
    {
        return INVALID_SYMBOL_ID;
    }
}


const char* Symbols::get(SymbolID id) const
{
    return symbol_strs_[id];  // use its check
}
