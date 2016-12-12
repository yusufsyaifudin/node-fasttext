var fasttext = require('bindings')('node-fasttext');

let config = { 
  dim: 100,
  input: "data/dbpedia.train",
  output: "dbpedia"
}

// fasttext.train("supervised", config, function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }
  
//   console.log(success)
  
// })

// fasttext.printVectors(
// "dbpedia.bin",
// ['word'],
// function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }

//   console.log(success)
// })

// fasttext.modelInfo("dbpedia.bin", function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }
  
//   console.log(success)
  
// })


// fasttext.test("dbpedia.bin", "data/dbpedia.test", 1, function (data, error) {
//   if(error) {
//     console.log(error)
//     return;
//   }
//   console.log(data)
  
// });