/*
 * =====================================================================================
 *
 *       Filename:  debug.cpp
 *
 *    Description:  debug related
 *
 *        Version:  1.0
 *        Created:  03/28/2013 23:13:22
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "debug.h"
#include "util.h"

#include <memory>
#include <functional>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>


void dump_stack()
{
    enum { MAX_STACK_DEPTH = 128 };
    void* callstack[MAX_STACK_DEPTH] = {0};
    int frames = backtrace(callstack, array_size(callstack));
    /*char** strs = backtrace_symbols(callstack, frames);*/
    std::unique_ptr<char*, decltype(free)*> strs(
            backtrace_symbols(callstack, frames), free);
    for(int i = 0; i < frames; ++i)
    {
        fprintf(stderr, "%s\n", strs.get()[i]);
    }
    /*free(strs);*/
}
