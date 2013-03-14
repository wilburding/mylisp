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


bool init();

Object* eval(Object* obj, Environment* env);

#endif  // EVAL_H
