#include <stdbool.h>
#include <stdlib.h>

#include "statemodel.h"
#include "stringmodel.h"
#include <string.h>

/* Begins at fsm->current and tries to identify a string in the given
   input data. A string begins and ends with ". In between, only two
   special escape sequences are allowed: \\ and \". If the string is
   successfully built, the result pointer will be changed to point to
   a dynamically allocated copy and return true. Return false if any
   error occurs. The escape sequences in the input will be replaced with
   the appropriate character (e.g., \\ in the input becomes \ in the
   result string). (HINT: Note that the effect functions do not have
   access to local variables here; instead, you should extend the fsm_t
   declaration to include helper variables, such as a buffer to copy the
   input bytes into.) */
bool accept_string(fsm_t *fsm, char **result)
{

  // should the generic handle_event be called here?

  // read the input string in a loop

  event_t evt = NIL_CHAR;
  char *input_ch;
  char *prior_input_ch = "";

  // fsm->input
  char *walker = fsm->input;
  int cnt = 0;

  while (*walker != '\0')
  {
    input_ch = walker;
    if (!strcmp(input_ch, "\"") && cnt == 0)
      evt = OPEN_QUOTE;
    else if (!strcmp(input_ch, "\"") && cnt == strlen(fsm->input))
      evt = CLOSE_QUOTE;
    else if (!strcmp(input_ch, "\\") && !strcmp(prior_input_ch, "\\"))
      evt = BACKSLASH;
    else if (!strcmp(input_ch, "\\"))
      evt = ESC_CHAR;
    else
      evt = NON_CTRL;

    strcpy(prior_input_ch, input_ch);
    walker++;
    cnt++;

    handle_event(fsm, evt);
  }

  // when accepting is done, we point the result to the fsm built buffer
  if (fsm->final)
  {
    result = &fsm->buffer;
    fsm->final = true;
  }

  return false;
}

/* Begins at fsm->current and tries to build a valid integer value. This
   function should accept both positive and negative numbers, and should
   allow leading 0s to indicate that the number is in octal format (e.g.,
   023 should print as the decimal value 19). Whitespace, the } character,
   the , character, and the null byte '\0' can be used to indicate the end
   of a number. Any other non-digit value should be considered a NON_DIGIT
   and result in a syntax error. If the number was successfully built, it
   should be copied into the location pointed to by the call-by-reference
   parameter value and the function should return true. Otherwise, return
   false. */
bool accept_integer(fsm_t *fsm, int64_t *value)
{
  return false;
}

/* Begins at fsm->current and tries to build a value that can be either
   a string or an integer. Skip over leading whitespace until either a
   leading ", -, or digit is encountered. The " indicates the value will
   be a string, whereas the others indicate an integer. Other characters
   should cause a BAD_VALUE event, triggering a syntax error message.
   Returns true only if a valid string or integer value was accepted.
   If a value was successully parsed, sets the is_string, string, and
   value call-by-reference parameters as appropriate. */
bool accept_value(fsm_t *fsm, bool *is_string, char **string, int64_t *value)
{
  return false;
}

/* Begins at fsm->current and tries to build a valid JSON object. All JSON
   objects consist of key-value pairs, where the key is a string and the
   value can be either a string or integer. If more than one key-value pair
   is in the object, they are separated by commas. The following examples
   are all valid (note that whitespace is ignored):

     {"a":1}
     { "first": "second", "alpha": "beta"}
     { "integer": 1, "string": "one" }

   These are examples of bad objects (and the corresponding error events):

     { not : "good" }  -- BAD_TOKEN
     { "no \t allowed" : } -- BAD_ID
     { "alpha" } -- NON_COLON
     { "float" : 1.5 } -- BAD_VALUE
     { "a" : "b" : "c" } -- BAD_TOKEN
     { "a" : "b", } -- BAD_TOKEN

   All key-value pairs for an object will be concatenated as a string
   and returned via the keys call-by-reference parameter. The format
   should look exactly as (ignore the leading space):

     KEYS[integer] = 1
     KEYS[string] = one

   AppendKeyValuePair should append a new line (ending in '\n') for
   each key-value pair found. In the case above, the exact string
   returned would be:

     "KEYS[integer] = 1\nKEYS[string] = one\n"

   Return true if the object is successfully parsed, false otherwise.
   */
bool accept_object(fsm_t *fsm, char **keys)
{
  return false;
}
