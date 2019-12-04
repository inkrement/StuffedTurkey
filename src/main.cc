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
using namespace StuffedTurkey;


Item mean(std::vector<Item> items) {
    if (items.size() == 0){
        throw std::runtime_error("cannot calculate mean vector given ");
    }
    
    uint32_t dim = items[0].size();
    
    
    // input array
    Vector nv(dim);
    
    for (uint32_t ci = 0; ci < dim; ci++) {
        // component
        float tc = 0.0;
        
        for (uint32_t vi = 0; vi < items.size(); vi++) {
            //Vector v = *vecs[vi];
            //tc += v[ci];
            tc += items[vi][ci];
        }
        
        nv[ci] = tc/items.size();
    }
    
    return Item(nv);
}


void printUsage(){
    std::cerr << "Usage: ./stuffedturkey mapping.json input_emb.vec output_emb.vec" << std::endl;
}

void printInfo(std::string filename){
    // load original embedding
    Embedding orig_emb = Embedding::loadvec(filename);
    //TODO
    //std::cerr << "Usage: ./stuffedturkey mapping.json input_emb.vec output_emb.vec" << std::endl;
}

int main(int argc, char * argv[]){
    if (argc <= 1){
        printUsage();
        exit (EXIT_FAILURE);
    }

    string command(argv[1]);


    if (command == "view") {
        if (argc == 2){
            printInfo(argv[2]);
        } else {
            printUsage();
        }
    }
    
    //orig_emb.dump(std::cout);
    
    // load and iterate mapping
    /*
    std::ifstream mfs(argv[1]);
    json j;
    mfs >> j;
    
    Embedding new_emb(orig_emb.dim());
    
    
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        vector<std::string> item_ids = *it;
        
        // generate place for vectors
        vector<Item> items;
        
        for (uint32_t i = 0; i < item_ids.size(); i++){
            string idx = item_ids[i];
            
            if( orig_emb.contains(idx)){
                items.push_back(orig_emb[idx]);
            }
        }
        
        
        
        // aggregation step
        if (items.size() > 0){
            // convert vector to array
            // TODO: normalize?
            // it.key() contains label
            //new_emb.insert(make_pair(it.key(), Vector::mean(items)));
            //TODO: fix me.. item statt vector
            
            
            Item ni = Item::aggregate(items, mean);
            new_emb.insert(make_pair(it.key(), mean(items)));
        }
        
        break;
    }
    */
    
    /*
    // save embedding
    std::ofstream ofs(argv[3]);
    
    new_emb.dump(ofs);
    
    */
   
    
    return 0;
}
