#include <cmath>
#include <iomanip>

#include "vector.h"

namespace StuffedTurkey {
    Vector::Vector() : data_(std::vector<float>()) {}

    Vector::Vector(int64_t m) : data_(m) {}

    void Vector::zero() {
      std::fill(data_.begin(), data_.end(), 0.0);
    }

    void Vector::norm() {
        double el = this->euclidean_length();

        for (float &c: data_){
          c = c/el;
        }
    }

    double Vector::euclidean_length() const {
      double sum = 0;
      for (auto c: data_) {
        sum += c*c;
      }
      return std::sqrt(sum);
    }
    
    std::ostream& operator<<(std::ostream& os, const Vector& v) {
      os << std::setprecision(5);
      for (int64_t j = 0; j < v.size(); j++) {
        os << v[j] << ' ';
      }
      return os;
    }
    
}