#include "embedding.h"

namespace StuffedTurkey {
    Item::Item(Vector vector) : v{vector}, c{std::nullopt} {}
    Item::Item(Vector vector, int64_t count) : v{vector}, c{count} {}
    
    Item Item::aggregate(std::vector<Item> items, Item func (std::vector<Item>)){
        return func(items);
    }
    
    Embedding::Embedding(int64_t dim) : dim_(dim), data_() {}
    
    void Embedding::dump(std::ostream& out) const {
      // write header
      out << data_.size() << " " << dim_ << std::endl;
      
      // write out vectors
      for (auto it = data_.begin(); it != data_.end(); ++it){
          out << it->first << ' ' << it->second.vector() << std::endl;
      } 
    };
}
