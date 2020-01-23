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

    Item averageing(std::vector<Item> items, bool weighted = false, bool logged = false) {
        if (items.size() == 0){
            throw std::runtime_error("cannot calculate mean vector given ");
        }
        
        uint32_t dim = items[0].size();
        
        // input array
        Vector nv(dim);
        nv.zero();

        double weight = 1/items.size();
        int sum_counts = 0;

        if (weighted){
            for (Item i: items){
                sum_counts += i.c.value_or(1);
            }
            if (logged) {
                sum_counts = log(sum_counts + items.size());
            }
        }

        for (uint32_t vi = 0; vi < items.size(); vi++) {
            if (weighted){
                if (logged){
                    weight = ((double) items[vi].c.value_or(1) + 1)/sum_counts;
                } else {
                    weight = ((double) items[vi].c.value_or(1))/sum_counts;
                }
            }
            
            for (uint32_t ci = 0; ci < dim; ci++) {
                nv[ci] += items[vi][ci] * weight;
            }
        }
        
        return Item(nv);
    }

    Item Item::avg(std::vector<Item> items) {
        return averageing(items, false, false);
    }
    Item Item::weighted_avg(std::vector<Item> items) {
        return averageing(items, true, false);
    }
    Item Item::log_weighted_avg(std::vector<Item> items) {
        return averageing(items, true, true);
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

    std::pair<std::string, Item> vec_parseline(const std::string &line, const std::int64_t& dim){
        Vector vec(dim);
        
        std::istringstream iss(line);
        
        // load label
        std::string label;
        iss >> label;
        
        // fill vector
        for (int i = 0; i < vec.size(); i++){
            iss >> vec.data()[i];
        }

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
        int num;
        std::string line;

        std::getline(in, line);
        std::stringstream s(line);
        
        s >> num;
        s >> dim_;

        uint32_t i = 0;
        
        while (std::getline(in, line) && i < num){
            // skip first entry if empty line
            if (i == 0 && std::all_of(line.begin(), line.end(), isspace)){
                continue;
            }

            data_.insert(vec_parseline(line, dim_));
            i++;
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

        in.read((char*)&(dim_), sizeof(int));

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

        for (auto &w : words_){
            auto d_v = std::vector<float>(dim_);

            in.read((char*)d_v.data(), sizeof(float) * dim_);

            data_.insert(std::make_pair(w.word, Item(Vector(d_v), w.count)));
        }
    }
}