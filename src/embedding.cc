#include "embedding.h"

namespace eagg {
    Embedding::Embedding(int64_t dim) : dim_(dim), data_() {}
    
    void Embedding::dump(std::ostream& out) const {
      // write header
      out << data_.size() << " " << dim_ << std::endl;
      
      // write out vectors
        
      for (auto it = data_.begin(); it != data_.end(); ++it){
          //TODO
          
          out << it->first << ' ' << *it->second << std::endl;
          //cout << *it << endl;
      }
      /*
      for (int64_t i = 0; i < m_; i++) {
        for (int64_t j = 0; j < n_; j++) {
          if (j > 0) {
            out << " ";
          }
          out << at(i, j);
        }
        out << std::endl;
      }
      */
        
    };
}