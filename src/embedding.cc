#include <fstream>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <iostream>

#include "embedding.h"

namespace StuffedTurkey {
    constexpr int32_t FASTTEXT_FILEFORMAT_MAGIC_INT32 = 793712314;

    Item::Item(Vector vector) : v{vector}, c{std::nullopt} {}
    Item::Item(Vector vector, std::int64_t count) : v{vector}, c{count} {}
    
    Item Item::aggregate(std::vector<Item> items, Item func (std::vector<Item>)){
        return func(items);
    }
    
    Embedding::Embedding() {}

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

    bool Embedding::is_unit() {
        std::vector<float> lengths;
        std::map<std::string, Item>::iterator iter = this->begin();
        for (uint8_t i = 0; i < 10 && iter != this->end(); i++, iter++){
            lengths.push_back(iter->second.vector().euclidean_length());
        }

        float sample_sum = std::accumulate(lengths.begin(), lengths.end(), 0.0);
        float mean = sample_sum / lengths.size();
        
        double sq_sum = std::inner_product(lengths.begin(), lengths.end(), lengths.begin(), 0.0);
        double sample_sd = std::accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();

        float test_statistic = mean - 1 / sample_sd / std::sqrt(lengths.size());

        return test_statistic * 2 < 1.96;
    }

    void Embedding::unit(){
        for (auto &item: data_){
            item.second.v.unit();
        }
    }

    void Embedding::loadvec(const std::string& filename){
        std::ifstream ifs(filename);

        if (!ifs.is_open()) {
            throw std::invalid_argument(filename + " cannot be opened for loading!");
        }

        loadvec(ifs);
        ifs.close();
    }

    void Embedding::loadvec(std::istream& in){
        int num, dim;

        in.read((char*) &num, sizeof(int));
        in.read((char*) &dim, sizeof(int));
        
        std::string line;
        
        // skip one empty line
        // TODO: check if you have to skip it
        std::getline(in, line);
        
        while (std::getline(in, line)){
            insert(vec_parseline(line, dim));
        }
    };

    void Embedding::loadbin(const std::string& filename){
        std::ifstream ifs(filename, std::ifstream::binary);

        if (!ifs.is_open()) {
            throw std::invalid_argument(filename + " cannot be opened for loading!");
        }

        loadbin(ifs);
        ifs.close();
    }

    void Embedding::loadbin(std::istream& in){
        struct entry { std::string word; int64_t count; int8_t type; std::vector<int32_t> subwords; };
        int32_t magic;
        in.read((char*)&(magic), sizeof(int32_t));

        if (magic != FASTTEXT_FILEFORMAT_MAGIC_INT32) {
            throw std::runtime_error("not a fasttext model");
        }
        // ignore version
        in.ignore(sizeof(int32_t)); 

        int dim;
        in.read((char*)&(dim), sizeof(int));

        // ignore fasttext arguments:
        //  ws, epoch, minCount, neg, wordNgrams, loss, 
        //  model, bucket, minn, maxn, lrUpdateRate
        in.ignore(11*sizeof(int32_t));
        in.ignore(sizeof(double)); // t

        int32_t size_, nwords_, nlabels_;
        int64_t ntokens_,pruneidx_size_;

        in.read((char*)&size_, sizeof(int32_t));
        in.read((char*)&nwords_, sizeof(int32_t));

        // ignore: nlabels_, ntokens_, pruneidx_size_
        in.ignore(sizeof(int32_t)+sizeof(int64_t)+sizeof(int64_t));
        
        std::vector<entry> words_;
        for (int32_t i = 0; i < size_; i++) {
            char c;
            entry e;
            while ((c = in.get()) != 0) {
                e.word.push_back(c);
            }
            
            in.read((char*)&e.count, sizeof(int64_t));
            in.read((char*)&e.type, sizeof(int8_t));
            
            if (e.type == 0){
                words_.push_back(e);
            }
        }

        bool quant_input;
        in.read((char*)&quant_input, sizeof(bool));

        if (quant_input == true) {
            throw std::runtime_error("quant. models are not supported yet!");
        }

        // ignore m_ 
        in.ignore(sizeof(int64_t));
        
        int64_t dim_;
        in.read((char*)&dim_, sizeof(int64_t));

        assert(dim == dim_);

        for (auto &w : words_){
            auto d_v = std::vector<float>(dim);

            in.read((char*)d_v.data(), sizeof(float) * dim);

            insert(
                w.word,
                Item(Vector(d_v),w.count)
            );
        }
    }

}