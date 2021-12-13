#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <stdexcept>

using namespace std;
#define STATIC_CAPACITY 16

/**
 *
 * @tparam T type of elements in vec.
 */
template<class T, size_t StaticCapacity = STATIC_CAPACITY>
class vl_vector {

 protected:
  size_t _static_capacity;
  size_t _size;
  size_t _heap_capacity;
  T _static_arr[StaticCapacity];
  T *_heap_arr;
  T *_current_arr;
  bool _is_heap;

  /**
   * check if we have to increase size.
   * @param k - num of new elements.
   * @return true or false.
   */
  bool to_increase (size_t k) {
    if (_is_heap) {
      return (_size + k > _heap_capacity);
    }
    return (_size + k > _static_capacity);
  }

  /**
 *
 * @param size - size of vec
 * @param k - elements to add
 * @return new capacity
 */
  size_t cap_c (size_t size, size_t k) {
    if (size + k <= _static_capacity) {

      return _static_capacity;
    }
    return (int) ((3 * (size + k)) / 2);
  }

  /**
 * copy the elements from stack to heap.
 * @param new_capacity - the new_capacity
 * @return true if success
 */
  bool copy_to_heap (size_t new_capacity) {
    if (_is_heap) {
      T *temp = new (nothrow) T[new_capacity * sizeof (T)];
      if (temp == nullptr) {
        return false;
      }
      for (size_t i = 0; i < this->_size; ++i) {
        temp[i] = _heap_arr[i];
      }
      delete[] _heap_arr;
      _heap_arr = temp;
      _current_arr = _heap_arr;
      _heap_capacity = new_capacity;
      return true;
    }
    else {
      _heap_arr = new (nothrow) T[new_capacity * sizeof (T)];
      if (_heap_arr == nullptr) {
        return false;
      }
      for (size_t i = 0; i < _size; ++i) {
        _heap_arr[i] = _static_arr[i];
      }
      _is_heap = true;
      _heap_capacity = new_capacity;
      _current_arr = _heap_arr;
      return true;
    }
  }

  /**
    * check if we have to decrease size.
    * @param k - num of elements re erase.
    * @return true or false.
    */
  bool to_decrease (size_t k) {
    if (_size - k > _static_capacity) {
      return false;
    }
    else if (_size > _static_capacity && _size - k <= _static_capacity) {
      return true;
    }
    return false;
  }

  /**
   * find the number of element
   * @param position iterator to find
   * @return the number of element
   */
  size_t find_position (const T *position) const {
    size_t location = 0;
    for (auto i = begin (); i != position; ++i) {
      location++;
    }
    return location;
  }

  /**
  * copy the elements from heap to stack.
  */
  void copy_to_stack () {
    for (size_t i = 0; i < _size; ++i) {
      _static_arr[i] = _heap_arr[i];
    }
    _heap_capacity = 0;
    delete[] _heap_arr;
    _current_arr = _static_arr;
    _is_heap = false;
  }

 public:

  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

/**
 * default constructor
 */
  vl_vector () :
      _static_capacity (StaticCapacity),
      _size (0),
      _heap_capacity (0),
      _heap_arr (nullptr),
      _current_arr (_static_arr),
      _is_heap (false) {}

  /**
 * copy constructor
 * @param vec - vector to copy from.
 */
  vl_vector (const vl_vector<T, StaticCapacity> &vec) {
    _static_capacity = vec._static_capacity;
    _size = vec._size;
    _heap_capacity = vec._heap_capacity;
    _is_heap = vec._is_heap;
    T *arr = nullptr;
    if (vec._is_heap) {
      _heap_arr = new (nothrow) T[_size * sizeof (T)];
      _current_arr = _heap_arr;
      arr = _heap_arr;
    }
    else {
      _heap_arr = nullptr;
      _current_arr = _static_arr;
      arr = _static_arr;
    }
    for (size_t i = 0; i < _size; ++i) {
      arr[i] = vec[i];
    }
  }

