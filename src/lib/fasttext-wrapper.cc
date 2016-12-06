#include <iostream>
#include <sstream>
#include <fenv.h>
#include <math.h>

#include <iomanip>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>

#include "fasttext/src/args.h"
#include "fasttext/src/fasttext.h"


#include "fasttext-wrapper.h"


namespace FastTextWrapper {

  std::shared_ptr<fasttext::Args> args_ = std::make_shared<fasttext::Args>();
  std::shared_ptr<fasttext::Dictionary> dict_ = std::make_shared<fasttext::Dictionary>(args_);
  std::shared_ptr<fasttext::Matrix> input_ = std::make_shared<fasttext::Matrix>();
  std::shared_ptr<fasttext::Matrix> output_ = std::make_shared<fasttext::Matrix>();

  std::shared_ptr<fasttext::Model> model_ = NULL;

  void FastTextWrapper::train(int argc, char** argv) 
  {
    std::shared_ptr<fasttext::Args> a = std::make_shared<fasttext::Args>();
    a->parseArgs(argc, argv);
    fasttext::FastText fasttext;

    std::cout << "Output >>>>>>>> " << a->output << std::endl;
    fasttext.train(a); 

    loadModel(a->output + ".bin");
  }

  void FastTextWrapper::textVectors(std::vector<std::string> words)
  {
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

      std::cout << words[i] << std::endl;
      std::cout << vec << std::endl;
    }
  }

  void FastTextWrapper::printVectors(std::string filename, std::vector<std::string> words)
  {
    loadModel(filename);

    if (args_->model == fasttext::model_name::sup) {
      textVectors(words);
    } else {
      // wordVectors();
      std::cout << "WORD VECTOR" << std::endl;
    }
  }

  void FastTextWrapper::loadModel(std::string filename)
  {
    std::ifstream ifs(filename, std::ifstream::binary);
    if (!ifs.is_open()) {
      std::cerr << "Model file cannot be opened for loading!" << std::endl;
      exit(EXIT_FAILURE);
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

    std::cout << "jumlah kata: " << dict_->nwords() << std::endl;
    std::cout << "jumlah label: " << dict_->nlabels() << std::endl;
    std::cout << "jumlah token: " << dict_->ntokens() << std::endl;

    ifs.close();
  }

}