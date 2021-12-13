#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define BASE 10
#define MAX_ARGS 5
#define MIN_ARGS 4

/**
 * struct that contain a pointer to word
 * dynamic array
 */
typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    int num_of_word_in_txt;
    int size_of_prob;
} WordStruct;

/**
 *
 * @param first_word
 * @param second_word
 * @return
 */
float probability (WordStruct *first_word, int num_of_next_word) {
  float second_word_after_first_word = (float) num_of_next_word;
  float first_word_txt = (float) first_word->num_of_word_in_txt;
  return second_word_after_first_word / first_word_txt;
}

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    float probability;
    int num_of_next_word;
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

int check_args (int argc, char **argv);
Node *is_node_exist (LinkList *dictionary, char *word);

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data) {
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL) {
    return 1;
  }
  *new_node = (Node) {data, NULL};
  if (link_list->first == NULL) {
    link_list->first = new_node;
    link_list->last = new_node;
  }
  else {
    link_list->last->next = new_node;
    link_list->last = new_node;
  }

  link_list->size++;
  return 0;
}
/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number) {
  int num = rand () % max_number;
  return num;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary) {
  int max_number = dictionary->size;
  int rand = get_random_number (max_number);
  int i = 0;
  Node *node = dictionary->first;
  while (i < rand) {
    node = node->next;
    i++;
  }
  int len = (int) strlen (node->data->word);
  if (node->data->word[len - 1] == '.') {
    return get_first_random_word (dictionary);
  }
  return node->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr) {
  int arr_size = word_struct_ptr->num_of_word_in_txt;
  WordStruct **arr = malloc (arr_size * sizeof (WordStruct));
  if (arr == NULL) {
    exit (EXIT_FAILURE);
  }
  int i = 0;
  while (i < arr_size) {
    for (int j = 0; j < word_struct_ptr->size_of_prob; j++) {
      int num = word_struct_ptr->prob_list[j].num_of_next_word;
      for (int k = 0; k < num; k++) {
        arr[i] = word_struct_ptr->prob_list[j].word_struct_ptr;
        i++;
      }
    }
  }
  int rand = get_random_number (arr_size);
  WordStruct *to_return = (WordStruct *) arr[rand];
  free (arr);
  return to_return;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary) {
  WordStruct *first_word = get_first_random_word (dictionary);
  WordStruct sentence_arr[MAX_WORDS_IN_SENTENCE_GENERATION] = {NULL};
  sentence_arr[0] = *first_word;
  int flag = 1, num_of_words = 1, to_stop = 0;
  while (num_of_words < MAX_WORDS_IN_SENTENCE_GENERATION && flag != 0) {
    WordStruct *next_word = get_next_random_word (first_word);
    int len = (int) strlen (next_word->word);
    if (next_word->word[len - 1] == '.') {
      flag = 0;
      to_stop = num_of_words + 1;
    }
    sentence_arr[num_of_words] = *next_word;
    num_of_words++;
    first_word = next_word;
  }
  if (flag == 1) {
    to_stop = MAX_WORDS_IN_SENTENCE_GENERATION;
  }
  for (int i = 0; i < to_stop; ++i) {
    if (i + 1 == to_stop) {
      fprintf (stdout, "%s\n", sentence_arr[i].word);
    }
    else fprintf (stdout, "%s ", sentence_arr[i].word);
  }

  return num_of_words;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word) {
  if (first_word->prob_list == 0) {
    first_word->prob_list = malloc (sizeof (WordProbability));
    if (first_word->prob_list == NULL) {
      exit (EXIT_FAILURE);
    }
    first_word->size_of_prob = 1;
    first_word->prob_list->word_struct_ptr = second_word;
    first_word->prob_list->num_of_next_word = 1;
    first_word->prob_list->probability = probability (first_word, 1);
    return 1;
  }
  else {
    for (int i = 0; i < first_word->size_of_prob; ++i) {
      if (strcmp (first_word->prob_list[i].word_struct_ptr->word,
                  second_word->word) == 0) {
        first_word->prob_list[i].num_of_next_word++;
        first_word->prob_list[i].probability =
            probability (first_word,
                         first_word->prob_list[i].num_of_next_word);
        return 0;
      }
    }
    void *new_ptr = realloc (first_word->prob_list,
                             (first_word->size_of_prob + 1)
                             * sizeof (WordProbability));
    if (new_ptr == NULL) {
      exit (EXIT_FAILURE);
    }
    first_word->prob_list = new_ptr;
    first_word->prob_list[first_word->size_of_prob].word_struct_ptr =
        second_word;
    first_word->prob_list[first_word->size_of_prob].num_of_next_word = 1;
    first_word->prob_list[first_word->size_of_prob].probability =
        probability (first_word, 1);
    first_word->size_of_prob++;
    return 1;
  }
}

