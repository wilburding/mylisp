/*
 * =====================================================================================
 *
 *       Filename:  exception.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/29/2013 23:03:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "exception.h"
#include "assert.h"

#include "stdio.h"


static Exception* current_exception = nullptr;


void set_exception(Exception* exception)
{
    if(exception)
        assert(current_exception == nullptr);

    current_exception = exception;
}


Exception* get_exception()
{
    return current_exception;
}


Exception::Exception(const char* message)
    :Object(),
    message_(message)
{}


Exception::Exception(const std::string& message)
    :Object(),
    message_(message)
{}


Exception::Exception(std::string&& message)
    :Object(),
    message_(std::move(message))
{}


std::string Exception::to_string() const 
{
    return { "Error: " + message_ };
}
