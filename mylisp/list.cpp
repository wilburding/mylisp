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


bool ListObject::is_list(const Object* obj)
{
    return typeid(*obj) == typeid(ListObject);
}


bool ListObject::is_null_list(const Object* obj)
{
    return is_list(obj) && static_cast<const ListObject*>(obj)->empty();
}


ListObject::ListObject(Object* car, Object* cdr)
    :Object(),
    car_(car),
    cdr_(cdr)
{
}


std::string ListObject::to_string() const
{
    std::stringstream ss;
    ss << "(";
    const ListObject* p = this;
    while(p)
    {
        if(p->car())
        {
            ss << p->car()->to_string() << " ";
            if(p->cdr())
            {
                if(is_list(p->cdr()))
                {
                    p = static_cast<const ListObject*>(p->cdr());
                }
                else
                {
                    ss << ". " << p->cdr()->to_string();
                }
            }
            else
            {
                p = nullptr;
            }
        }
        else
        {
            p = nullptr;
        }
    }
    ss << ")";
    return ss.str();
}


int ListObject::length() const
{
    const ListObject* cur = this;
    size_t len = 0;

    if(this->car())
    {
        ++len;
    }

    while(cur->cdr())
    {
        if(is_list(cur->cdr()))
        {
            ++len;
            cur = static_cast<const ListObject*>(cur->cdr());
        }
        else
        {
            set_exception(new Exception("bad list"));
            return -1;
        }
    }

    return len;
}


ListObject* pair(Object* car, Object* cdr)
{
    return new ListObject(car, cdr);
}


ListObject* null_list()
{
    return new ListObject(nullptr);
}


ListObject* try_list_cdr(ListObject* l)
{
    if(!l->cdr())
    {
        return nullptr;
    }
    if(!ListObject::is_list(l->cdr()))
    {
        return nullptr;
    }
    return static_cast<ListObject*>(l->cdr());
}
