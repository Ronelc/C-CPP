//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#include <cmath>

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    RELU,
    SOFTMAX
};

// Insert Activation class here...
class Activation{

 private:

  ActivationType _activation_type;

  /**
 * if m[i]<0 we cheng it to 0 and else we do nothing
 * @param m matrix
 * @return new matrix
 */
  Matrix relu (Matrix const &m) const;

  /**
 * Returns a matrix according to the formula provided in the exercise
 * @param m matrix
 * @return new matrix
 */
  Matrix softmax (Matrix const &m) const;



 public:

  /**
 * constructor of class
 * @param actType
 */
  Activation (ActivationType act_type){
    _activation_type = act_type;
  }

  /**
 * get type of activationType
 * @return activationType of obj
 */
  ActivationType get_activation_type();

  /**
 * Applies activation function on input
 * @param m matrix
 * @return the act function
 */
  Matrix operator()(const Matrix &m) const ;








};
#endif //ACTIVATION_H
