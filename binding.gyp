{
  "targets": [
    {
      "target_name": "node-fasttext",
      "sources": [
        "src/lib/node-argument.cc",
        "src/lib/fasttext/src/args.cc",
        "src/lib/fasttext/src/dictionary.cc",
        "src/lib/fasttext/src/matrix.cc",
        "src/lib/fasttext/src/vector.cc",
        "src/lib/fasttext/src/model.cc",
        "src/lib/fasttext/src/utils.cc",
        "src/lib/fasttext/src/fasttext.cc",
        "src/lib/fasttext-wrapper.cc",
        "src/node-fasttext.cc"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}