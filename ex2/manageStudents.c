#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BEST "best"
#define BUBBLE "bubble"
#define QUICK "quick"
#define ID_ERROR "ERROR: Id must have 10 digits and can not start with 0.\n"
#define AGE_ERROR "ERROR: Age should be an integer between 0 and 100"\
" (includes).\n"
#define GRADE_ERROR "ERROR: Grade should be an integer between 18 "\
"and 120 (includes).\n"
#define NUM_ERROR "ERROR: number of students is invalid\n"
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define MAX_INPUT_LINE 60
#define MAX_ID_DIG 10
#define ZERO_ASCII 48
#define NINE_ASCII 57
#define BASE 10

/**
 * struct of students information.
 */
typedef struct {
    long int id;
    int grade;
    int age;
} Student;

/**
 * function that calculate the average by dividing grade and age.
 * @param grade - the student grade.
 * @param age - the student age.
 * @return - grade / age.
 */
float best_average (float grade, float age) {
  return grade / age;
}

/**
 * function that return rhe best student by average.
 * @param start - the start of arr.
 * @param end - the end of arr.
 */
void best_student (Student *start, Student *end) {
  float max = -1;
  int i_max = 0, i = 0;
  Student *copy = start;
  while (copy < end) {
    float to_check = best_average ((float) start[i].grade,
                                   (float) start[i].age);
    if (to_check > max) {
      max = to_check;
      i_max = i;
    }
    i++;
    copy += sizeof (Student);
  }
  fprintf (stdout, "best student info is: %lu,%d,%d\n", start[i_max].id,
           start[i_max].grade, start[i_max].age);
}

/**
 * convert string to int.
 * @param str - string to convert.
 * @return - int.
 */
long int str_to_int (char *str) {
  char **c_pointer = NULL;
  return strtol (str, c_pointer, BASE);
}

/**
 * check the id.
 * @param id - the student id.
 * @return - 1 if there is a problem, 0 otherwise.
 */
int check_id (char const *id, int size) {
  int count = 0;
  if (id[0] == '0' || (size) != MAX_ID_DIG) {
    fprintf (stdout, "%s", ID_ERROR);
    return 1;
  }
  for (int j = 0; j < size; j++) {
    if ((int) id[j] >= ZERO_ASCII && (int) id[j] <= NINE_ASCII) {
      count++;
    }
    else {
      fprintf (stdout, "%s", ID_ERROR);
      return 1;
    }
  }
  return 0;
}

/**
 * check if the string is correct number.
 * @param num_to_check - string to check.
 * @return - -1 if there is a problem, the num otherwise.
 */
int check_num (const char *num_to_check, char *msg, int size) {
  for (int j = 0; j < size - 1; j++) {
    if ((int) num_to_check[j] < ZERO_ASCII ||
        (int) num_to_check[j] > NINE_ASCII) {
      fprintf (stdout, "%s", msg);
      return -1;
    }
  }
  return 0;
}

/**
 * check the user input.
 * @param input - the user input.
 * @return - 0 if there is a problem, -1 otherwise.
 */
int check_input (char *input, Student *s, int j) {
  int i = 0;
  char *id, *grade, *age;
  char *token = strtok (input, ",");
  while (token != NULL) {
    i++;
    if (i == 1) {
      id = token;
    }
    if (i == 2) {
      grade = token;
    }
    if (i == 3) {
      age = token;
    }
    if (i > 3) {
      fprintf (stdout, "%s", "ERROR: <ID>,<grade>,<age>\n");
      return 1;
    }
    token = strtok (NULL, ",");
  }
  int check_grade = check_num (grade, GRADE_ERROR,
                               (int) strlen (grade) + 1);
  int check_age = check_num (age, AGE_ERROR, (int) strlen (age));
  if (check_id (id, (int) strlen (id)) == 1 || check_grade == -1
      || check_age == -1) {
    return 1;
  }
  else {
    long int id_int = str_to_int (id);
    long int grade_int = str_to_int (grade);
    long int age_int = str_to_int (age);
    if (grade_int >= MIN_GRADE && grade_int <= MAX_GRADE) {
      if (age_int >= MIN_AGE && age_int <= MAX_AGE) {
        s[j].id = id_int;
        s[j].grade = (int) grade_int;
        s[j].age = (int) age_int;
        return -1;
      }
      else {
        fprintf (stdout, "%s", AGE_ERROR);
        return 1;
      }
    }
    else {
      fprintf (stdout, "%s", GRADE_ERROR);
      return 1;
    }
  }
}

/**
 * swap between two pointers.
 * @param first_p - the first pointer.
 * @param second_p - the second pointer.
 */
