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
            explicit Item(Vector);
            explicit Item(Vector, int64_t);
        
            inline Vector vector() const{
                return v;
            }

            inline float& operator[](uint8_t idx) {
              return v[idx];
            }

            inline int size() const{
              return v.size();
            }
        
	          static Item aggregate(std::vector<Item>, Item func (std::vector<Item>)); 
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
      bool empty() const {
        return data_.size() == 0;
      }
      inline int64_t dim() const {
        return dim_;
      }
      inline Item& operator[](std::string idx) {
        return data_[idx];
      }
      inline std::map<std::string, Item>::iterator begin() {
        return data_.begin();
      }
      inline std::map<std::string, Item>::iterator end() {
        return data_.end();
      }
      inline void insert(std::pair<std::string, Item> elem){
        data_.insert(elem);
      }
      inline bool contains(std::string idx){
        return (data_.find(idx) != data_.end());
      }

      void normalize();

      bool is_normalized();

      void dump(std::ostream&) const;

      static Embedding loadvec(std::string);

      class EncounteredNaNError : public std::runtime_error {
       public:
        EncounteredNaNError() : std::runtime_error("Encountered NaN.") {}
      };
    };
}
