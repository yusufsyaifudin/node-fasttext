var fasttext = require('bindings')('node-fasttext');

fasttext.train("supervised",
{ 
  dim: 100,
  input: "data/dbpedia.train",
  output: "dbpedia"
}, function (success, error) {
  console.log(success)
  console.log(error)
})

// fasttext.printVectors(
// "dbpedia.bin",
// ['word', 'as', 'test'],
// function (success, error) {
//   console.log(success)
//   console.log(error)
// })