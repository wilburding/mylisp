/*
 * =====================================================================================
 *
 *       Filename:  lambda.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/21/2013 21:49:13
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef LABMDA_H
#define LAMBDA_H

#include "object.h"


class Environment;
class ListObject;


class Lambda: public Object
{
public:
    static bool is_lambda(const Object* obj);

public:
    Lambda(ListObject* parameters, ListObject* body);

    virtual std::string to_string() const;
    virtual std::string type_name() const { return "lambda"; }

    virtual Object* eval(Environment* env) override;

    ListObject* parameters() { return parameters_; }
    const ListObject* parameters() const { return parameters_; }

    ListObject* body() { return body_; }
    const ListObject* body() const { return body_; }
private:
    ListObject* parameters_;
    ListObject* body_;
};


class Procedure: public Object
{
public:
    static bool is_procedure(const Object* obj);
public:
    Procedure(Lambda* lambda, Environment* env);

    virtual std::string to_string() const override;
    virtual std::string type_name() const override { return "procedure"; }

    virtual Object* eval(Environment* env) override;

    ListObject* parameters() { return lambda_->parameters(); }
    const ListObject* parameters() const { return lambda_->parameters(); }

    ListObject* body() { return lambda_->body(); }
    const ListObject* body() const { return lambda_->body(); }

    Environment* env() { return env_; }
private:
    Lambda* lambda_;
    Environment* env_;
};


#endif  // LABMDA_H
