///
/// Tests for functions in ../src/ballot.c.
///

#include "ballot.h"
#include "libvc.h"
#include "helpers.h"

#include <lib211.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Asserts that `clean_name(name)` produces `expected`. (Borrows both
// arguments.)
static void assert_clean_name(const char* name, const char* expected);

// Test case functions (you need more!):
static void test_clean_name(void);
static void test_ballot(void);
static void test_ballot_with_vc(void);


///
/// MAIN FUNCTION
///

int main(int argc, char* argv[])
{
    test_clean_name();
    test_ballot();
    test_ballot_with_vc();
}


///
/// TEST CASE FUNCTIONS
///

static void test_clean_name(void)
{
    assert_clean_name("Alan Turing", "ALANTURING");
    assert_clean_name("Stroustrup, Bjarne", "STROUSTRUPBJARNE");
    assert_clean_name("C4P0", "CP");
    assert_clean_name("    ", "");
}

static void test_ballot(void)
{
    ballot_t ballot = ballot_create();
    assert(ballot);

    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_insert(ballot, strdupb("A", "test_ballot"));
    ballot_insert(ballot, strdupb("B", "test_ballot"));
    ballot_insert(ballot, strdupb("C", "test_ballot"));

    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "D");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "B");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "A");
    CHECK_STRING(ballot_leader(ballot), "C");
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_destroy(ballot);
}

static void test_ballot_with_vc(void)
{
    //
    // TODO: your code here
    //
}


///
/// HELPER FUNCTIONS
///

// Asserts that `clean_name(name)` returns `expected`.
static void assert_clean_name(const char* name, const char* expected)
{
    char* actual = strdupb(name, "assert_clean_name");
    clean_name(actual);
    CHECK_STRING(actual, expected);
    free(actual);
}
