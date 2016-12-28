var FastText = require('./index');
var chai = require('chai')
var assert = chai.assert

let config = { 
  dim: 100,
  input: "data/dbpedia.train",
  output: "result/dbpedia"
}

describe('Testing...', () => {

  it('should train successfully', done => {
    FastText.train("supervised", config, function (data, error) {

      if(error) {
        console.log(error)
        return;
      }
      
      // console.log(data)
      assert.equal(data.word_count, 803537, "word count is same")
      assert.equal(data.token_count, 32833702, "token count is same")
      assert.equal(data.label_count, 14, "label count is same")

      done()
    })    
  })

  it('should print word vector', done => {
    FastText.printVectors(
    "result/dbpedia.bin",
    ['word'],
    function (data, error) {

      if(error) {
        console.log(error)
        return;
      }

      // console.log(data)
      assert.equal(data.length, 1, "vector length is same")
      assert.equal(data[0].key, 'word', "the vector key is same")

      done()
    })
  })

  it('should return model info', done => {
    FastText.modelInfo("result/dbpedia.bin", function (data, error) {

      if(error) {
        console.log(error)
        return;
      }

      // console.log(data)
      assert.equal(data.word_count, 803537, "word count is same")
      assert.equal(data.token_count, 32833702, "token count is same")
      assert.equal(data.label_count, 14, "label count is same")

      done()
    })
  })

  it('should test properly', done => {
    FastText.test("result/dbpedia.bin", "data/dbpedia.test", 1, function (data, error) {
      if(error) {
        console.log(error)
        return;
      }
    
      // console.log(data)
      for (let i = data.length - 1; i >= 0; i--) {
        if (data[i].key == 'Number of examples') {
          assert.equal(data[i].value, 70000, "number of examples is true")
        }

        if (data[i].key == 'P@1') {
          assert.isAbove(data[i].value, 0.9, "P@1 true")
        }

        if (data[i].key == 'R@1') {
          assert.isAbove(data[i].value, 0.9, "R@1 true")
        }
      }      

      done()
    });
  })

  it('should predict properly', done => {

    FastText.predict(
    "result/dbpedia.bin", 1,
    ['cola lukei , cola lukei is a species of flowering plant in the malvaceae family . it is found only in tanzania . its natural habitat is subtropical or tropical moist lowland forests . it is threatened by habitat loss .'],
    function (data, error) {

      if(error) {
        console.log(error)
        return;
      }
      
      assert.equal(data[0].label, '__label__11', "true detection label")
      // console.log(success[0].label == '__label__11')
      // console.log(success)

      done()
    })   
  })

})