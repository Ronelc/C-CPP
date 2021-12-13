#include "vector.h"

/**
 *
 * check the load factor and update the capacity.
 * @param vector a pointer to vector.
 * @param load_factor the load factor.
 * @return 1 if success, 0 otherwise.
 */
int update_capacity (vector *vector, size_t capacity) {
  size_t old_capacity = vector->capacity;
  vector->capacity = capacity;
  void **temp = realloc (vector->data,
                         sizeof (*vector) * capacity);
  if (temp == NULL) {
    vector->capacity = old_capacity;
    return 0;
  }
  vector->data = temp;
  return 1;
}

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector
 * (returns dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the
 * vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *
vector_alloc (vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
              vector_elem_free elem_free_func) {
  if (elem_copy_func == NULL || elem_cmp_func == NULL
      || elem_free_func == NULL) {
    return NULL;
  }
  vector *v = malloc (sizeof (vector));
  if (v == NULL) {
    return NULL;
  }
  v->capacity = VECTOR_INITIAL_CAP;
  v->size = 0;
  v->data = malloc (sizeof (*v) * v->capacity);
  if (v->data == NULL) {
    free (v);
    return NULL;
  }
  v->elem_copy_func = elem_copy_func;
  v->elem_cmp_func = elem_cmp_func;
  v->elem_free_func = elem_free_func;
  return v;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector) {
  if (p_vector == NULL || *p_vector == NULL) {
    return;
  }
  vector **v = (vector **) p_vector;
  vector_clear (*v);
  free ((*v)->data);
  (*v)->data = NULL;
  free ((*v));
  *v = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists
 * (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind) {
  if (vector == NULL || vector->size <= ind) {
    return NULL;
  }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 * ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value) {
  if (vector == NULL || value == NULL) {
    return -1;
  }
  for (size_t j = 0; j < vector->size; j++) {
    if (vector->elem_cmp_func (value, vector_at (vector, j)) == 1) {
      return (int) j;
    }
  }
  return -1;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value) {
  if (vector == NULL || vector_find (vector, value) != -1) {
    return 0;
  }
  vector->size++;
  double load_factor = vector_get_load_factor (vector);
  int check = 1;
  if (load_factor > VECTOR_MAX_LOAD_FACTOR) {
    check = update_capacity (vector, vector->capacity *
                                     VECTOR_GROWTH_FACTOR);
  }
  void *new_elem = vector->elem_copy_func (value);
  if (new_elem == NULL) {
    vector->size--;
    return 0;
  }
  if (check == 1) {
    vector->data[vector->size - 1] = new_elem;
    return 1;
  }
  vector->size--;
  return 0;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector) {
  if (vector == NULL) {
    return -1;
  }
  return (double) vector->size / (double) vector->capacity;
}

/**
 * Removes the element at the given index from the vector.
 * alters the indices of the remaining elements so that there are no empty
 * indices in the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind) {
  if (vector == NULL || vector->size <= ind) {
    return 0;
  }
  vector->size--;
  double load_factor = vector_get_load_factor (vector);
  int check = 1;
  if (load_factor < VECTOR_MIN_LOAD_FACTOR) {
    check = update_capacity (vector, vector->capacity /
                                     VECTOR_GROWTH_FACTOR);
  }
  if (check == 0) {
    vector->size++;
    return 0;
  }
  for (size_t j = ind; j < vector->size + 1; ++j) {
    if (j + 1 == vector->size + 1) {
      vector->elem_free_func (&vector->data[j]);
      vector->data[j] = NULL;
    }
    else {
      vector->elem_free_func (&vector->data[j]);
      void *temp_data = vector->elem_copy_func (vector->data[j + 1]);
      vector->data[j] = temp_data;
    }
  }
  return 1;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear (vector *vector) {
  if (vector == NULL) {
    return;
  }
  for (size_t j = 0; j < vector->size; ++j) {
    vector->elem_free_func (&(vector->data[j]));
    vector->data[j] = NULL;
  }
  vector->size = 0;
}