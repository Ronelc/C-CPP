#include "hashmap.h"

/**
 * free buckets
 * @param buckets
* @param bucket_size
 */
void free_bucket (vector **buckets, size_t bucket_size) {
  for (size_t i = 0; i < bucket_size; ++i) {
    if (buckets[i] != NULL) {
      vector_free (&buckets[i]);
    }
  }
//  free (*buckets);
  free (buckets);
  buckets = NULL;
}

/**
 * rehash function.
 * @param buckets - old hash map buckets.
 * @param func - hash function.
 * @param old_capacity  - the old capacity.
 * @param capacity - the new capacity.
 * @return the new map if success, NULL otherwise.
 */
int
hash_map_update (hashmap *map, size_t capacity) {
  vector **new_buckets = (vector **) calloc (capacity,
                                             sizeof (vector *));
  for (size_t i = 0; i < map->capacity; ++i) {
    if (map->buckets[i] != NULL) {
      for (size_t j = 0; j < map->buckets[i]->size; ++j) {
        pair *pair_to_insert = map->buckets[i]->data[j];
        if (pair_to_insert == NULL) {
          continue;
        }
        size_t index =
            (size_t) map->hash_func (pair_to_insert->key)
            & (capacity - 1);
        vector *vec = new_buckets[index];
        if (vec == NULL) {
          vec = vector_alloc (pair_copy, pair_cmp, pair_free);
          if (vec == NULL) {
            free_bucket (new_buckets, capacity);
            return 0;
          }
          new_buckets[index] = vec;
        }
        if (vector_push_back (vec, pair_to_insert) == 0) {
          free_bucket (new_buckets, capacity);
          return 0;
        }
      }
    }
  }
  free_bucket (map->buckets, map->capacity);
  map->buckets = new_buckets;
  return 1;
}

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func) {
  hashmap *hash_map = malloc (sizeof (hashmap));
  if (hash_map == NULL) {
    return NULL;
  }
  if (func == NULL) {
    free (hash_map);
    return NULL;
  }
  hash_map->capacity = HASH_MAP_INITIAL_CAP;
  hash_map->size = 0;
  hash_map->hash_func = func;
  hash_map->buckets = (vector **) calloc (hash_map->capacity,
                                          sizeof (vector *));
  if (hash_map->buckets == NULL) {
    free (hash_map);
    return NULL;
  }
  return hash_map;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map) {
  if (p_hash_map == NULL || *p_hash_map == NULL) {
    return;
  }
  for (size_t i = 0; i <  (*p_hash_map)->capacity; ++i) {
    if ((*p_hash_map)->buckets[i] != NULL) {
      vector_free (&((*p_hash_map)->buckets[i]));
    }
  }
  free ((*p_hash_map)->buckets);
  (*p_hash_map)->buckets = NULL;
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair) {
  if (hash_map == NULL || in_pair == NULL || in_pair->value == NULL) {
    return 0;
  }
  if (hashmap_at (hash_map, in_pair->key) != NULL) {
    return 0;
  }
  hash_map->size++;
  if (hashmap_get_load_factor (hash_map) > VECTOR_MAX_LOAD_FACTOR) {
    int rehash = hash_map_update (hash_map,
                                  hash_map->capacity *
                                  VECTOR_GROWTH_FACTOR);
    if (rehash == 1) {
//      hashmap_free (&hash_map);
      hash_map->capacity *= VECTOR_GROWTH_FACTOR;
    }
    else {
      hash_map->size--;
      return 0;
    }
  }
  size_t index =
      (size_t) hash_map->hash_func (in_pair->key) & (hash_map->capacity - 1);
  vector *vec_to_insert = hash_map->buckets[index];
  if (vec_to_insert == NULL) {
    vec_to_insert = vector_alloc (pair_copy, pair_cmp, pair_free);
    if (vec_to_insert == NULL) {
      hash_map->size--;
      return 0;
    }
    hash_map->buckets[index] = vec_to_insert;
  }
  if (vector_push_back (vec_to_insert, in_pair) == 0) {
    hash_map->size--;
    return 0;
  }
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise
 * (the value itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key) {
  if (hash_map == NULL || key == NULL) {
    return NULL;
  }
  valueT val = (valueT) hash_map->hash_func (key);
  size_t index = (size_t) val & (hash_map->capacity - 1);
  if (index >= hash_map->capacity) {
    return NULL;
  }
  vector *vec = hash_map->buckets[index];
  if (vec == NULL) {
    return NULL;
  }
  for (size_t i = 0; i <vec->size; ++i) {
    pair *p = vec->data[i];
    if (p->key_cmp (p->key, key) == 1) {
      return p->value;
    }
  }
  return NULL;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 * (if key not in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key) {
  if (hash_map == NULL || key == NULL) {
    return 0;
  }
  hash_map->size--;
  if (hashmap_get_load_factor (hash_map) < VECTOR_MIN_LOAD_FACTOR) {
    int rehash = hash_map_update (hash_map,
                                  hash_map->capacity /
                                  VECTOR_GROWTH_FACTOR);
    if (rehash == 1) {
      hash_map->capacity /= VECTOR_GROWTH_FACTOR;
    }
    else {
      hash_map->size++;
      return 0;
    }
  }
  size_t index = (size_t) hash_map->hash_func (key) & (hash_map->capacity - 1);
  if (index >= hash_map->capacity) {
    hash_map->size++;
    return 0;
  }
  vector *vector_to_delete = hash_map->buckets[index];
  if (vector_to_delete == NULL) {
    hash_map->size++;
    return 0;
  }
  valueT val = hashmap_at (hash_map, key);
  for (size_t i = 0; i <  vector_to_delete->size; ++i) {
    pair *pair_to_check = vector_to_delete->data[i];
    if (pair_to_check->key_cmp (pair_to_check->key, key) == 1
        && pair_to_check->value_cmp (pair_to_check->value, val) == 1) {
      if (vector_erase (vector_to_delete, i) == 0) {
        hash_map->size++;
        return 0;
      }
      return 1;
    }
  }
  hash_map->size++;
  return 0;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map) {
  if (hash_map == NULL) {
    return -1;
  }
  return (double) hash_map->size / (double) hash_map->capacity;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys, and the seconds apply some modification on the
 * values.
 * The function should apply the modification only on the values that are
 * associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z), and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)},
 * and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1
 * if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int
hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                  valueT_func valT_func) {
  if (hash_map == NULL || valT_func == NULL || keyT_func == NULL) {
    return -1;
  }
  int count = 0;
  for (size_t i = 0; i < hash_map->capacity; ++i) {
    if (hash_map->buckets[i] != NULL) {
      for (size_t j = 0; j <  hash_map->buckets[i]->size; ++j) {
        pair *pair_to_check = hash_map->buckets[i]->data[j];
        if (keyT_func (pair_to_check->key) == 1) {
          valT_func (pair_to_check->value);
          count++;
        }
      }
    }
  }
  return count;
}
