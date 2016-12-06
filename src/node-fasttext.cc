#include <node.h>
#include <v8.h>
#include <iostream>

#include "lib/fasttext-wrapper.h"
#include "lib/node-argument.h"

FastTextWrapper::FastTextWrapper fasttextWrapper;

void train(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  // callback index in second order (index start from zero)
  v8::Local<v8::Function> callbackIndex = v8::Local<v8::Function>::Cast(args[2]);

  if (args.Length() != 3) 
  {
    // number of callback params (in this case 2 argument/parameter)
    v8::Local<v8::Value> callback[2] = 
    { 
      // array of callback value
      v8::Null(isolate),
      v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Not enough arguments."))
    };

    // will be the same as above (number of callback params)
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  } 
  
  if (!args[0]->IsString()) 
  {
    v8::Local<v8::Value> callback[2] = 
    { 
      v8::Null(isolate),
      v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "First argument must be a command string."))
    };
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  } 
  
  if (!args[1]->IsObject())
  {
    v8::Local<v8::Value> callback[2] = 
    { 
      v8::Null(isolate),
      v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Second argument must be an object."))
    };
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  }

  // Now do anything you want
  v8::String::Utf8Value commandArg(args[0]->ToString());
  std::string command = std::string(*commandArg);

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[1]);

  NodeArgument::NodeArgument nodeArg;

  NodeArgument::CArgument c_argument = nodeArg.ObjectToCArgument(obj);
  
  int count = c_argument.argc;
  char** argument = c_argument.argv;
  // prepend new string command to argv
  char *emptyString = (char *)"-command";
  char *argv[count + 2];
  int argc = count + 2; // increment the argc

  for(int j = 0; j < count; j++)
  {
    argv[j + 2] = argument[j];
  }

  argv[0] = emptyString;
  argv[1] = (char *) command.c_str();

  // nodeArg.PrintArguments(argv, argc); // for debugging purpose

  if (command == "skipgram" || command == "cbow" || command == "supervised") 
  {
    fasttextWrapper.train(argc, argv);

    v8::Local<v8::Value> callback[2] = 
    { 
      v8::String::NewFromUtf8(isolate, "SUCCESS"),
      v8::Null(isolate)      
    };
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;

  }
  else
  {
    v8::Local<v8::Value> callback[2] = 
    { 
      v8::Null(isolate),
      v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Permitted command type is ['skipgram', 'cbow', 'supervised']."))    
    };

    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  }
}

void printVectors(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  // callback index in second order (index start from zero)
  v8::Local<v8::Function> callbackIndex = v8::Local<v8::Function>::Cast(args[2]);

  if (args.Length() != 3) 
  {
    // number of callback params (in this case 2 argument/parameter)
    v8::Local<v8::Value> callback[2] = 
    { 
      // array of callback value
      v8::Null(isolate),
      v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Not enough arguments."))
    };

    // will be the same as above (number of callback params)
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  } 
  
  if (!args[0]->IsString()) 
  {
    v8::Local<v8::Value> callback[2] = 
    { 
      v8::Null(isolate),
      v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "First argument must be string (model name)."))
    };
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  } 
  
  if (!args[1]->IsArray())
  {
    v8::Local<v8::Value> callback[2] = 
    { 
      v8::Null(isolate),
      v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Second argument must be an array."))
    };
    callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
    return;
  }

  // Do Anything Here

  // Get model name
  v8::String::Utf8Value modelName(args[0]->ToString());
  std::string model = std::string(*modelName);

  // get array of string in first order (index start from zero)
  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[1]);

  NodeArgument::NodeArgument nodeArg;

  std::vector<std::string> words = nodeArg.ObjectToArrayString(obj);

  // for debuging purpose
  // std::cout << "vector size: " << words.size() << std::endl;
  // for (uint32_t i = 0; i < words.size(); i++)
  // {
  //   std::cout << words[i] << std::endl;
  // }
  
  fasttextWrapper.printVectors(model, words);

  v8::Local<v8::Value> callback[2] = 
  { 
    v8::String::NewFromUtf8(isolate, "SUCCESS"),
    v8::Null(isolate)      
  };

  callbackIndex->Call(isolate->GetCurrentContext()->Global(), 2, callback);
  return;
}

void Init(v8::Handle<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "train", train);
  NODE_SET_METHOD(exports, "printVectors", printVectors);
}


NODE_MODULE(NodeFastText, Init)