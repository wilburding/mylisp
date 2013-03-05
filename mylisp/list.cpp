/*
 * =====================================================================================
 *
 *       Filename:  list.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/29/2013 23:44:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "list.h"
#include "exception.h"

#include <sstream>


ListObject::ListObject(Object* value)
    :Object(),
    value_(value),
    next_(nullptr)
{
}


std::string ListObject::to_string() const
{
    std::stringstream ss;
    ss << "(";
    const ListObject* p = this;
    while(p && p->value_)
    {
        ss << p->value_->to_string() << " ";
        p = p->next_;
    }
    ss << ")";
    return ss.str();
}


bool ListObject::set_next(Object* obj)
{
    if(empty())
    {
        set_exception(new Exception("empty list!"));
        return false;
    }
    else
    {
        next_ = new ListObject(obj);
        return true;
    }
}


size_t ListObject::length() const
{
    const ListObject* cur = this;
    size_t len = 0;
    while(cur->value())
    {
        ++len;
        cur = cur->next();
    }
    return len;
}


ListObject* pair(Object* car, Object* cdr)
{
    ListObject* list = new ListObject(car);
    list->set_next(cdr);
    return list;
}
