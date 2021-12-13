#include "Matrix.h"

/**
 * constructor of class matrix
 * @param rows num of rows
 * @param cols num of cols
 */
Matrix::Matrix (int rows, int cols) : _matrix_dims{rows, cols} {
  if (rows <= 0 || cols <= 0) {
    std::cerr << "Error: the cols and rows must be a positive number"
              << std::endl;
    exit (EXIT_FAILURE);
  }
  _matrix = new (std::nothrow) float[_matrix_dims.cols * _matrix_dims.rows]{0};
  if (_matrix == nullptr) {
    std::cerr << "Error: allocation failed" << std::endl;
    exit (EXIT_FAILURE);
  }
}

/**
 * copy constructor
 * @param m matrix to copy
 */
Matrix::Matrix (const Matrix &m) {
  if (this == &m) {
    return;
  }
  this->_matrix_dims.rows = m.get_rows ();
  this->_matrix_dims.cols = m.get_cols ();
  this->_matrix = new (std::nothrow) float[_matrix_dims.cols
                                           * _matrix_dims.rows]{0};
  if (_matrix == nullptr) {
    std::cerr << "Error: allocation failed" << std::endl;
    exit (EXIT_FAILURE);
  }
  int end = m.get_rows () * m.get_cols ();
  for (int i = 0; i < end; ++i) {
    (*this)[i] = m[i];
  }
}

/**
 * destructor of class
 */
Matrix::~Matrix () {
  delete[] _matrix;
}

/**
 *
 * @return num of rows of matrix
 */
int Matrix::get_rows () const {
  return _matrix_dims.rows;
}

/**
 *
 * @return num of cols of matrix
 */
int Matrix::get_cols () const {
  return _matrix_dims.cols;
}

/**
 *
 * @return transpose matrix
 */
Matrix &Matrix::transpose () {
  Matrix new_matrix = Matrix (_matrix_dims.cols, _matrix_dims.rows);
  for (int r = 0; r < _matrix_dims.rows; ++r) {
    for (int c = 0; c < _matrix_dims.cols; ++c) {
      new_matrix[c * _matrix_dims.rows + r] =
          _matrix[r * _matrix_dims.cols + c];
    }
  }
  *this = new_matrix;
  return (*this);
}

/**
 * change the matrix to: rows = rows * cols, and cols = 1
 * @return the matrix as vector
 */
Matrix &Matrix::vectorize () {
  _matrix_dims.rows = _matrix_dims.cols * _matrix_dims.rows;
  _matrix_dims.cols = 1;
  return *this;
}

/**
 * Prints matrix elements, no return value.
 * Prints space after each element (include last element in the row)
 * prints newline after each row (include last row)
 */
