#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "statemodel.h"
#include "stringmodel.h"

static void AdvancePointer(fsm_t *);
static void AllocateBuffer(fsm_t *);
static void AppendCharacter(fsm_t *);
static void ReplaceCharacter(fsm_t *);
static void SyntaxError(fsm_t *);

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

// /////////////////// EFFECT FUNCTIONS ////////////////////

static state_t parse_transition(fsm_t *fsm, event_t event, action_t *effect, action_t *entry);
// fsm->transition = &parse_transition

// static void accept(fsm_t *);
// static void reject(fsm_t *);

static state_t const _transition[NSTR_STATES][NSTR_EVENTS] = {
    // OPEN_QUOTE NON_CTRL BACKSLASH NO_ESC ESC_CHAR CLOSE_QUOTE
    {BUILDING, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR},    // ENTER
    {NON_STR, BUILDING, ESCAPE, NON_STR, NON_STR, STR_FINISH},  // BUILDING
    {NON_STR, NON_STR, NON_STR, STR_FINISH, BUILDING, NON_STR}, // ESCAPE
};

static action_t const _effect[NSTR_STATES][NSTR_EVENTS] = {
    // OPEN_QUOTE NON_CTRL BACKSLASH NO_ESC ESC_CHAR CLOSE_QUOTE
    {AllocateBuffer, NULL, NULL, NULL, NULL, NULL},            // ENTER
    {NULL, AppendCharacter, NULL, NULL, NULL, AdvancePointer}, // BUILDING
    {NULL, NULL, NULL, SyntaxError, ReplaceCharacter, NULL},   // ESCAPE
};

// static action_t const _entryArr[NSTR_STATES] =
//     {NULL, NULL}; // NEW

/* Return an FSM that links to these internals */
fsm_t *
string_init(char const *input)
{
   fsm_t *fsm = calloc(1, sizeof(fsm_t));
   fsm->nevents = NSTR_EVENTS;
   fsm->state = STR_INIT;
   fsm->transition = parse_transition;

   /* Set up internal fields for keeping track of characters */
   fsm->input = input;
   fsm->next_event = OPEN_QUOTE;
   fsm->final = false;

   return fsm;
}

/* Used to move beyond the quote at the end of the string */
static void AdvancePointer(fsm_t *fsm)
{

   fsm->final = true;
}

/* Create a dynamically allocated buffer for storing the string as it is
   being built. Note that you will need to modify the fsm_t struct declaration
   to include whatever fields you may need for managing the buffer. */
static void
AllocateBuffer(fsm_t *fsm)
{
   // Allocate buffer is called once when we see open quote
   // must be able to contain the whole string.

   fsm->buffer_size = strlen(fsm->input) + 1;
   fsm->buffer = (char *)calloc(fsm->buffer_size, sizeof(char));
   fsm->length = 0;
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter(fsm_t *fsm)
{

   // assert(fsm->length < fsm->buffer_size - 1);
   strcat(fsm->buffer, fsm->current);
   strcpy(fsm->last_appended_char, fsm->current);
   fsm->length++;
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter(fsm_t *fsm)
{
   // assert(fsm->length < fsm->buffer_size - 1);
   fsm->buffer[fsm->length - 1] = *fsm->current;
}

/* Given FSM instance and event, perform the table lookups */
static state_t
parse_transition(fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
   /* If the state/event combination is bad, return -1 */
   if (fsm->state >= NON_STR || event >= NIL_CHAR || _transition[fsm->state][event] == NON_STR)
      return -1;

   /* Look up the effect and transitions in the tables */
   *effect = _effect[fsm->state][event];

   /* Look up the next state in the list of entry events */
   state_t next = _transition[fsm->state][event];
   // if (next != NON_STR)
   //    *entry = _entryArr[next];

   return next;
}

/* Reports an invalid escape-code character */
static void
SyntaxError(fsm_t *fsm)
{
   printf("SYNTAX ERROR: '%c%c' is not a valid escape code\n", *fsm->last_appended_char, *fsm->current);
}
