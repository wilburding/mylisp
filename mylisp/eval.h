/*
 * =====================================================================================
 *
 *       Filename:  eval.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2013 21:09:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef EVAL_H
#define EVAL_H

class Object;
class Environment;
class Symbol;


bool eval_init();
void eval_fini();

Symbol* make_symbol(const char* name);

Object* eval(Object* obj, Environment* env);

#endif  // EVAL_H