void swap (Student *first_p, Student *second_p) {
  Student temp = *first_p;
  *first_p = *second_p;
  *second_p = temp;
}

/**
 * return the size of struct.
 * @param start - start of arr.
 * @param end - end of arr.
 * @return - size of the struct.
 */
int size_of_struct (Student *start, Student *end) {
  int n = 0;
  Student *copy_start = start;
  while (copy_start != end) {
    n++;
    copy_start += sizeof (Student);
  }
  return n;
}

/**
 * print the struct values.
 * @param start - start of arr.
 * @param end - end of arr.
 */
void print_struct (Student *start, Student *end) {
  int k = 0;
  Student *copy = start;
  while (copy < end) {
    fprintf (stdout, "%lu,%d,%d\n", start[k].id, start[k].grade, start[k].age);
    k++;
    copy += sizeof (Student);
  }
}

/**
 * function that sort the students by there grades.
 * @param start - start of arr.
 * @param end - end of arr.
 */
void bubble_sort (Student *start, Student *end) {
  int n = size_of_struct (start, end);
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if ((start + j)->grade > (start + j + 1)->grade) {
        swap (start+j, start+ (j + 1));
      }
    }
  }
  print_struct (start, end);
}

/**
 *
 * @param start - start of arr.
 * @param end - end of arr.
 * @return
 */
Student *partition (Student *start, Student *end) {
  Student *left = start, *right = end - 1;
  Student *i = left - 1;
  for (Student *j = left; j <= right; j++) {
    if (j->age < (end - 1)->age) {
      i++;
      swap (i, j);
    }
  }
  swap (i + 1, right);
  return (i + 1);
}

/**
 * function that sort the students by there ages.
 * @param start - start of arr.
 * @param end - end of arr.
 */
void quick_sort (Student *start, Student *end) {
  Student *left = start, *right = end - 1;
  if (left < right) {
    Student *pivot = partition (start, end);
    quick_sort (start, pivot);
    quick_sort (pivot + 1, end);
  }
}

/**
 * get user input.
 * @param command - user command.
 * @return -1 if fail, 0 otherwise.
 */
int get_input (char *command) {
  fprintf (stdout, "Enter number of students. Then enter\n");
  char num_of_students[MAX_INPUT_LINE];
  char *g = fgets (num_of_students, MAX_INPUT_LINE, stdin);
  if (g == NULL) {
    return 1;
  }
  int num_to_check = check_num (num_of_students, "", \
  (int) strlen (num_of_students));
  long int num = str_to_int (num_of_students);
  while (num <= 0 || num_to_check == -1) {
    fprintf (stdout, "%s", NUM_ERROR);
    fprintf (stdout, "Enter number of students. Then enter\n");
    g = fgets (num_of_students, MAX_INPUT_LINE, stdin);
    if (g == NULL) {
      return 1;
    }
    num_to_check = check_num (num_of_students, "",
                              (int) strlen (num_of_students));
    num = str_to_int (num_of_students);
  }
  Student *struct_arr = NULL;
  struct_arr = malloc (sizeof (Student) * num);
  if (struct_arr == NULL) {
    return -1;
  }
  int i = 0;
  while (i < num) {
    fprintf (stdout, "%s", "Enter student info. Then enter\n");
    char id_grade_age[MAX_INPUT_LINE];
    g = fgets (id_grade_age, MAX_INPUT_LINE, stdin);
    if (g == NULL) {
      free (struct_arr);
      struct_arr = NULL;
      return 1;
    }
    if (check_input (id_grade_age, struct_arr, i) != 1) {
      i++;
    }
  }
  if (strcmp (command, BEST) == 0) {
    best_student (struct_arr, struct_arr + sizeof (Student) * num);
  }
  if (strcmp (command, BUBBLE) == 0) {
    bubble_sort (struct_arr, struct_arr + sizeof (Student) * num);
  }
  if (strcmp (command, QUICK) == 0) {
    quick_sort (struct_arr, struct_arr + (num));
    print_struct (struct_arr, struct_arr + sizeof (Student) * num);
  }
  free (struct_arr);
  struct_arr = NULL;
  return 0;
}

/**
 * the main function
 * @param argc - size of arguments.
 * @param argv - the args line.
 * @return successes or fail
 */
int main (int argc, char **argv) {
  char *command = argv[1];
  if (argc == 2
      && (strcmp (command, BEST) == 0 || strcmp (command, BUBBLE) == 0
          || (strcmp (command, QUICK) == 0))) {
    if (get_input (command) != -1) {
      return 0;
    }
    return 1;
  }
  else {
    fprintf (stdout, "%s", "USAGE: command is invalid\n");
    return 1;
  }
}
