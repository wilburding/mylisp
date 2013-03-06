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


static bool init()
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


static bool is_if_expr(ListObject* expr)
{
    return static_cast<Symbol*>(expr->car())->id() == SYMBOL_IF;
}


static Object* eval_if(ListObject* expr, Environment* env)
{
    if(expr->length() <= 2 || expr->length() > 4)
    {
        set_exception(new Exception("bad if expr!" + expr->to_string()));
        return nullptr;
    }

    Object *predict_expr = nullptr, 
           *consequence_expr = nullptr, 
           *alternative_expr = nullptr;

    ListObject* cdr = try_list_cdr(expr);
    predict_expr = cdr->car();
    ListObject* cddr = try_list_cdr(cdr);
    consequence_expr = cddr->car();
    ListObject* cdddr = try_list_cdr(cddr);
    if(cdddr)
        alternative_expr = cdddr->car();

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


static bool is_quoted_expr(ListObject* expr)
{
    return static_cast<Symbol*>(expr->car())->id() == SYMBOL_QUOTE;
}


static Object* eval_quoted(ListObject* expr, Environment* env)
{
    if(expr->length() != 2)
    {
        set_exception(new Exception("bad quoted expr!" + expr->to_string()));
        return nullptr;
    }
    return expr->cdr();
}


static bool is_assignment_expr(ListObject* expr)
{
    return static_cast<Symbol*>(expr->car())->id() == SYMBOL_SET;
}


static bool eval_assignment(ListObject* expr, Environment* env)
{
    if(expr->length() != 3)
    {
        set_exception(new Exception("bad assignment expr!" + expr->to_string()));
        return nullptr;
    }
    ListObject* cdr = try_list_cdr(expr);
    Object* cadr = cdr->car();
    if(!Symbol::is_symbol(cadr))
    {
        set_exception(new Exception("bad assignment expr! cadr must be a variable name! " + \
                    expr->to_string()));
        return nullptr;
    }
    Object* caddr = try_list_cdr(cdr)->car();
    Object* value = caddr->eval(env);
    if(!value)
    {
        return nullptr;
    }
    Symbol* variable = static_cast<Symbol*>(cadr);
    if(!env->set_variable(variable->name(), value))
    {
        return nullptr;
    }
    return null_list();
}



Object* ListObject::eval(Environment* env)
{
    // only compound expr
    int len = this->length();
    // a valid list (not a pair)?
    if(len == -1)
    {
        return nullptr;
    }

    if(Symbol::is_symbol(this->car()))
    {
        if(is_quoted_expr(this))
        {
            return eval_quoted(this, env);
        }
        else if(is_if_expr(this))
        {
            return eval_if(this, env);
        }
    }
    return nullptr;
}

