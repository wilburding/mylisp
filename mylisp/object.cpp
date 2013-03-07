/*
 * =====================================================================================
 *
 *       Filename:  object.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/21/2013 21:05:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "object.h"


Object::~Object()
{
}

std::string Object::repr() const
{
    char buf[512] = {0};
    snprintf(buf, sizeof(buf), "<%s at %p>", this->repr().c_str(), this);
    return {buf};
}


bool BoolObject::is_bool(const Object* obj)
{
    return typeid(*obj) == typeid(BoolObject);
}


BoolObject::BoolObject(bool value)
    :Object(),
    value_(value)
{}


bool is_true(const Object* obj)
{
    if(BoolObject::is_bool(obj))
    {
        return static_cast<const BoolObject*>(obj)->value();
    }
    else
    {
        // TODO: need fix?
        return false;
    }
}


IntObject::IntObject(int value)
    :Object(),
    value_(value)
{}


StringObject::StringObject(const std::string& value)
    :Object(),
    value_(value)
{}

StringObject::StringObject(std::string&& value)
    :Object(),
    value_(std::move(value))
{}
