/*
 * =====================================================================================
 *
 *       Filename:  lambda.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/2013 21:49:29
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "lambda.h"
#include "list.h"
#include "environment.h"

#include <typeinfo>


bool Lambda::is_lambda(const Object* obj)
{
    return typeid(*obj) == typeid(Lambda);
}


Lambda::Lambda(ListObject* parameters, ListObject* body)
    :Object(),
    parameters_(parameters),
    body_(body)
{}


std::string Lambda::to_string() const
{
    char buf[256] = {0};
    std::snprintf(buf, sizeof(buf), "lambda#%p ", reinterpret_cast<const void*>(this));
    return std::string(buf) + parameters_->to_string();
}


Object* Lambda::eval(Environment* env)
{
    return new Procedure(this, env);
}


bool Procedure::is_procedure(const Object* obj)
{
    return typeid(*obj) == typeid(Procedure);
}


Procedure::Procedure(Lambda* lambda, Environment* env)
    :Object(),
    lambda_(lambda),
    env_(env)
{}


std::string Procedure::to_string() const
{
    char buf[256] = {0};
    std::snprintf(buf, sizeof(buf), "procedure#%p ", static_cast<const void*>(this));
    return std::string(buf) + this->parameters()->to_string();
}


Object* Procedure::eval(Environment* )
{
    return nullptr;
}
