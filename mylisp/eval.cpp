/*
 * =====================================================================================
 *
 *       Filename:  eval.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/23/2013 22:52:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "mylisp.h"
#include "list.h"


#define DECLARE_SYMBOL(name) static SymbolID name = INVALID_SYMBOL_ID;
#define DEFINE_SYMBOL(name, value) name = symbols.add_symbol(#value);

DECLARE_SYMBOL(SYMBOL_QUOTE)
DECLARE_SYMBOL(SYMBOL_DEFINE)
DECLARE_SYMBOL(SYMBOL_IF)
DECLARE_SYMBOL(SYMBOL_ELSE)
DECLARE_SYMBOL(SYMBOL_LAMBDA)
DECLARE_SYMBOL(SYMBOL_BEGIN)
DECLARE_SYMBOL(SYMBOL_COND)
DECLARE_SYMBOL(SYMBOL_SET)

bool init()
{
    Symbols& symbols = Symbols::instance();

    DEFINE_SYMBOL(SYMBOL_QUOTE, quote)
    DEFINE_SYMBOL(SYMBOL_DEFINE, define)
    DEFINE_SYMBOL(SYMBOL_IF, if)
    DEFINE_SYMBOL(SYMBOL_ELSE, else)
    DEFINE_SYMBOL(SYMBOL_LAMBDA, lambda)
    DEFINE_SYMBOL(SYMBOL_BEGIN, begin)
    DEFINE_SYMBOL(SYMBOL_COND, cond)
    DEFINE_SYMBOL(SYMBOL_SET, set!)

    return true;
}


Object* eval(Object* obj, Environment* env)
{
    return obj->eval(env);
}


Object* Symbol::eval(Environment* env)
{
    auto result = env->look_up_variable(this->name());
    if(!result)
    {
        set_exception(new Exception("could not resolve symbol " + this->to_string()));
    }
    return result;
}


bool is_if_expr(ListObject* expr)
{
    return static_cast<Symbol*>(expr->value())->id() == SYMBOL_IF;
}


Object* eval_if(ListObject* expr, Environment* env)
{
    if(expr->length() <= 2 || expr->length() > 4)
    {
        set_exception(new Exception("bad if expr!"));
        return nullptr;
    }

    Object* predict_expr = expr->next()->value();
    Object* consequence_expr = expr->next()->next()->value();
    Object* alternative_expr = expr->next()->next()->next()? expr->next()->next()->next(): nullptr;

    Object* predict_value = predict_expr->eval(env);
    if(predict_value)
    {
        if(is_true(predict_value))
        {
            return consequence_expr->eval(env);
        }
        else
        {
            if(alternative_expr)
            {
                return alternative_expr->eval(env);
            }
            else
            {
                return new BoolObject(false);
            }
        }
    }
    else
    {
        // error already set
        return nullptr;
    }
}


Object* ListObject::eval(Environment* env)
{
    if(typeid(*this->value()) == typeid(Symbol))
    {
        if(is_if_expr(this))
        {
            return eval_if(this, env);
        }
    }
    return nullptr;
}
