#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "statemodel.h"
#include "parser.h"

#include "stringmodel.h"
#include "intmodel.h"
#include "valmodel.h"
#include "objmodel.h"

// Used to specify which of the four FSMs to use
typedef enum
{
  STR,
  INT,
  VAL,
  OBJ
} fsm_type_t;

static bool get_args(int, char **, fsm_type_t *, char **);
static void usage(void);

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    usage();
    return EXIT_FAILURE;
  }

  fsm_type_t type = INT;
  char *filename = NULL;
  if (!get_args(argc, argv, &type, &filename))
  {
    usage();
    return EXIT_FAILURE;
  }

  // open and read the file
  char *result;
  bool success = false;
  char *inputbuffer = 0;
  long length;
  FILE *f = fopen(filename, "r");

  if (f)
  {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    inputbuffer = malloc(length);
    if (inputbuffer)
    {
      fread(inputbuffer, 1, length, f);
    }
    fclose(f);
  }
  else
  {
    printf("file %s can't be opened! \n", filename);
    return EXIT_FAILURE;
  }

  fsm_t *fsm;

  switch (type)
  {
  default:
  case INT:
    // call int's accept
    break;
  case STR:
    // call string's accept
    fsm = string_init(inputbuffer);
    if (accept_string(fsm, &result))
    {
      success = true;
      printf("STRING: %s", result);
    }
    break;
  case VAL:
    // call value's accept
    break;
  case OBJ:
    // call object's accept
    break;
  }
  if (success)
  {
    printf("Success!\n");
    return EXIT_SUCCESS;
  }
  else
  {
    printf("Parsing %s failed\n", filename);
    return EXIT_SUCCESS;
  }
}

/* Parse the command-line arguments. Sets the type based on whether the
   -i -s -v or -o flag was passed. If -d was passed, turn on debugging
   mode. Set the filename based on the last parameter. */
static bool
get_args(int argc, char **argv, fsm_type_t *type, char **filename)
{
  int ch = 0;
  while ((ch = getopt(argc, argv, "isvodh")) != -1)
  {
    switch (ch)
    {
    case 'i':
      *type = INT;
      break;
    case 's':
      *type = STR;
      break;
    case 'v':
      *type = VAL;
      break;
    case 'o':
      *type = OBJ;
      break;
    case 'd':
      debug = true;
      break;
    default:
      return false;
    }
  }
  *filename = argv[optind];
  return true;
}

static void
usage(void)
{
  printf("json, a simple JSON parser\n\n");
  printf("usage: json flag filename\n");
  printf("filename is the name of the file with JSON contents.\n");
  printf("flag is one of the following:\n");
  printf("  -i         read the file as an integer\n");
  printf("  -s         read the file as a string\n");
  printf("  -v         read the file as a value (integer or string)\n");
  printf("  -o         read the file as an object\n");
  printf("  -d         turn on debugging mode\n");
  printf("The last of -i, -s, -v, and -o will be used.\n");
}
