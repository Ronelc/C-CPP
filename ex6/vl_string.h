

#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include "cstring"
#include "vl_vector.h"

template<size_t StaticCapacity = STATIC_CAPACITY>
class vl_string : public vl_vector<char, StaticCapacity> {

 private:

  /**
   * delete the last char at the string
   */
  void delete_last () {
    this->_current_arr[size () - 1] = this->_current_arr[size ()];
    this->_size--;
  }

 public:
  /**
   * default constructor
   */
  vl_string () : vl_vector<char, StaticCapacity> () {
    this->_size = 1;
    this->_static_arr[0] = (const char) '\0';
  }

  /**
   * copy constructor
   * @param other_str - other_str to copy
   */
  vl_string (const vl_string &other_str)
      : vl_vector<char, StaticCapacity> (other_str) {
  }

  /**
   * implicit constructor
   */
  vl_string (const char *str) :
      vl_vector<char, StaticCapacity> (str, str + strlen (str) + 1) {
  }

  /**
 *
 * @param elem - elem to check
 * @return true if exist
 */
  bool contains (const char *str) const {
    char *arr = this->_current_arr;
    return strstr (arr, str) != nullptr;
  }

  size_t size () const override {
    return this->_size - 1;
  }

  /**
 *
 * @return - is vector empty.
 */
  bool empty () const override {
    return size () == 0;
  }

/**
* push back new elem to the end of vec.
* @param elem - elem to insert
*/
  void push_back (char elem) override {
    if (this->to_increase (1)) {
      size_t new_cap = this->cap_c (this->_size, 1);
      this->copy_to_heap (new_cap);
    }
    this->_current_arr[this->_size - 1] = elem;
    this->_current_arr[this->_size++] = (const char) '\0';
  }

/**
* delete the last element in vec.
*/
  void pop_back () override {
    if (empty ()) {
      return;
    }
    if (this->to_decrease (1)) {
      delete_last ();
      this->copy_to_stack ();
    }
    else {
      delete_last ();
    }
  }

  /**
* erase a element from vec
* @param elem_to_erase - elem to erase
* @return - pointer to the right of deleted elem.
*/
  typename vl_vector<char, StaticCapacity>::iterator erase
      (typename vl_vector<char, StaticCapacity>::
          const_iterator elem_to_erase) override {
    char *arr = this->data ();
    size_t location = this->find_position (elem_to_erase);
    for (size_t i = location; i < this->_size; ++i) {
      arr[i] = arr[i + 1];
    }
    if (this->to_decrease (1)) {
      delete_last ();
      this->copy_to_stack ();
    }
    else {
      delete_last ();
    }
    return typename vl_vector<char, StaticCapacity>::
        iterator (&this->_current_arr[location]);
  }

  /**
  * erase a elements from vec
  * @param first - first elem to add
  * @param last - last elem to add
  * @return - pointer to the right of deleted elements.
  */
  template<class InputIterator>
  typename vl_vector<char, StaticCapacity>::iterator
  erase (InputIterator first, InputIterator last) {
    vl_vector<char, StaticCapacity>::erase (first, last);
  }

  /**
   * clear vec
   */
  void clear () override {
    this->_size = 1;
    if (this->_is_heap) {
      delete[] this->_heap_arr;
      this->_is_heap = false;
      this->_current_arr = this->_static_arr;
//      this->_heap_capacity=0;
    }

    this->_static_arr[0] = (const char) '\0';
  }

  /**
   *
   * @param str - string to add
   * @return new vec
   */
  vl_string &operator+= (const char *str) {
    if (this->to_increase (strlen (str))) {
      size_t new_cap = this->cap_c (this->_size, strlen (str));
      this->copy_to_heap (new_cap);
    }
    char *arr = this->_current_arr;
    int p = this->size ();
    for (size_t i = 0; i < strlen (str) + 1; ++i) {
      arr[p] = str[i];
      ++p;
    }
    this->_size += strlen (str);
    return *this;
  }

  /**
   *
   * @param str - string to add
   * @return new vec
   */
  vl_string &operator+= (const char str) {
    push_back (str);
    return *this;
  }

  /**
   *
   * @param str - string to add
   * @return new vec
   */
  vl_string &operator+= (const vl_string<StaticCapacity> vl_str) {
    if (this->to_increase (vl_str.size ())) {
      size_t new_cap = this->cap_c (this->_size, vl_str.size ());
      this->copy_to_heap (new_cap);
    }
    char *arr = this->_current_arr;
    arr[size () - 1] = arr[size ()];
    this->insert (
        arr + size (), vl_str.data (), vl_str.data () + vl_str._size);
    this->_size--;
    return *this;
  }

  /**
   *
   * @param str - string to add
   * @return new vec
   */
  vl_string operator+ (const char *str) {
    vl_string<> vl (*this);
    if (vl.to_increase (strlen (str))) {
      size_t new_cap = vl.cap_c (vl._size, strlen (str));
      vl.copy_to_heap (new_cap);
    }
    size_t p = vl.size ();
    char *arr = vl._current_arr;
    for (int i = 0; i < strlen (str) + 1; ++i) {
      arr[p] = str[i];
      ++p;
    }
    vl._size += strlen (str);
    return vl;
  }

  /**
   *
   * @param str - string to add
   * @return new vec
   */
  vl_string operator+ (const char str) {
    vl_string<> vl (*this);
    vl.push_back (str);
    return vl;
  }

  /**
   *
   * @param vl_str - vector to add
   * @return new vec
   */
  vl_string operator+ (const vl_string<StaticCapacity> vl_str) {
    vl_string<> vl (*this);
    char *arr = vl._current_arr;
    arr[size () - 1] = arr[size ()];
    vl.insert (arr + size (), vl_str.data (),
                  vl_str.data () + vl_str._size);
    vl._size--;
    return vl;
  }

  /**
   * cast vl_string to (const char *)
   * @return (const char *)
   */
  operator const char * () const {
    return (const char *) this->_current_arr;
  }
};

#endif //_VL_STRING_H_
