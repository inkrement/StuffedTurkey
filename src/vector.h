#pragma once

#include <cstdint>
#include <vector>
#include <ostream>
#include <numeric>

namespace StuffedTurkey {
    
class Vector {
 protected:
  
  std::vector<float> data_;

 public:
  Vector();
  explicit Vector(int64_t);
  explicit Vector(std::vector<float>);
  Vector(const Vector&) = default;
  Vector(Vector&&) noexcept = default;
  Vector& operator=(const Vector&) = default;
  Vector& operator=(Vector&&) = default;
    
  inline double dot(Vector const& a, Vector const& b){
    if (a.size() != b.size()) {
        throw std::runtime_error("different sizes");
    }

    return std::inner_product(a.data_.begin(), a.data_.end(), b.data_.begin(), 0.0);
  }

  inline float* data() {
    return data_.data();
  }
  inline const float* data() const {
    return data_.data();
  }
  inline float& operator[](int64_t i) {
    return data_[i];
  }
  inline const float& operator[](int64_t i) const {
    return data_[i];
  }

  inline int64_t size() const {
    return data_.size();
  }
  void zero();
  void unit();
  double euclidean_length() const;
  //static Vector* mean(std::vector<Vector*> vecs);
};
    
std::ostream& operator<<(std::ostream&, const Vector&);
}