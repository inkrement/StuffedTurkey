#include <cmath>
#include <iomanip>

#include "vector.h"

namespace StuffedTurkey {
    Vector::Vector() : data_(std::vector<float>()) {}

    Vector::Vector(int64_t m) : data_(m) {}

    void Vector::zero() {
      std::fill(data_.begin(), data_.end(), 0.0);
    }

    float Vector::norm() const {
      float sum = 0;
      for (int64_t i = 0; i < size(); i++) {
        sum += data_[i] * data_[i];
      }
      return std::sqrt(sum);
    }
    
    //Vector* Vector::aggregate(std::vector<Vector*> vecs, function callback_func){
    //    
    //}
    
    
    std::ostream& operator<<(std::ostream& os, const Vector& v) {
      os << std::setprecision(5);
      for (int64_t j = 0; j < v.size(); j++) {
        os << v[j] << ' ';
      }
      return os;
    }
    
}