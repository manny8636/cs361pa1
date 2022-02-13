#ifndef __statemodel_h__
#define __statemodel_h__

#include <stdbool.h>
#include <stdint.h>

extern bool debug; // Global variable that is used for debugging transitions

/* States and events should just be integers */
typedef int state_t;
typedef int event_t;

// Needed for circular typedef. This lets action_t use fsm_t in its parameter
// list, while the struct fsm can use action_t as a field.
typedef struct fsm fsm_t;

// All entry, exit, and effect instances use the action type
typedef void (*action_t)(fsm_t *);

// Generic FSM structure with pointers to the transition, effect, and
// entry tables. Note that transitions and effects should point to
// dynamically allocated arrays of pointers to the rows of the table.
// This will allow you to still access fsm->transitions[state][event]
// as needed.
struct fsm
{
  state_t state;  /* current state */
  size_t nevents; /* number of events for this FSM */

  /* pointer to the FSM's transition function */
  state_t (*transition)(struct fsm *, event_t, action_t *, action_t *);

  // Pointers to the input text, including the current byte being processed
  char const *input;
  char const *current;

  // TODO: Extend this with additional fields you need to hold information
  // during the execution of your FSM.

  bool final; /* set on accept or reject */

  // buffer to contain parse output
  char *buffer;
  size_t length;
  size_t buffer_size;
  char *last_appended_char;
  char *last_input_char;
  event_t next_event;
};

// Generic entry point for handling events
void handle_event(fsm_t *fsm, event_t event);

#endif
