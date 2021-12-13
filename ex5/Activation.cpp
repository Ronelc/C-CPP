#include "Activation.h"


/**
* get type of activationType
* @return activationType of obj
*/
ActivationType Activation::get_activation_type () {
  return _activation_type;
}

/**
* if m[i]<0 we cheng it to 0 and else we do nothing
* @param m matrix
* @return new matrix
*/
Matrix Activation::relu (Matrix const &m) const {
  Matrix copy_vec = m;
  for (int i = 0; i < m.get_cols () * m.get_rows (); ++i) {
    if (copy_vec[i] < 0) {
      copy_vec[i] = 0;
    }
  }
  return copy_vec;
}

/**
* Returns a matrix according to the formula provided in the exercise
* @param m matrix
* @return new matrix
*/
Matrix Activation::softmax (Matrix const &m) const {
  float sum = 0;
  Matrix copy_vec = m;
  for (int i = 0; i < m.get_cols () * m.get_rows (); ++i) {
    copy_vec[i] = std::exp (m[i]);
    sum +=  copy_vec[i];
  }
  float scalar = (1 / sum);
  return scalar * copy_vec;
}

/**
* Applies activation function on input
* @param m matrix
* @return the activation function
*/
Matrix Activation::operator() (const Matrix &m) const {
  if (_activation_type == RELU) {
    return relu (m);
  }
  return softmax (m);
}


