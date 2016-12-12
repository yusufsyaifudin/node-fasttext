var FastText = require('./index');

let config = { 
  dim: 100,
  input: "data/dbpedia.train",
  output: "dbpedia"
}

// FastText.train("supervised", config, function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }
  
//   console.log(success)
  
// })

FastText.printVectors(
"dbpedia.bin",
['word'],
function (success, error) {

  if(error) {
    console.log(error)
    return;
  }

  for(let i=0; i<success.length; i++) {
    console.log(success[i].key)
  }
})

// FastText.modelInfo("dbpedia.bin", function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }
  
//   console.log(success)
  
// })


// FastText.test("dbpedia.bin", "data/dbpedia.test", 1, function (data, error) {
//   if(error) {
//     console.log(error)
//     return;
//   }
//   console.log(data)
  
// });