void Matrix::plain_print () {
  for (int r = 0; r < _matrix_dims.rows; ++r) {
    for (int c = 0; c < _matrix_dims.cols; ++c) {
      std::cout << _matrix[r * _matrix_dims.cols + c] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

/**
 *
 * @param m matrix to multi with;
 * @return dot matrix;
 */
Matrix Matrix::dot (const Matrix &m) {
  if (m.get_cols () != _matrix_dims.cols
      || m.get_rows () != _matrix_dims.rows) {
    std::cerr << "Error: cols and rows of the new matrix must be equal to"
                 " the old one" << std::endl;
    exit (EXIT_FAILURE);
  }
  Matrix to_return (*this);
  for (int i = 0; i < this->get_rows () * this->get_cols (); ++i) {
    to_return[i] = _matrix[i] * m[i];
  }
  return to_return;
}

/**
 *
 * @return the matrix norm
 */
float Matrix::norm () const {
  float sum = 0;
  for (int i = 0; i < _matrix_dims.rows * _matrix_dims.cols; ++i) {
    sum += powf (_matrix[i], 2);
  }
  return sqrtf (sum);
}

/**
 *
 * @param is istream
 * @param m matrix to read to
 */

void read_binary_file (std::istream &is, Matrix &m) {
  int i = 0;
  for (; i < m.get_cols () * m.get_rows (); ++i) {
    is.read ((char *) &m[i], sizeof (float));
    if (!is.good ()) {
      std::cerr << "Error: cant read the file" << std::endl;
      exit (EXIT_FAILURE);
    }
  }
  if (i < m.get_cols () * m.get_rows () || is.bad ()) {
    std::cerr << "Error: cant read the file" << std::endl;
    exit (EXIT_FAILURE);
  }
  int read = is.peek ();
  if (read != EOF) {
    if (!is.eof ()) {
      std::cerr << "Error: cant read the file" << std::endl;
      exit (EXIT_FAILURE);
    }
  }
//  return is;
}

//Operators

/**
 * add the given matrix the the obj
 * @param m matrix to add
 * @return the new matrix
 */
Matrix Matrix::operator+ (const Matrix &m) {
  if (m.get_cols () != _matrix_dims.cols
      || m.get_rows () != _matrix_dims.rows) {
    std::cerr << "Error: cols and rows of the new matrix must be equal to"
                 " the old one" << std::endl;
    exit (EXIT_FAILURE);
  }
  Matrix to_return (*this);
  for (int i = 0; i < this->get_rows () * this->get_cols (); ++i) {
    to_return[i] = _matrix[i] + m[i];
  }
  return to_return;
}

/**
 * copy the given matrix the the obj
 * @param m matrix to copy
 * @return the new matrix
 */
Matrix &Matrix::operator= (const Matrix &m) {
  if (this == &m) {
    return *this;
  }
  delete[] this->_matrix;
  _matrix_dims.rows = m.get_rows ();
  _matrix_dims.cols = m.get_cols ();
  _matrix = new (std::nothrow) float[m.get_rows () * m.get_cols ()]{0.0};
  if (_matrix == nullptr) {
    std::cerr << "Error: allocation failed" << std::endl;
    exit (EXIT_FAILURE);
  }
  for (int i = 0; i < m.get_rows () * m.get_cols (); ++i) {
    (*this)[i] = m[i];
  }
  return *this;
}

/**
 * Multiplies the 2 matrix according to the rules of the matrix multi
 * @param m matrix to multi
 * @return the new matrix
 */
Matrix Matrix::operator* (const Matrix &m) const {
  if (_matrix_dims.cols != m.get_rows ()) {
    std::cerr << "Error: rows of the new matrix must be equal to"
                 " the cols of the old one" << std::endl;
    exit (EXIT_FAILURE);
  }
  Matrix new_matrix = Matrix (_matrix_dims.rows, m.get_cols ());
  for (int r = 0; r < _matrix_dims.rows; ++r) {
    for (int c = 0; c < m.get_cols (); ++c) {
      for (int k = 0; k < _matrix_dims.cols; ++k) {
        float a = (*this) (r, k);
        float b = m (k, c);
        new_matrix (r, c) += (a * b);
      }
    }
  }
  return new_matrix;
}

/**
 * Multiples between the matrix and scalar with the scalar on the left
 * @param s scalar
 * @return the new matrix
 */
Matrix Matrix::operator* (float s) {
  Matrix to_return (*this);
  for (int i = 0; i < this->get_rows () * this->get_cols (); ++i) {
    to_return[i] = _matrix[i] * s;
  }
  return to_return;
}

/**
 * Adds to the correct matrix, the matrix is accepted as a parameter
 * @param m -  matrix to add
 * @return
 */
Matrix &Matrix::operator+= (const Matrix &m) {
  if (_matrix_dims.rows != m.get_rows ()
      || _matrix_dims.cols != m.get_cols ()) {
    std::cerr << "Error: cols and rows of the new matrix must be equal to"
                 " the old one" << std::endl;
    exit (EXIT_FAILURE);
  }
  for (int i = 0; i < _matrix_dims.rows * _matrix_dims.cols; ++i) {
    (*this)[i] += m[i];
  }
  return *this;
}

/**
 *
 * @param i row index
 * @param j col index
 * @return the i,j element in the matrix
 */
float Matrix::operator() (int i, int j) const {
  if (i >= _matrix_dims.rows || j >= _matrix_dims.cols || i < 0 || j < 0) {
    std::cerr << "Error: index out of range" << std::endl;
    exit (EXIT_FAILURE);
  }
  return _matrix[i * _matrix_dims.cols + j];
}

/**
 *
 * @param i row index
 * @param j col index
 * @return reference to the i,j element in the matrix
 */
float &Matrix::operator() (int i, int j) {
  if (i >= _matrix_dims.rows || j >= _matrix_dims.cols || i < 0 || j < 0) {
    std::cerr << "Error: index out of range" << std::endl;
    exit (EXIT_FAILURE);
  }
  return _matrix[i * _matrix_dims.cols + j];
}

/**
 *
 * @param index - index to return
 * @return - the index element in the matrix
 */
float Matrix::operator[] (int index) const {
  return _matrix[index];
}

/**
 *
 * @param index - index to return
 * @return - reference to the index element in the matrix
 */
float &Matrix::operator[] (int index) {
  return _matrix[index];
}

/**
 * print the matrix.
 * @param os - out stream
 * @return os
 */
std::ostream &operator<< (std::ostream &os, const Matrix &m) {
  for (int r = 0; r < m.get_rows (); ++r) {
    for (int c = 0; c < m.get_cols (); ++c) {
      if (m (r, c) >= TO_PRINT) {
        os << "  ";
      }
      else {
        os << "**";
      }
    }
    os << std::endl;
  }
  return os;
}
