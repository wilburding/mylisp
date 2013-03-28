/*
 * =====================================================================================
 *
 *       Filename:  debug_helper.cpp
 *
 *    Description:  write down traceback when i die
 *
 *        Version:  1.0
 *        Created:  03/28/2013 23:31:18
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


static void sigsegv_handler(int)
{
    fprintf(stderr, "Something terrable happened! See below!\n");
    dump_stack();
    abort();
}

sig_t _dummy = signal(SIGSEGV, sigsegv_handler);
