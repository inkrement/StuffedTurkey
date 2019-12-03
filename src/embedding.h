#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <assert.h>
#include <map>
#include <optional>

#include "vector.h"

namespace StuffedTurkey {
    class Item {

	public:
            Vector v;
            std::optional<int64_t> c;
            
            // default constructor 
            // required by std::map / [] operator
            Item() = default;
            explicit Item(Vector vector);
            explicit Item(Vector vector, int64_t count);
        
            inline Vector vector() const{
                return v;
            }
        
	    Item aggregate(std::vector<Item>, Item func (std::vector<Item>)); 
    };
    
 
    class Embedding {
     protected:
      std::map<std::string, Item> data_;
      int64_t dim_;

     public:
      explicit Embedding(int64_t);

      inline int64_t len() const {
        return data_.size();
      }
      inline int64_t dim() const {
        return dim_;
      }
      inline Item& operator[](std::string idx) {
        return data_[idx];
      }
      inline void insert(std::pair<std::string, Item> elem){
        data_.insert(elem);
      }
      inline bool contains(std::string idx){
        return (data_.find(idx) != data_.end());
      }
        
      void dump(std::ostream&) const;

      class EncounteredNaNError : public std::runtime_error {
       public:
        EncounteredNaNError() : std::runtime_error("Encountered NaN.") {}
      };
    };
}
