/** *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 12:10 PM
 *
 */

#ifndef FASTTEXT_WRAPPER_H
#define FASTTEXT_WRAPPER_H

#include <node.h>
#include <v8.h>
#include "fasttext/src/real.h"

namespace FastTextWrapper
{
    struct PredictResult
    {
      std::string text;
      std::string label;
      double value;
    };

    class FastTextWrapper
    {
      public:
        bool fileExist(const std::string& filename);
        std::map<std::string, std::string> train(int argc, char** argv);
        std::vector<std::map<std::string, std::vector<double>>> wordVectors(std::vector<std::string> words);
        std::vector<std::map<std::string, std::vector<double>>> textVectors(std::vector<std::string> words);
        std::vector<std::map<std::string, std::vector<double>>> printVectors(std::string filename, std::vector<std::string> words);
        std::map<std::string, std::string> loadModel(std::string filename);
        std::map<std::string, std::string> test(std::string model, std::string testFile, int32_t k);
        void predict(std::string sentence, int32_t k, std::vector<std::pair<fasttext::real,std::string>>& predictions) const;
        std::vector<PredictResult> predict(std::string model, std::vector<std::string> sentences, int32_t k);
    };
}

#endif