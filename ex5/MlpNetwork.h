//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"
#include "Matrix.h"
#include "Digit.h"

#define MLP_SIZE 4
#define OUTPUT_VEC_SIZE 10

//
const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

// Insert MlpNetwork class here...
class MlpNetwork {

 private:
  const Matrix *_weights, *_biases;

 public:

  /**
 * constrctor of class
 * @param weights
 * @param biases
 */
  MlpNetwork (const Matrix *weights, const Matrix *biases)
      : _weights (weights), _biases (biases) {};

  /**
   * Applies the entire network on input returns digit struct
   * @param img
   * @return
   */
  digit operator() (const Matrix &img);


};
#endif // MLPNETWORK_H
