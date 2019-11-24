#include <iostream>
#include <string>
#include <cstdint>
#include <stdlib.h> 

#include "vector.h"
#include "embedding.h"
#include <fstream>
#include <sstream>

#include "json.hpp"
#include "cxxopts.hpp"

using json = nlohmann::json;

using namespace std;
using namespace eagg;


std::pair<std::string, Vector*> parseline(string line, Vector* vector){
    istringstream iss(line);
    
    // load label
    string label;
    iss >> label;
    
    // fill vector
    for (int i = 0; i < vector->size(); i++){
        iss >> vector->data()[i];
    }
    
    //cout << label << " " << *vector << endl;
    
    return make_pair(label, vector);
}

Embedding parsevec(ifstream &in){
    int64_t num;
    int64_t dim;
    
    in >> num;
    in >> dim;
    
    Embedding emb(dim);
    std::string line;
    
    // skip one empty line
    getline(in, line);
    
    int32_t lines = 0;
    
    while (getline(in, line)){
        emb.insert(parseline(line, new Vector(dim)));
        
        lines++;
    }
    
    return emb;
}

void printUsage(){
    std::cerr << "Usage: ./embagg mapping.json input_emb.vec output_emb.vec" << std::endl;
}

int main(int argc, char * argv[]){
    if (argc != 4){
        printUsage();
        exit (EXIT_FAILURE);
    }
    
    // load original embedding
    std::ifstream ifs(argv[2]);
    Embedding orig_emb = parsevec(ifs);
    
    // load and iterate mapping
    std::ifstream mfs(argv[1]);
    json j;
    mfs >> j;
    
    Embedding new_emb(orig_emb.dim());
    
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        vector<std::string> item_ids = *it;
        
        // generate place for vectors
        vector<Vector*> items_vecs;
        
        for (uint32_t i = 0; i < item_ids.size(); i++){
            if( orig_emb.contains(item_ids[i]) ){
                items_vecs.push_back(orig_emb[item_ids[i]]);
            }
        }
        
        // aggregation step
        if (items_vecs.size() > 0){
            // convert vector to array
            // TODO: normalize?
            // it.key() contains label
            new_emb.insert(make_pair(it.key(), Vector::mean(items_vecs)));
        }
    }
    
    
    // save embedding
    std::ofstream ofs(argv[3]);
    
    new_emb.dump(ofs);
    
    return 0;
}