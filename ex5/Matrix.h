// Matrix.h
#include <cmath>
#include <iostream>
#define TO_PRINT 0.1

#ifndef MATRIX_H
#define MATRIX_H

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...
/**
 * class matrix
 */
class Matrix {

 private:
  matrix_dims _matrix_dims{};
  float *_matrix;

 public:

  /**
   * constructor of class matrix
   * @param rows num of rows
   * @param cols num of cols
   */
  Matrix (int rows, int cols);

  /**
   * default constructor creat mat whit 1 row and 1 col
   */
  Matrix() : Matrix(1, 1)
  {}

  /**
   * copy constructor
   * @param m matrix to copy
   */
  Matrix (const Matrix &m);

  /**
   * destructor of class
   */
  ~Matrix ();

  /**
   *
   * @return num of rows of matrix
   */
  int get_rows () const;

  /**
   *
   * @return num of cols of matrix
   */
  int get_cols () const;

  /**
   *
   * @return transpose matrix
   */
  Matrix &transpose ();

  /**
   * change the matrix to: rows = rows * cols, and cols = 1
   * @return the matrix as vector
   */
  Matrix &vectorize ();

  /**
   * Prints matrix elements, no return value.
   * Prints space after each element (include last element in the row)
   * prints newline after each row (include last row)
   */
  void plain_print ();

  /**
   *
   * @param m matrix to multi with;
   * @return dot matrix;
   */
  Matrix dot (const Matrix &m);

  /**
   *
   * @return the matrix norm
   */
  float norm () const;

  /**
   *
   * @param is istream
   * @param m matrix to read to
   */
  friend void read_binary_file (std::istream &is, Matrix &m);



  //Operators
  /**
   * add the given matrix the the obj
   * @param m matrix to add
   * @return the new matrix
   */
  Matrix operator+ (const Matrix &m);

  /**
   * copy the given matrix the the obj
   * @param m matrix to copy
   * @return the new matrix
   */
  Matrix &operator= (const Matrix &m);

  /**
   * Multiplies the 2 matrix according to the rules of the matrix multi
   * @param m matrix to multi
   * @return the new matrix
   */
  Matrix operator* (const Matrix &m) const;

  /**
   * Multiples between the matrix and scalar with the scalar on the left
   * @param s scalar
   * @return the new matrix
   */
  Matrix operator* (float s);

  /**
   * Multiples between the matrix and scalar with the scalar on the right
   * @param s scalar
   * @return the new matrix
   */
  friend Matrix operator* (float const s, Matrix &m) {
    return m * s;
  }

  /**
   * Adds to the correct matrix, the matrix is accepted as a parameter
   * @param m -  matrix to add
   * @return
   */
  Matrix &operator+= (const Matrix &m);

  /**
   *
   * @param i row index
   * @param j col index
   * @return the i,j element in the matrix
   */
  float operator() (int i, int j) const;

  /**
   *
   * @param i row index
   * @param j col index
   * @return reference to the i,j element in the matrix
   */
  float &operator() (int i, int j);

  /**
   *
   * @param index - index to return
   * @return - the index element in the matrix
   */
  float operator[] (int index) const;

  /**
   *
   * @param index - index to return
   * @return - reference to the index element in the matrix
   */
  float &operator[] (int index);

  /**
   * print the matrix.
   * @param os - out stream
   * @return os
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &m);
};

#endif //MATRIX_H
