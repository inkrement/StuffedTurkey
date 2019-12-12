#include <iostream>
#include <string>
#include <cstdint>
#include <stdlib.h> 
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <memory>

#include "json.hpp"
#include "cxxopts.hpp"

#include "vector.h"
#include "embedding.h"

using json = nlohmann::json;

using namespace std;
using namespace StuffedTurkey;


void printUsage(){
    std::cerr << "Usage: ./stuffedturkey command" << std::endl << std::endl;
    std::cerr << "possible commands: " << std::endl;
    std::cerr << " - help" << std::endl;
    std::cerr << " - view <embedding-file>" << std::endl;
    std::cerr << " - unit <input-file> <output-file>" << std::endl;
    
}

inline bool ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

uint32_t FThash(const std::string& str) {
  uint32_t h = 2166136261;
  for (size_t i = 0; i < str.size(); i++) {
    h = h ^ uint32_t(int8_t(str[i]));
    h = h * 16777619;
  }
  return h;
}

std::unique_ptr<Embedding> loadEmbedding(const std::string& filename){
    std::unique_ptr<Embedding> e(new Embedding());

    if (ends_with(filename, ".vec")){
        std::cout << "read vec-model" << std::endl;
        e->loadvec(filename);
    } else if (ends_with(filename, ".bin")) {
        std::cout << "read binary-model" << std::endl;
        e->loadbin(filename);
    } else {
        throw runtime_error("unknown format");
    }

    return e;
}

void printInfo(std::string filename){
    auto emb = loadEmbedding(filename); 

    std::cout << "Dimensions: " << emb->dim() << std::endl;
    std::cout << "Vocab size: " << emb->len() << std::endl;
    std::cout << "normalized: " << (emb->is_unit() ? "true" : "false") << std::endl;

    std::map<std::string, Item>::iterator iter = emb->begin();

    for (uint8_t i = 0; i < 10 && iter != emb->end(); i++, iter++){
        std::cout << iter->first;

        if (i != 9) {
            std::cout << ", ";
        }
    }
    std::cout <<  endl;
}

void unitEmbedding(std::string in_file, std::string out_file){
    auto emb = loadEmbedding(in_file);

    emb->unit();

    std::ofstream ofs(out_file);
    emb->dump(ofs);
}

int main(int argc, char * argv[]){
    if (argc <= 1){
        printUsage();
        exit (EXIT_FAILURE);
    }

    string command(argv[1]);

    if (command == "view") {
        if (argc == 3){
            printInfo(argv[2]);
        } else {
            printUsage();
        }
    } else if (command == "unit") {
        if (argc == 4){
            unitEmbedding(argv[2], argv[3]);
        } else {
            printUsage();
        }
    } else if (command == "help") {
        printUsage();
    }
    
    return 0;
}
