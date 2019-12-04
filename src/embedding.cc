#include <fstream>
#include <sstream>

#include "embedding.h"

namespace StuffedTurkey {
    Item::Item(Vector vector) : v{vector}, c{std::nullopt} {}
    Item::Item(Vector vector, std::int64_t count) : v{vector}, c{count} {}
    
    Item Item::aggregate(std::vector<Item> items, Item func (std::vector<Item>)){
        return func(items);
    }
    
    Embedding::Embedding(std::int64_t dim) : dim_(dim), data_() {}
    
    void Embedding::dump(std::ostream& out) const {
      // write header
      out << data_.size() << " " << dim_ << std::endl;
      
      // write out vectors
      for (auto it = data_.begin(); it != data_.end(); ++it){
          out << it->first << ' ' << it->second.vector() << std::endl;
      } 
    }

    std::pair<std::string, Item> vec_parseline(std::string line, std::int64_t dim){
        Vector vec(dim);
        
        std::istringstream iss(line);
        
        // load label
        std::string label;
        iss >> label;
        
        // fill vector
        for (int i = 0; i < vec.size(); i++){
            iss >> vec.data()[i];
        }
        
        //cout << label << " " << *vec << endl;
        
        return make_pair(label, Item(vec));
    }

    Embedding Embedding::loadvec(std::string filename){
        std::ifstream ifs(filename);

        if (!ifs.is_open()) {
            throw std::invalid_argument(filename + " cannot be opened for loading!");
        }

        int num;
        int dim;
        
        ifs >> num;
        ifs >> dim;
        
        Embedding emb(dim);
        std::string line;
        
        // skip one empty line
        std::getline(ifs, line);
        
        int32_t lines = 0;
        
        while (std::getline(ifs, line)){
            //TODO
            emb.insert(vec_parseline(line, dim));
            
            lines++;
        }

        ifs.close();

        return emb;
    };

}