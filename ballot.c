#include "ballot.h"
#include "helpers.h"
#include "libvc.h"
#include <lib211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};


ballot_t ballot_create(void)
{
    ballot_t b  = malloc(sizeof(struct ballot));

    b->length = 0;
    
    return b;
}

void ballot_destroy(ballot_t ballot)
{
    // first free the string in each entry
    // going into the array of entries in ballot
    size_t i = 0;

    while (i < ballot->length){
        free((ballot->entries)[i].name);
        i++;    
    }
    free(ballot);   
}

void ballot_insert(ballot_t ballot, char* name)
{
    clean_name(name);
    // checking if the ballot is full already
    if (ballot->length == MAX_CANDIDATES){
        exit(3);
    }
    

    (ballot->entries)[ballot->length].name = name;
    
    (ballot->entries)[ballot->length].active = true;
    
    ballot->length = ballot->length + 1;
}

const char* ballot_leader(ballot_t ballot)
{
    size_t i = 0;
    
    while (i < ballot->length){
        if ((ballot->entries)[i].active){
            return (ballot->entries)[i].name;
        }
        i++;
        
    }
    
    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    size_t i = 0;

    while (i < ballot->length){
        if (strcmp((ballot->entries)[i].name, name) == 0){
            (ballot->entries)[i].active = false;
        }
        i++;
    }
    
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    if (vc_update(vc, ballot_leader(ballot)) == NULL) {
        exit(4);
    } else {
        size_t* cp = vc_update(vc, ballot_leader(ballot));
        ++*cp;
    }
    
   
}

ballot_t read_ballot(FILE* inf)
{
//    FILE* the_file = fopen(*inf, "r");
    ballot_t b = ballot_create();
    char* input = fread_line(inf);

    // checking for end of file
    if (fread_line(inf) == NULL){
        return NULL;
    }

    while (input[0] != '%'){

        ballot_insert(b, input);
        input = fread_line(inf);      
    }
    free(input);      
    return b;
}

void clean_name(char* name)
{
    char* dst = name;

    while (*name != '\0'){
        // if the char isn't an alphabetical char, remove it
        int c = *name;
        
        if (isalpha(c) == 0){
            name = name + 1;
        } else {
            //char c = *src;
            
            *dst = toupper(c);
            dst = dst + 1;
            name = name + 1;
            
        }
    }
    *dst = '\0';
    
    
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i) {
        bool active = ballot->entries[i].active;
        fprintf(outf, "%c%s%s\n",
                active? ' ' : '[',
                ballot->entries[i].name,
                active? "" : "]");
    }
}
