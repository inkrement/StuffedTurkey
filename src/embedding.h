#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include <assert.h>
#include <map>
#include "vector.h"

namespace eagg {
    class Embedding {
     protected:
      std::map<std::string, Vector*> data_;
      int64_t dim_;

     public:
      explicit Embedding(int64_t);

      inline int64_t len() const {
        return data_.size();
      }
      inline int64_t dim() const {
        return dim_;
      }
      inline Vector* operator[](std::string idx) {
        return data_[idx];
      }
      inline void insert(std::pair<std::string, Vector*> elem){
        data_.insert(elem);
      }
      inline bool contains(std::string idx){
        return (data_.find(idx) !=  data_.end());
      }
        
      void dump(std::ostream&) const;

      class EncounteredNaNError : public std::runtime_error {
       public:
        EncounteredNaNError() : std::runtime_error("Encountered NaN.") {}
      };
    };
}