///
/// Tests for functions in ../src/ballot_box.c.
///

#include "ballot_box.h"
#include "helpers.h"

#include <lib211.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Takes the expected winner, followed by all the votes. Each ballot is
// given as successive string arguments, terminated by "%". The final
// argument, after the final ballot's "%", must be NULL.
static void assert_election(const char* expected_winner, ...);

// Test case functions (you need more!):
static void three_candidates_tied(void),
            win_on_second_round(void),
            example_from_wikipedia(void);


///
/// MAIN FUNCTION
///

int main(void)
{
    three_candidates_tied();
    win_on_second_round();
    example_from_wikipedia();
}


///
/// TEST CASE FUNCTIONS
///

static void three_candidates_tied(void)
{
    assert_election("A",
            "a", "b", "c", "%",
            "a", "b", "c", "%",
            NULL);
}

static void win_on_second_round(void)
{
    assert_election("BIDEN",
            "Bloomberg", "Biden", "%",
            "Yang", "Biden", "%",
            "Warren", "Biden", "%",
            "Sanders", "Biden", "%",
            "Biden", "Sanders", "%",
            "Biden", "Gabbard", "%",
            NULL);
}

static void example_from_wikipedia(void)
{
    assert_election("SUE",
            "bob", "bill", "s u e", "%",
            "Sue", "Bob", "Bill", "%",
            "Bill!", "Sue!", "BoB!", "%",
            "bob", "bill", "sue", "%",
            "sue", "bob", "bill", "%",
            NULL);
}


///
/// HELPER FUNCTIONS
///

static void assert_election(const char* expected, ...)
{
    ballot_box_t bb = empty_ballot_box;

    {
        ballot_t ballot = ballot_create();

        va_list ap;
        va_start(ap, expected);

        char* name;
        while ((name = va_arg(ap, char*))) {
            if (strcmp(name, "%") == 0) {
                bb_insert(&bb, ballot);
                ballot = ballot_create();
            } else {
                ballot_insert(ballot, strdupb(name, "assert_election"));
            }
        }

        va_end(ap);

        ballot_destroy(ballot);
    }

    char* actual = get_irv_winner(bb);
    if (expected)
        CHECK_STRING(actual, expected);
    else
        CHECK_POINTER(actual, expected);
    free(actual);

    bb_destroy(bb);
}
