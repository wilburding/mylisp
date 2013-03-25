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


#define DECLARE_SYMBOL(name) static SymbolID name = INVALID_SYMBOL_ID;
#define DEFINE_SYMBOL(name, value) name = g_pSymbols->add(#value);


DECLARE_SYMBOL(SYMBOL_QUOTE)
DECLARE_SYMBOL(SYMBOL_DEFINE)
DECLARE_SYMBOL(SYMBOL_IF)
DECLARE_SYMBOL(SYMBOL_ELSE)
DECLARE_SYMBOL(SYMBOL_LAMBDA)
DECLARE_SYMBOL(SYMBOL_BEGIN)
DECLARE_SYMBOL(SYMBOL_COND)
DECLARE_SYMBOL(SYMBOL_SET)


static Symbols* g_pSymbols;


bool eval_init()
{
    g_pSymbols = new Symbols;

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


void eval_fini()
{
    delete g_pSymbols;
}


Symbol* make_symbol(const char* name)
{
    assert(g_pSymbols != nullptr);
    return new Symbol(name, g_pSymbols);
}


Object* eval(Object* obj, Environment* env)
{
    return obj->eval(env);
}


static bool is_if_expr(const ListObject* expr)
{
    return static_cast<const Symbol*>(expr->car())->id() == SYMBOL_IF;
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
                return null_list();
            }
        }
    }
    else
    {
        // error already set
        return nullptr;
    }
}


static bool is_quoted_expr(const ListObject* expr)
{
    return static_cast<const Symbol*>(expr->car())->id() == SYMBOL_QUOTE;
}


static Object* eval_quoted(ListObject* expr, Environment*)
{
    if(expr->length() != 2)
    {
        set_exception(new Exception("bad quoted expr!" + expr->to_string()));
        return nullptr;
    }
    return try_list_cdr(expr)->car();
}


static bool is_assignment_expr(const ListObject* expr)
{
    return static_cast<const Symbol*>(expr->car())->id() == SYMBOL_SET;
}


static Object* eval_assignment(ListObject* expr, Environment* env)
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
    if(!env->set_variable(variable->id(), value))
    {
        set_exception(new Exception("bad assignment expr! unbound variable! " +  \
                    expr->to_string()));
        return nullptr;
    }
    else
    {
        return null_list();
    }
}


static bool is_definition_expr(const ListObject* expr)
{
    return static_cast<const Symbol*>(expr->car())->id() == SYMBOL_DEFINE;
}


static Object* eval_definition(ListObject* expr, Environment* env)
{
    if(expr->length() != 3)
    {
        set_exception(new Exception("bad definition expr!" + expr->to_string()));
        return nullptr;
    }

    ListObject* cdr = try_list_cdr(expr);
    Object* cadr = cdr->car();
    if(!Symbol::is_symbol(cadr))
    {
        set_exception(new Exception("bad definition expr! cadr must be a variable name! " + \
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
    env->define_variable(variable->id(), value);

    return null_list();
}


bool is_sequence_expr(const ListObject* expr)
{
    return static_cast<const Symbol*>(expr->car())->id() == SYMBOL_BEGIN;
}


static Object* eval_sequence(ListObject* expr, Environment* env)
{
    //without the begin head
    ListObject* cur = expr;
    Object* result = nullptr;
    while(cur && cur->cdr())  // cdr to check nil
    {
        result = cur->eval(env);
        if(!result)
        {
            return nullptr;
        }
    }
    return result;
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
        else if(is_assignment_expr(this))
        {
            return eval_assignment(this, env);
        }
        else if(is_definition_expr(this))
        {
            return eval_definition(this, env);
        }
        else if(is_sequence_expr(this))
        {
            return eval_sequence(this->cdr_as_list(), env);
        }
        else
        {
            assert(false);
        }
    }
    else if(ListObject::is_list(this->car()))
    {
        Object* result = this->car()->eval(env);
        if(!result)
        {
            return nullptr;
        }
        // apply
    }
    return nullptr;
}

