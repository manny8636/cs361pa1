#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "statemodel.h"
#include "stringmodel.h"

static state_t parse_transition (fsm_t *, event_t, action_t *, action_t *);

static void AdvancePointer (fsm_t *);
static void AllocateBuffer (fsm_t *);
static void AppendCharacter (fsm_t *);
static void ReplaceCharacter (fsm_t *);
static void SyntaxError (fsm_t *);

/* Return an FSM that links to these internals */
fsm_t *
string_init (char const *input)
{
  fsm_t *fsm = calloc (1, sizeof (fsm_t));
  fsm->nevents = NSTR_EVENTS;
  fsm->state = STR_INIT;  
  fsm->transition = &parse_transition;
  fsm->input = input;
  fsm->current = input;

  return fsm;
}

/* Define additional functions or global data structures for this specific
   FSM. IMPORTANT: You must declare all such additional declarations as static
   so that they are accessible only from within this file. No other portion of
   your code should access these functions or data structures directly; all
   access should be indirect through the fsm_t structure. */

// Use the following print format string if a syntax error is
// encountered. The two characters should be the last one
// successfully processed and the next character causing the
// error.
// printf ("SYNTAX ERROR: '%c%c' is not a valid escape code\n",
static state_t const _transitions[NSTR_STATES][NSTR_EVENTS] = {
  // OPEN_QUOTE CLOSE_QUOTE NON_CTRL BACKSLASH ESC_CHAR NO_ESC
  { BUILDING, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR }, // STR_INIT
  { NON_STR, STR_FINISH, BUILDING, ESCAPE, NON_STR, NON_STR }, // BUILDING
  { NON_STR, NON_STR, NON_STR, NON_STR, BUILDING, STR_ERROR }, // ESCAPE
  { NON_STR, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR }, // STR_FINISH
  { NON_STR, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR }  // STR_ERROR
};
static action_t const _effect[NSTR_STATES][NSTR_EVENTS] = {
  // OPEN_QUOTE CLOSE_QUOTE NON_CTRL BACKSLASH ESC_CHAR NO_ESC
  { AllocateBuffer, NULL, NULL, NULL, NULL, NULL }, // STR_INIT
  { NULL, AdvancePointer, AppendCharacter, NULL, NULL, NULL }, // BUILDING
  { NULL, NULL, NULL, NULL, ReplaceCharacter, SyntaxError }, // ESCAPE
  { NULL, NULL, NULL, NULL, NULL, NULL }, // STR_FINISH
  { NULL, NULL, NULL, NULL, NULL, NULL }  // STR_ERROR
};
// /////////////////// EFFECT FUNCTIONS ////////////////////
static state_t
parse_transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  if (fsm->state >= NON_STR || _transitions[fsm->state][event] == NON_STR)
    return -1;
  *effect = _effect[fsm->state][event];
  state_t next = _transitions[fsm->state][event];
  return next;
}

/* Used to move beyond the quote at the end of the string */
static void
AdvancePointer (fsm_t *fsm)
{
   assert (fsm != NULL);
   fsm->current++;
}

/* Create a dynamically allocated buffer for storing the string as it is
   being built. Note that you will need to modify the fsm_t struct declaration
   to include whatever fields you may need for managing the buffer. */
static void
AllocateBuffer (fsm_t *fsm)
{
   assert (fsm != NULL);
   // fsm->buffer_size = strlen(fsm->input) + 1;
   // fsm->buffer = (char *)calloc(fsm->buffer_size, sizeof(char));
   // fsm->length = 0;
   memset (fsm->buffer, 0, sizeof (fsm->buffer));
   fsm->length = 0;
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter (fsm_t *fsm)
{
   assert (fsm != NULL);
   // assert (fsm->length < BUFFER_LENGTH - 1);
   fsm->buffer[fsm->length++] = *fsm->current;
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter (fsm_t *fsm)
{
   assert (fsm != NULL);
   // assert (fsm->length < BUFFER_LENGTH - 1);
   fsm->buffer[fsm->length++] = *fsm->current;
}

/* Reports an invalid escape-code character */
static void
SyntaxError (fsm_t *fsm)
{
   printf ("SYNTAX ERROR: '\\%c' is not a valid escape code\n", *fsm->current);
}
