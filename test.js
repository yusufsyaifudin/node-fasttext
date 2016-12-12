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

// FastText.printVectors(
// "dbpedia.bin",
// ['word', 'word', 'another', 'words', 'here'],
// function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }

//   console.log(success.length)
// })

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

// FastText.predict(
// "dbpedia.bin", 1,
// ['cola lukei , cola lukei is a species of flowering plant in the malvaceae family . it is found only in tanzania . its natural habitat is subtropical or tropical moist lowland forests . it is threatened by habitat loss .'],
// function (success, error) {

//   if(error) {
//     console.log(error)
//     return;
//   }

//   console.log(success[0].label == '__label__11')
//   console.log(success)
// })