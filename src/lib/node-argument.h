/**
 * This is header file to convert node object arugment to standar argv C or C++ argument
 *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 10:57 AM
 *
 */

#ifndef NODEARGUMENT_NODEARGUMENT_H
#define NODEARGUMENT_NODEARGUMENT_H

#include <node.h>
#include <v8.h>

namespace NodeArgument
{
    struct CArgument
    {
      size_t argc;
      char** argv;
    };

    class NodeArgument
    {

      public:
        char* concat(const char *s1, const char *s2);
        int AddStringArgument(char*** strings, size_t* count, const char* newStr);
        void PrintArguments(char** strings, size_t count);
        CArgument ObjectToCArgument(v8::Local<v8::Object> obj);
        std::vector<std::string> ObjectToArrayString(v8::Local<v8::Object> obj);
    };
}

#endif