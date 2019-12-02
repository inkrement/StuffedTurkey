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


std::pair<std::string, Item> parseline(string line, int64_t dim){
    Vector vec(dim);
    
    istringstream iss(line);
    
    // load label
    string label;
    iss >> label;
    
    // fill vector
    for (int i = 0; i < vec.size(); i++){
        iss >> vec.data()[i];
    }
    
    //cout << label << " " << *vec << endl;
    
    return make_pair(label, Item(vec));
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
        //TODO
        emb.insert(parseline(line, dim));
        
        lines++;
    }
    
    return emb;
}

void printUsage(){
    std::cerr << "Usage: ./stuffedturkey mapping.json input_emb.vec output_emb.vec" << std::endl;
}

Item& mean(std::vector<Item> items) {
        if (vecs.size() == 0){
            throw std::runtime_error("cannot calculate mean vector given ");
        }
        
        uint32_t dim = items[0]->size();
        
        
        // input array
        Vector* nv = new Vector(dim);
        
        for (uint32_t ci = 0; ci < dim; ci++) {
            // component
            float tc = 0.0;
            
            for (uint32_t vi = 0; vi < vecs.size(); vi++) {
                //Vector v = *vecs[vi];
                //tc += v[ci];
                tc += (*vecs[vi])[ci];
            }
            
            nv->data_[ci] = tc/vecs.size();
        }
        
        return nv;
    }


int main(int argc, char * argv[]){
    /*if (argc != 4){
        printUsage();
        exit (EXIT_FAILURE);
    }*/
    
    // load original embedding
    std::ifstream ifs(argv[2]);
    Embedding orig_emb = parsevec(ifs);
    
    //orig_emb.dump(std::cout);
    
    // load and iterate mapping
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
            new_emb.insert(make_pair(it.key(), Vector::mean(items)));
        }
        
        break;
    }
    
    /*
    // save embedding
    std::ofstream ofs(argv[3]);
    
    new_emb.dump(ofs);
    
    */
    
    return 0;
}