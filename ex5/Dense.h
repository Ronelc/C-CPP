#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"
#include "Matrix.h"

// implement class Dense here...

/**
 * The class represents a layer ,and will be used to
 * define and run the various layer operations on the network.
 */
class Dense {
 private:
  const Matrix _w;
  const Matrix _bias;
  Activation _activation;

 public:

  /**
 * constructor of class
 * @param w - matrix
 * @param bias - vector
 * @param activationType - the type of activation function.
 */
  Dense (const Matrix &w, const Matrix &bias, ActivationType activationType) :
      _w (w), _bias (bias), _activation (activationType) {
  }

  /**
 *
 * @return Returns the weights of this layer forbids modification
 */
  const Matrix &get_weights () {
    return _w;
  }

  /**
 *
 * @return Returns the bias of this layer forbids modification
 */
  const Matrix &get_bias () {
    return _bias;
  }

  /**
 *
 * @return activation func of obj
 */
  Activation get_activation () {
    return _activation;
  }

  /**
   *
   * @param m - matrix.
   * @return Applies the layer on input and returns output matrix Layers
   * operate
   */
  Matrix operator() (Matrix const &m);

};

#endif //C___PROJECT_DENSE_H
