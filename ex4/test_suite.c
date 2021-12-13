#include "test_suite.h"
#include "test_pairs.h"
#include "hash_funcs.h"


/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert (void) {

  //check pairs of {char:int};
  pair *pairs[18];
  for (int j = 0; j < 18; ++j) {
    char key = (char) (j + 48);
    if (key % 2) {
      key += 17;
    }
    int value = j;
    pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  }
  hashmap *map = hashmap_alloc (hash_char);

  //insert pairs to the map;
  for (int k = 0; k < 18; ++k) {
    assert(hashmap_insert (map, pairs[k]) == 1);
  }

  //check if tha pairs is in there right place;
  for (int i = 0; i < 18; ++i) {
    pair *p = pairs[i];
    size_t index = (size_t) hash_char (p->key) & (map->capacity - 1);
    assert(vector_find (map->buckets[index], p) != -1);
  }

  //check double insert;
  for (int k = 0; k < 18; ++k) {
    assert(hashmap_insert (map, pairs[k]) == 0);
  }

  //check map size;
  assert(map->size == 18);

  //free map & pairs;
  for (int i = 0; i < 18; ++i) {
    pair_free ((void **) &pairs[i]);
  }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void) {

  //check pairs of {char:int};
  pair *pairs[18];
  for (int j = 0; j < 18; ++j) {
    char key = (char) (j + 48);
    if (key % 2) {
      key += 17;
    }
    int value = j;
    pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  }
  hashmap *map = hashmap_alloc (hash_char);

  //insert pairs to the map;
  for (int k = 0; k < 18; ++k) {
    if (hashmap_insert (map, pairs[k]) == 0) {
      exit (EXIT_FAILURE);
    }
  }

  //check has_map_at with exist pairs;
  for (int i = 0; i < 18; ++i) {
    assert(
        *(int *) hashmap_at (map, pairs[i]->key) == *(int *) pairs[i]->value);
  }

  //check has_map_at with un exist pairs;
  assert(hashmap_at (map, "1") == NULL);
  assert(hashmap_at (map, "3") == NULL);
  assert(hashmap_at (map, "5") == NULL);
  assert(hashmap_at (map, "7") == NULL);


  //free map & pairs;
  for (int i = 0; i < 18; ++i) {
    pair_free ((void **) &pairs[i]);
  }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void) {

  //check pairs of {char:int};
  pair *pairs[40];
  for (int j = 0; j < 40; ++j) {
    char key = (char) (j + 48);
    if (key % 2) {
      key += 17;
    }
    int value = j;
    pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  }
  hashmap *map = hashmap_alloc (hash_char);

  //insert pairs to the map;
  for (int k = 0; k < 18; ++k) {
    if (hashmap_insert (map, pairs[k]) == 0) {
      exit (EXIT_FAILURE);
    }
  }
  assert(map->size == 18);

  //erase pairs;
  for (int i = 0; i < 18; ++i) {
    hashmap_erase (map, pairs[i]->key);
    pair *p = pairs[i];
    size_t index = hash_char (p->key) & (map->capacity - 1);
    assert(vector_find (map->buckets[index], p) == -1);
  }

//   try to erase un exist pairs;
  for (int i = 18; i < 40; ++i) {
    assert(hashmap_erase (map, pairs[i]->key) == 0);
  }
  assert(map->size == 0);

  //free map & pairs;
  for (int i = 0; i < 40; ++i) {
    pair_free ((void **) &pairs[i]);
  }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap
 * library.
 * If hashmap_get_load_factor fails at some points, the functions exits with
 * exit code 1.
 */
void test_hash_map_get_load_factor (void) {

  //check pairs of {char:int};
  pair *pairs[50];
  for (int j = 0; j < 50; ++j) {
    char key = (char) (j);
    int value = j;
    pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  }
  hashmap *map = hashmap_alloc (hash_char);

  //check initial load factor;
  assert(map->capacity == HASH_MAP_INITIAL_CAP);
  assert(hashmap_get_load_factor (map) == 0);


  //insert pairs to the map;
  for (int k = 0; k < 18; ++k) {
    if (hashmap_insert (map, pairs[k]) == 0) {
      exit (EXIT_FAILURE);
    }
  }

  //check load factor with size = 15;
  assert(map->capacity == 32);
  assert(hashmap_get_load_factor (map) == 0.5625);



  //insert more pairs to the map;
  for (int k = 18; k < 50; ++k) {
    if (hashmap_insert (map, pairs[k]) == 0) {
      exit (EXIT_FAILURE);
    }
  }

  //check load factor with size = 50;
  assert(map->capacity == 128);
  assert(hashmap_get_load_factor (map) == 0.390625);

  //check load factor, after erase with size = 30;
  for (int k = 0; k < 20; ++k) {
    hashmap_erase (map, pairs[k]->key);
  }
  assert(map->capacity == 64);
  assert(hashmap_get_load_factor (map) == 0.46875);

  //check load factor, after erase with size = 0;
  for (int k = 20; k < 50; ++k) {
    hashmap_erase (map, pairs[k]->key);
  }
  assert(map->capacity == 2);
  assert(hashmap_get_load_factor (map) == 0);

  //free map & pairs;
  for (int i = 0; i < 50; ++i) {
    pair_free ((void **) &pairs[i]);
  }
  hashmap_free (&map);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if () {

  //check pairs of {char:int};
  pair *pairs[50];
  for (int j = 0; j < 50; ++j) {
    char key = (char) (j + 48);
    int value = j;
    pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  }
  hashmap *map = hashmap_alloc (hash_char);

  //insert pairs to the map;
  for (int k = 0; k < 50; ++k) {
    if (hashmap_insert (map, pairs[k]) == 0) {
      exit (EXIT_FAILURE);
    }
  }
  assert(hashmap_apply_if (map, is_digit, double_value) == 10);

  //free map & pairs;
  for (int i = 0; i < 50; ++i) {
    pair_free ((void **) &pairs[i]);
  }
  hashmap_free (&map);
}


int main () {
  test_hash_map_insert ();
  test_hash_map_at ();
  test_hash_map_erase ();
  test_hash_map_get_load_factor ();
  test_hash_map_apply_if ();
}