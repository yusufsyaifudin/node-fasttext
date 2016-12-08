#include <iostream>
#include <sstream>
#include <fenv.h>
#include <math.h>
#include <string>

#include <iomanip>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "fasttext/src/args.h"
#include "fasttext/src/fasttext.h"

#include "fasttext-wrapper.h"


namespace FastTextWrapper {

  std::shared_ptr<fasttext::Args> args_ = std::make_shared<fasttext::Args>();
  std::shared_ptr<fasttext::Dictionary> dict_ = std::make_shared<fasttext::Dictionary>(args_);
  std::shared_ptr<fasttext::Matrix> input_ = std::make_shared<fasttext::Matrix>();
  std::shared_ptr<fasttext::Matrix> output_ = std::make_shared<fasttext::Matrix>();

  std::shared_ptr<fasttext::Model> model_ = NULL;

  bool FastTextWrapper::fileExist(const std::string& filename)
  {
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
      fclose(file);
      return true;
    }
    else
    {
      return false;
    }
  }

  std::map<std::string, std::string> FastTextWrapper::train(int argc, char** argv) 
  {
    std::shared_ptr<fasttext::Args> a = std::make_shared<fasttext::Args>();
    a->parseArgs(argc, argv);
    fasttext::FastText fasttext;

    std::cout << "Input >>>>>>>> " << a->input << std::endl;
    std::cout << "Output >>>>>>>> " << a->output << std::endl;

    std::string inputFilename = a->input;
    if ( !fileExist(inputFilename) )
    {
      std::string errorMessage = "Input file is not exist.";
      throw errorMessage;
    }

    fasttext.train(a);
    return loadModel(a->output + ".bin");
  }

  std::map<std::string, std::vector<double>> FastTextWrapper::wordVectors(std::vector<std::string> words)
  {
    std::map<std::string, std::vector<double>> response;
    std::cout << "WORD VECTOR" << std::endl;

    fasttext::Vector vec(args_->dim);
    for(uint32_t i = 0; i < words.size(); i++)
    {
      // fasttext get vector
      const std::vector<int32_t>& ngrams = dict_->getNgrams(words[i]);
      vec.zero();
      for (auto it = ngrams.begin(); it != ngrams.end(); ++it) {
        vec.addRow(*input_, *it);
      }

      if (ngrams.size() > 0) {
        vec.mul(1.0 / ngrams.size());
      }

      // tmp variable
      std::vector<double> arr(vec.size());
      for ( int64_t i = 0; i < vec.size(); i++ )
      {
        arr[i] = vec[i];
        // std::cout << vec[i] << std::endl;
      }
      
      response[words[i]] = arr;
    }

    return response;
  }

  std::map<std::string, std::vector<double>> FastTextWrapper::textVectors(std::vector<std::string> words)
  {
    std::map<std::string, std::vector<double>> response;
    std::cout << "TEXT VECTOR" << std::endl;

    std::vector<int32_t> line, labels;
    fasttext::Vector vec(args_->dim);

    for(uint32_t i = 0; i < words.size(); i++)
    {
      std::istringstream input_string(words[i]);
      dict_->getLine(input_string, line, labels, model_->rng);
      dict_->addNgrams(line, args_->wordNgrams);
      vec.zero();

      for (auto it = line.cbegin(); it != line.cend(); ++it)
      {
        vec.addRow(*input_, *it);
      }

      if (!line.empty())
      {
        vec.mul(1.0 / line.size());
      }

      // std::cout << words[i] << std::endl;

      std::vector<double> arr(vec.size());
      for ( int64_t i = 0; i < vec.size(); i++ )
      {
        arr[i] = vec[i];
        // std::cout << vec[i] << std::endl;
      }
      
      response[words[i]] = arr;
    }

    // std::cout << "==============================" << std::endl;
    // std::cout << "====== debugging mode ========" << std::endl;
    // std::cout << "==============================" << std::endl;

    // for (auto const& iterator : response)
    // {
    //   std::cout << iterator.first << std::endl;
    //   for (uint i = 0; i < iterator.second.size(); ++i)
    //   {
    //     std::cout << iterator.second[i] << std::endl;
    //   }
    // }

    return response;
  }

  std::map<std::string, std::vector<double>> FastTextWrapper::printVectors(std::string filename, std::vector<std::string> words)
  {
    loadModel(filename);

    if (args_->model == fasttext::model_name::sup) {
      return textVectors(words);
    } else {
      return wordVectors(words);
    }
  }

  std::map<std::string, std::string> FastTextWrapper::loadModel(std::string filename)
  {
    std::map<std::string, std::string> response;

    std::ifstream ifs(filename, std::ifstream::binary);
    if (!ifs.is_open()) {
      std::string errorMessage = "Model file cannot be opened for loading!";
      throw errorMessage;
    }

    args_->load(ifs);
    dict_->load(ifs);
    input_->load(ifs);
    output_->load(ifs);

    model_ = std::make_shared<fasttext::Model>(input_, output_, args_, 0);

    if (args_->model == fasttext::model_name::sup) {
      model_->setTargetCounts(dict_->getCounts(fasttext::entry_type::label));
    } else {
      model_->setTargetCounts(dict_->getCounts(fasttext::entry_type::word));
    }


    // dictionary
    response["word_count"] = std::to_string( dict_->nwords() );
    response["label_count"] = std::to_string( dict_->nlabels() );
    response["token_count"] = std::to_string( dict_->ntokens() );

    // arguments
    response["lr"] = std::to_string( args_->lr );
    response["dim"] = std::to_string( args_->dim );
    response["ws"] = std::to_string( args_->ws );
    response["epoch"] = std::to_string( args_->epoch );
    response["minCount"] = std::to_string( args_->minCount );
    response["minCountLabel"] = std::to_string( args_->minCountLabel );
    response["neg"] = std::to_string( args_->neg );
    response["wordNgrams"] = std::to_string( args_->wordNgrams );

    std::string loss_name = "";
    if(args_->loss == fasttext::loss_name::hs)
    {
      loss_name = "hs";
    }
    else if (args_->loss == fasttext::loss_name::ns)
    {
      loss_name = "ns";
    }
    else if (args_->loss == fasttext::loss_name::softmax)
    {
      loss_name = "softmax";
    }

    std::string model_name = "";
    if(args_->model == fasttext::model_name::cbow)
    {
      model_name = "cbow";
    }
    else if (args_->model == fasttext::model_name::sup)
    {
      model_name = "supervised";
    }
    else if (args_->model == fasttext::model_name::sg)
    {
      model_name = "skipgram";
    }

    response["loss"] = loss_name;
    response["model"] = model_name;
    response["bucket"] = std::to_string( args_->bucket );
    response["minn"] = std::to_string( args_->minn );
    response["maxn"] = std::to_string( args_->maxn );
    response["thread"] = std::to_string( args_->thread );
    response["lrUpdateRate"] = std::to_string( args_->lrUpdateRate );
    response["t"] = std::to_string( args_->t );
    response["label"] = args_->label;
    response["verbose"] = std::to_string( args_->verbose );
    response["pretrainedVectors"] = args_->pretrainedVectors;

    ifs.close();

    return response;
  }

}