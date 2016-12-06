/** *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 12:10 PM
 *
 */

#ifndef FASTTEXT_WRAPPER_H
#define FASTTEXT_WRAPPER_H

#include <node.h>
#include <v8.h>

namespace FastTextWrapper
{
    class FastTextWrapper
    {
      public:
        void train(int argc, char** argv);
        void textVectors(std::vector<std::string> words);
        void printVectors(std::string filename, std::vector<std::string> words);
        void loadModel(std::string filename);
    };
}

#endif