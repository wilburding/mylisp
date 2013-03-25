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
#include "exception.h"

#include <assert.h>
#include <stdlib.h>
#include <typeinfo>


bool Symbol::is_symbol(Object* obj)
{
    return typeid(*obj) == typeid(Symbol);
}


Symbol::Symbol(const char* name, Symbols* symbols)
    :Object(),
    id_(symbols->add(name)),
    owner_(symbols)
{}


Symbol::Symbol(SymbolID id, Symbols* symbols)
    :Object(),
    id_(id),
    owner_(symbols)
{}


std::string Symbol::to_string() const
{
    return {this->name()};
}


const char* Symbol::name() const
{
    return owner_->get(id_);    
}


Object* Symbol::eval(Environment* env)
{
    auto result = env->look_up_variable(this->id());
    if(!result)
    {
        set_exception(new Exception("could not resolve symbol " + this->to_string()));
    }
    return result;
}




Symbols::~Symbols()
{
    for(auto str: symbol_strs_)
    {
        free(str);
    }
}


SymbolID Symbols::add(const char* symbol)
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
        symbol_map_[str] = symbol_strs_.size() - 1;
        assert(symbol_map_.find(symbol) != symbol_map_.end());
        return symbol_strs_.size() - 1;
    }
}


SymbolID Symbols::find(const char* symbol) const
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