/**
 *
 * @param dictionary Empty dictionary to fill
 * @param word word to add
 * @param new_word word struct.
 * @param node_to_check node to check if is exist.
 * @return word struct.
 */
WordStruct *
fill_dictionary_helper (LinkList *dictionary, char *word,
                        WordStruct *new_word, Node *node_to_check) {
  if (node_to_check == NULL) {
    new_word = malloc (sizeof (WordStruct));
    if (new_word == NULL) {
      exit (EXIT_FAILURE);
    }
    new_word->word = malloc ((strlen (word) + 1) * sizeof (char));
    if (new_word->word == NULL) {
      exit (EXIT_FAILURE);
    }
    new_word->prob_list = NULL;
    new_word->size_of_prob = 0;
    strcpy (new_word->word, word);
    new_word->num_of_word_in_txt = 1;
    if (add (dictionary, new_word) != 0) {
      exit (EXIT_FAILURE);
    }
  }
  else {
    node_to_check->data->num_of_word_in_txt += 1;
  }
  return new_word;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary) {
  char sentence[MAX_SENTENCE_LENGTH];
  WordStruct *new_word = NULL, *first_word = NULL, *second_word = NULL;
  int i = 0;
  if (words_to_read == -1) {
    i = -2;
  }
  while (fgets (sentence, MAX_SENTENCE_LENGTH, fp) != NULL) {
    sscanf (sentence, "%[^\r\n]\r\n", sentence);
    char *word = strtok (sentence, " ");
    int flag = 0;
    while (word != NULL && i < words_to_read) {
      if (words_to_read != -1) {
        i++;
      }
      Node *node_to_check = is_node_exist (dictionary, word);
      new_word = fill_dictionary_helper
          (dictionary, word, new_word, node_to_check);
      if (flag == 1) {
        if (node_to_check == NULL) {
          second_word = new_word;
        }
        else {
          second_word = node_to_check->data;
        }
        add_word_to_probability_list (first_word, second_word);
        first_word = second_word;
      }
      else {
        if (node_to_check == NULL) {
          first_word = new_word;
        }
        else {
          first_word = node_to_check->data;
        }
        flag = 1;
      }
      word = strtok (NULL, " ");
    }
  }
}

/**
 * check if the world is already in dictionary.
 * @param dictionary - Linked list
 * @param word word to check.
 * @return thr node if exist, NULL else.
 */
Node *is_node_exist (LinkList *dictionary, char *word) {
  Node *node = dictionary->first;
  if (node == NULL) {
    return NULL;
  }
  while (node != dictionary->last) {
    if (strcmp (node->data->word, word) == 0) {
      return node;
    }
    node = node->next;
  }
  if (strcmp (node->data->word, word) == 0) {
    return node;
  }
  return NULL;
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary) {
  Node *node = dictionary->first, *prev_node = NULL;
  while (node != dictionary->last) {
    free (node->data->prob_list);
    free (node->data->word);
    free (node->data);
    prev_node = node;
    node = node->next;
    free (prev_node);
    prev_node = NULL;
  }
  free (node->data->prob_list);
  free (node->data->word);
  free (node->data);
  free (node);
  free (dictionary);
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int main (int argc, char *argv[]) {
  return check_args (argc, argv);
}

/**
 * convert string to int.
 * @param str - string to convert.
 * @return - int.
 */
int str_to_int (char *str) {
  char **c_pointer = NULL;
  return (int) strtol (str, c_pointer, BASE);
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
int check_args (int argc, char *argv[]) {
  int num_of_tweets = 0, words_to_read = -1, seed = 0;
  char *path;
  if (argc == MIN_ARGS || argc == MAX_ARGS) {
    seed = str_to_int (argv[1]);
    srand ((unsigned int) seed);
    num_of_tweets = str_to_int (argv[2]);
    path = argv[3];
    if (argc == MAX_ARGS) {
      words_to_read = str_to_int (argv[4]);
    }
    FILE *source_file = NULL;
    source_file = fopen (path, "r");
    if (source_file == NULL) {
      fprintf (stdout, "Error: The given file is invalid\n");
      return EXIT_FAILURE;
    }
    else {
      LinkList *dictionary = NULL;
      dictionary = malloc (sizeof (LinkList));
      if (dictionary == NULL) {
        return EXIT_FAILURE;
      }
      dictionary->first = NULL;
      dictionary->last = NULL;
      dictionary->size = 0;
      fill_dictionary (source_file, words_to_read, dictionary);
      int num = 0;
      while (num < num_of_tweets) {
        fprintf (stdout, "%s %d: ", "Tweet", num + 1);
        generate_sentence (dictionary);
        num++;
      }
      fclose (source_file);
      free_dictionary (dictionary);
      return EXIT_SUCCESS;
    }
  }
  fprintf (stdout, "Usage: <seed>,<Number of sentences to generate>,"
                   "<Path to file>,<Optional - Number of words to read>\n");
  return EXIT_FAILURE;
}