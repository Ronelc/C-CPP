#include "MlpNetwork.h"

/**
* Applies the entire network on input returns digit struct
* @param img
* @return
*/
digit MlpNetwork::operator() (const Matrix &img) {
  ActivationType act[MLP_SIZE] = {RELU, RELU, RELU, SOFTMAX};
  Matrix new_matrix = img;
  for (int i = 0; i < MLP_SIZE; ++i) {
    Dense dense (_weights[i], _biases[i], act[i]);
    new_matrix = dense (new_matrix);
  }
  unsigned int index = 0;
  float max_index = new_matrix[(int) index];
  for (int j = 0; j < OUTPUT_VEC_SIZE; j++) {
    if (max_index < new_matrix[j]) {
      index = j;
      max_index = new_matrix[j];
    }
  }
  digit digit = {index, max_index};
  return digit;
}

