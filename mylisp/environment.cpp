/*
 * =====================================================================================
 *
 *       Filename:  environment.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/23/2013 22:13:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "environment.h"


Environment::Environment(Environment* outer)
    :outer_(outer)
{
}


Object* Environment::look_up_variable(SymbolID id)
{
    return const_cast<Object*>(static_cast<const Environment*>(this)->look_up_variable(id));
}


const Object* Environment::look_up_variable(SymbolID id) const
{
    const Environment* cur = this;
    while(cur)
    {
        auto value = cur->get(id);
        if(value)
        {
            return value;
        }
        else
        {
            cur = cur->outer_;
        }
    }
    return nullptr;
}


void Environment::define_variable(SymbolID id, Object* value)
{
    this->set(id, value);
}


bool Environment::set_variable(SymbolID id, Object* value)
{
    Environment* cur = this;
    while(cur)
    {
        auto result = cur->bindings_.find(id);
        if(result != cur->bindings_.end())
        {
            result->second = value;
            return true;
        }
        else
        {
            cur = cur->outer_;
        }
    }
    return false;
}


Object* Environment::get(SymbolID id)
{
    return const_cast<Object*>(static_cast<const Environment*>(this)->get(id));
}


const Object* Environment::get(SymbolID id) const
{
    auto result = bindings_.find(id);
    if(result == bindings_.end())
    {
        return nullptr;
    }
    else
    {
        return result->second;
    }
}


void Environment::set(SymbolID id, Object* value)
{
    bindings_.emplace(id, value);
}
