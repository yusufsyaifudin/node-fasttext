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
        bool fileExist(const std::string& filename);
        std::map<std::string, std::string> train(int argc, char** argv);
        void textVectors(std::vector<std::string> words);
        void printVectors(std::string filename, std::vector<std::string> words);
        std::map<std::string, std::string> loadModel(std::string filename);
    };
}

#endif