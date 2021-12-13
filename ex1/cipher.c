#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 *
 */
#define ENCODE  "encode"
#define DECODE "decode"
#define CHECK "check"
#define LENGTH 100000
#define CAPITAL_START 65
#define CAPITAL_END 90
#define SHORT_START 97
#define SHORT_END 122
#define MOD 26
#define ARGC_SIZE_EN 5
#define ARGC_SIZE_C 4
#define POSSIBLE_K 25

/**
 * encode function.
 * @param sourceBuffer - a buffer of the source file.
 * @param output - the output file.
 * @param k - the k parameter.
 * @param charArr - a empty array of chars that we need to manipulate.
 */

void
encode (const char *sourceBuffer, const char *output, int k, char *charArr) {
  for (int i = 0; i <= LENGTH; i++) {
    k = (LENGTH * MOD + k) % MOD;
    int val = (int) sourceBuffer[i];
    if (CAPITAL_START <= val && val <= CAPITAL_END) {
      val = (val + k - CAPITAL_START) % MOD + CAPITAL_START;
      charArr[i] = (char) val;
    }
    if (SHORT_START <= val && val <= SHORT_END) {
      val = (val + k - SHORT_START) % MOD + SHORT_START;
      charArr[i] = (char) val;
    }
    else {
      charArr[i] = (char) val;
    }
  }
  FILE *output_file = fopen (output, "a");
  fputs (charArr, output_file);
  fclose (output_file);
}

/**
 * check function.
 * @param source - the source file.
 * @param result - the result file.
 * @return successes or fail
 */
int check (const char *source, const char *result) {
  FILE *source_file = fopen (source, "r");
  FILE *result_file = fopen (result, "r");
  int result_int = 0, source_int = 0, k = 0;
  int p = fseek (source_file, 0, SEEK_SET);
  while (1) {
    while (source_int != EOF && result_int != EOF) {
      source_int = fgetc (source_file);
      result_int = fgetc (result_file);
      if (CAPITAL_START <= source_int && source_int <= CAPITAL_END) {
        source_int = (source_int + k - CAPITAL_START) % MOD + CAPITAL_START;
      }
      if (SHORT_START <= source_int && source_int <= SHORT_END) {
        source_int = (source_int + k - SHORT_START) % MOD + SHORT_START;
      }
      if (k > POSSIBLE_K) {
        fclose (source_file);
        fclose (result_file);
      }
      if (source_int != result_int) {
        if (source_int == EOF || result_int == EOF
            || (k > 0 && p != fseek (source_file, 0, SEEK_SET))) {
          fclose (source_file);
          fclose (result_file);
          fprintf (stdout, "Invalid encrypting\n");
          return EXIT_SUCCESS;
        }
        else {
          k++;
          fseek (source_file, 0, SEEK_SET);
          fseek (result_file, 0, SEEK_SET);
          continue;
        }
      }
      p++;
    }
    fclose (source_file);
    fclose (result_file);
    fprintf (stdout, "Valid encrypting with k = %d\n", k);
    return EXIT_SUCCESS;
  }
}

/**
 * function that call to other functions - encode, decode or check/
 * @param command - the command.
 * @param source - the source file.
 * @param result - the result file.
 * @param k - the k parameter.
 */
void
encryption (char *command, const char *source, const char *output, int k) {
  char char_arr[LENGTH];
  FILE *source_file = fopen (source, "r");
  char source_buffer[LENGTH];
  if (strcmp (command, CHECK) == 0) {
    check (source, output);
  }
  else {
    FILE *output_file = fopen (output, "w");
    fputs ("", output_file);
    fclose (output_file);
  }
  while (fgets (source_buffer, LENGTH, source_file) != NULL) {
    if (strcmp (command, ENCODE) == 0) {
      encode (source_buffer, output, k, char_arr);
    }
    if (strcmp (command, DECODE) == 0) {
      encode (source_buffer, output, -k, char_arr);
    }
  }
  fclose (source_file);
}

/**
 * check the arguments.
 * @param argc - size of arguments.
 * @param argv - the args line.
 * @return successes or fail
 */
int check_args (int argc, char **argv) {
  char *command, *source, *output;
  int k = 0;
  if (argc > 1) {
    if ((strcmp (argv[1], ENCODE) == 0 || strcmp (argv[1], DECODE) == 0)
        && argc != ARGC_SIZE_EN) {
      fprintf (stderr, "Usage: cipher <encode|decode> <k>"
                       " <source path file> <output path file>\n");
      return EXIT_FAILURE;
    }
    if (strcmp (argv[1], CHECK) == 0 && argc != ARGC_SIZE_C) {
      fprintf (stderr, "Usage: cipher <check> <source path file> "
                       "<output path file>\n");
      return EXIT_FAILURE;
    }
    if ((strcmp (argv[1], ENCODE) != 0 && strcmp (argv[1], DECODE) != 0
         && strcmp (argv[1], CHECK) != 0)) {
      fprintf (stderr, "The given command is invalid\n");
      return EXIT_FAILURE;
    }
    if (argc != ARGC_SIZE_C && argc != ARGC_SIZE_EN) {
      if (strcmp (argv[1], ENCODE) != 0 || strcmp (argv[1], DECODE) != 0) {
        fprintf (stderr, "Usage: cipher <encode|decode> <k>"
                         " <source path file> <output path file>\n");
        return EXIT_FAILURE;
      }
      else {
        fprintf (stderr, "Usage: cipher <check> <source path file> "
                         "<output path file>\n");
        return EXIT_FAILURE;
      }
    }
    if (argc == ARGC_SIZE_C) {
      command = argv[1];
      source = argv[2];
      output = argv[3];
    }
    if (argc == ARGC_SIZE_EN) {
      command = argv[1];
      k = atoi (argv[2]);
      source = argv[3];
      output = argv[4];
    }
  }
  else {
    fprintf (stderr, "The given command is invalid\n");
    return EXIT_FAILURE;
  }
  FILE *source_file = fopen (source, "r");
  if (source_file == NULL) {
    fprintf (stderr, "The given file is invalid\n");
    return EXIT_FAILURE;
  }
  else {
    fclose (source_file);
    encryption (command, source, output, k);
    return EXIT_SUCCESS;
  }
}

/**
 * the main function
 * @param argc - size of arguments.
 * @param argv - the args line.
 * @return successes or fail
 */
int main (int argc, char *argv[]) {
  if (check_args (argc, argv) == 0) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;


}

