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


Object* Environment::look_up_variable(const char* name)
{
    return const_cast<Object*>(static_cast<const Environment*>(this)->look_up_variable(name));
}


const Object* Environment::look_up_variable(const char* name) const
{
    const Environment* cur = this;
    while(cur)
    {
        auto value = cur->get(name);
        if(value)
        {
            return value;
        }
        else
        {
            cur = this->outer_;
        }
    }
    return nullptr;
}


void Environment::define_variable(const char* name, Object* value)
{
    this->set(name, value);
}


bool Environment::set_variable(const char* name, Object* value)
{
    Environment* cur = this;
    while(cur)
    {
        auto result = cur->bindings_.find(name);
        if(result != cur->bindings_.end())
        {
            result->second = value;
            return true;
        }
        else
        {
            cur = this->outer_;
        }
    }
    return false;
}


Object* Environment::get(const char* name)
{
    return const_cast<Object*>(const_cast<Environment*>(this)->get(name));
}


const Object* Environment::get(const char* name) const
{
    auto result = bindings_.find(name);
    if(result == bindings_.end())
    {
        return nullptr;
    }
    else
    {
        return result->second;
    }
}


void Environment::set(const char* name, Object* value)
{
    bindings_.emplace(name, value);
}
