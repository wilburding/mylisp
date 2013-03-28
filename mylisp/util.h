/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2013 22:56:38
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Wilbur Ding (), powerx1202@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef UTIL_H
#define UTIL_H

#include <string.h>


struct CStrHash
{
    // std::hash<char*> only hash the string's address..
    size_t operator()(const char* str) const
    {
        // sdbm algorithm.
        // see http://www.cse.yorku.ca/~oz/hash.html
        register size_t hash = 0;
        register int c = 0;
        while((c = *str++))
        {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return hash;
    }
};


struct CStrEqualTo
{
    // std::equal_to<char*> only compare arguments' addresses..
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};


template<class T, size_t N>
constexpr size_t array_size(T (&)[N]) { return N; }


#endif  // UTIL_H
