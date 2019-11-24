#include <cmath>
#include <iomanip>

#include "vector.h"

namespace eagg{

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
    
    Vector* Vector::mean(std::vector<Vector*> vecs) {
        if (vecs.size() == 0){
            throw std::runtime_error("cannot calculate mean vector given ");
        }
        
        uint32_t dim = vecs[0]->size();
        
        
        // input array
        Vector* nv = new Vector(dim);
        
        for (uint32_t ci = 0; ci < dim; ci++) {
            // component
            float tc = 0.0;
            
            for (uint32_t vi = 0; vi < vecs.size(); vi++) {
                //Vector v = *vecs[vi];
                //tc += v[ci];
                tc += (*vecs[vi])[ci];
            }
            
            nv->data_[ci] = tc/vecs.size();
        }
        
        return nv;
    }
    
    std::ostream& operator<<(std::ostream& os, const Vector& v) {
      os << std::setprecision(5);
      for (int64_t j = 0; j < v.size(); j++) {
        os << v[j] << ' ';
      }
      return os;
    }
    
}