  /**
 * Sequence based Constructor
 * @tparam InputIterator - iterator to copy from
 * @param first - first elem.
 * @param last - last elem
 */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last)
      :_static_capacity (StaticCapacity) {
    size_t num_of_elements = distance (first, last);
    if (num_of_elements > _static_capacity) {
      size_t new_cap = cap_c (0, num_of_elements);
      _heap_arr = new (nothrow) T[new_cap * sizeof (T)];
      _is_heap = true;
      _heap_capacity = new_cap;
      _current_arr = _heap_arr;
      _size = num_of_elements;
    }
    else {
      _heap_arr = nullptr;
      _current_arr = _static_arr;
      _is_heap = false;
      _size = num_of_elements;
      _heap_capacity = 0;
    }
    for (size_t i = 0; i < num_of_elements; ++i) {
      _current_arr[i] = first[i];
    }
    _size = num_of_elements;
  }

  /**
   * Single-value initialized constructor
   * @param count - num of elements to add.
   * @param v - elem to add.
   */
  vl_vector (size_t count, T v) : vl_vector<T, StaticCapacity> () {
    if (count > _static_capacity) {
      size_t new_cap = cap_c (0, count);
      copy_to_heap (new_cap);
    }
    for (size_t i = 0; i < count; ++i) {
      _current_arr[i] = v;
    }
    _size = count;
  }

  /**
 * destructor of class.
 */
  virtual ~vl_vector () {
    if (_is_heap) {
      delete[] _heap_arr;
    }
  }

  /**
 *
 * @return size of vector.
 */
  virtual size_t size () const {
    return _size;
  }

  /**
 *
 * @return - capacity of vector.
 */
  size_t capacity () const {
    if (_is_heap) {
      return _heap_capacity;
    }
    return _static_capacity;
  }

  /**
 *
 * @return - is vector empty.
 */
  virtual bool empty () const {
    return size () == 0;
  }

  /**
 * @param index - index to return.
 * @return - array index
 */
  T at (size_t index) const {
    if (index < 0 || index >= _size) {
      throw out_of_range ("Error: index out of range");
    }
    else {
      T *current_arr = data ();
      return current_arr[index];
    }
  }

  /**
 *
  * @param index - index to return.
 * @return - array index
 */
  T &at (size_t index) {
    if (index < 0 || index >= _size) {
      throw out_of_range ("Error: index out of range");
    }
    else {
      T *current_arr = data ();
      return current_arr[index];
    }
  }

  /**
 * push back new elem to the end of vec.
 * @param elem - elem to insert
 */
  virtual void push_back (T elem) {
    if (to_increase (1)) {
      size_t new_cap = cap_c (_size, 1);
      copy_to_heap (new_cap);
      _heap_arr[_size++] = elem;
    }
    else {
      _current_arr[_size++] = elem;
    }
  }

  /**
 * insert elem to position.
 * @param position - place to insert the new elem.
 * @param elem - elem to insert.
 * @return  - pointer to the new elem
 */
  iterator insert (const_iterator position, T elem) {
    size_t location = find_position (position);
    if (to_increase (1)) {
      size_t new_cap = cap_c (_size, 1);
      copy_to_heap (new_cap);
    }
    T *arr = data ();
    for (size_t i = (_size + 1); i > location; --i) {
      arr[i] = arr[i - 1];
    }
    arr[location] = elem;
    _size++;
    return iterator (&_current_arr[location]);
  }

  /**
  *
  * @tparam InputIterator.
  * @param position - place to insert the new elements.
  * @param first - first elem to add
  * @param last - last elem to add
  * @return pointer to the first new elem.
  */
  template<class InputIterator>
  const_iterator
  insert (const_iterator position, InputIterator first, InputIterator last) {
    size_t num_of_elements = distance (first, last);
    size_t location = find_position (position);
    if (to_increase (num_of_elements)) {
      size_t new_cap = cap_c (_size, num_of_elements);
      copy_to_heap (new_cap);
    }
    T *arr = data ();
    for (size_t i = _size; i > location; --i) {
      arr[i + num_of_elements - 1] = arr[i - 1];
    }
    auto j = location;
    for (size_t i = 0; i < num_of_elements; ++i) {
      arr[j] = first[i];
      j++;
    }
    _size += num_of_elements;
    return const_iterator (&_current_arr[location]);
  }

  /**
 * delete the last element in vec.
 */
  virtual void pop_back () {
    if (empty ()) {
      return;
    }
    if (to_decrease (1)) {
      _size--;
      copy_to_stack ();
    }
    else {
      _size--;
    }
  }

  /**
 * erase a element from vec
 * @param elem_to_erase - elem to erase
 * @return - pointer to the right of deleted elem.
 */
  virtual iterator erase (const_iterator elem_to_erase) {
    T *arr = data ();
    size_t location = find_position (elem_to_erase);
    for (size_t i = location; i < _size; ++i) {
      arr[i] = arr[i + 1];
    }

    if (to_decrease (1)) {
      _size--;
      copy_to_stack ();
    }
    else { _size--; }
    return iterator (&_current_arr[location]);
  }

  /**
  * erase a elements from vec
  * @param first - first elem to add
  * @param last - last elem to add
  * @return - pointer to the right of deleted elements.
  */
  template<class InputIterator>
  iterator erase (InputIterator first, InputIterator last) {
    size_t num_of_elements = distance (first, last);
    T *arr = data ();
    size_t location = find_position (first);
    size_t i = location;
    while ((i + num_of_elements) < _size || i < location + num_of_elements) {
      arr[i] = arr[i + num_of_elements];
      i++;
    }
    if (to_decrease (num_of_elements)) {
      _size -= num_of_elements;
      copy_to_stack ();
    }
    else { _size -= num_of_elements; }
    return iterator (&_current_arr[location]);
  }

  virtual /**
    * clear vec
    */
  void clear () {
    if (_is_heap) {
      delete[] _heap_arr;
      _is_heap = false;
      _current_arr = _static_arr;
    }
    _size = 0;
  }

  /**
 *
 * @return the current arr
 */
  T *data () const {
    return _current_arr;
  }

  /**
 *
 * @param elem - elem to check
 * @return true if exist
 */
  bool contains (const T elem) const {
    T *arr = _current_arr;
    for (size_t i = 0; i < _size; ++i) {
      if (arr[i] == elem) {
        return true;
      }
    }
    return false;
  }

  /**
*
*/


  //iterator

  /**
   *
   * @return the beginning of vec
   */
  iterator begin () {
    T *arr = data ();
    return arr;
  }

  /**
 *
 * @return the end of vec
 */
  iterator end () {
    T *arr = data ();
    return arr + _size;
  }

  //const_iterator

  /**
 *
 * @return the beginning of vec
 */
  const_iterator begin () const {
    T *arr = data ();
    return arr;
  }

  /**
*
* @return the end of vec
*/
  const_iterator end () const {
    T *arr = data ();
    return arr + _size;
  }

  /**
 *
 * @return the beginning of vec
 */
  const_iterator cbegin () const {
    T *arr = data ();
    return arr;
  }

  /**
*
* @return the end of vec
*/
  const_iterator cend () const {
    T *arr = data ();
    return arr + _size;
  }


  //reverse_iterator

  /**
*
* @return the end of vec
*/
  reverse_iterator rbegin () {
    return reverse_iterator (end ());
  }

  /**
 *
 * @return the beginning of vec
 */
  reverse_iterator rend () {
    return reverse_iterator (begin ());
  }

  /**
*
* @return the end of vec
*/
  const_reverse_iterator rbegin () const {
    return const_reverse_iterator (end ());
  }

  /**
 *
 * @return the beginning of vec
 */
  const_reverse_iterator rend () const {
    return const_reverse_iterator (begin ());
  }

  /**
*
* @return the end of vec
*/
  const_reverse_iterator crbegin () const {
    return const_reverse_iterator (end ());
  }

  /**
 *
 * @return the beginning of vec
 */
  const_reverse_iterator crend () const {
    return const_reverse_iterator (begin ());
  }

  /**
  *
  * @param vec - vec to compare to
  * @return the copy vec
  */
  vl_vector &operator= (const vl_vector<T, StaticCapacity> &vec) {
    if (this == &vec) {
      return *this;
    }
    clear ();
    _static_capacity = vec._static_capacity;
    _heap_capacity = vec._heap_capacity;
    insert (this->begin (), vec.begin (), vec.end ());
    _heap_capacity = vec._heap_capacity;
    _size = vec._size;
    return *this;
  }

  /**
 *
 * @param index index to return
 * @return the index
 */
  T &operator[] (int index) {
    T *arr = data ();
    return arr[index];
  }

  /**
 *
 * @param index index to return
 * @return the index
 */
  T operator[] (int index) const {
    T *arr = data ();
    return arr[index];
  }

  /**
 *
 * @param vec - vec to compare to
 * @return - true if equals
 */
  bool operator== (const vl_vector<T, StaticCapacity> &vec) const {
    if (_size == vec._size) {
      for (size_t i = 0; i < _size; ++i) {
        if (_current_arr[i] != vec._current_arr[i]) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  /**
   *
   * @param vec - vec to compare to
   * @return true f not equals
   */
  bool operator!= (const vl_vector<T, StaticCapacity> &vec) const {
    return !(*this == vec);
  }

};
#endif //_VL_VECTOR_H_
