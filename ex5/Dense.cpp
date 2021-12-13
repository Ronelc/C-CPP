#include "Dense.h"


/**
 *
 * @param m - matrix
 * @return Applies the layer on input and returns output matrix Layers operate
 */
Matrix Dense::operator() (Matrix const &m){
  return _activation((_w * m) + _bias);
}

