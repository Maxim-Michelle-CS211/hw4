#include "ballot_box.h"
#include "libvc.h"
#include "helpers.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// A `ballot_box_t` (defined in `ballot_box.h`) is a linked list made
// of `struct bb_node`s. Each node owns the next node, so the whole list
// is owned by the pointer to the first node.
struct bb_node
{
    ballot_t ballot;
    struct bb_node* next;
};


// The empty ballot box is the null pointer.
const ballot_box_t empty_ballot_box = NULL;


void bb_destroy(ballot_box_t bb)
{
    while (bb){
	ballot_box_t temp = bb->next;	
        ballot_destroy(bb->ballot);
	free(bb);

        bb = temp;

    }         
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    ballot_box_t head = mallocb(sizeof *head, "bb_insert");
    head->ballot = ballot;
    head->next   = *bbp;
    *bbp = head;
}

ballot_box_t read_ballot_box(FILE* inf)
{
    bool reading = true;
    
    ballot_box_t bb = NULL;

    while (reading){
        ballot_t b = read_ballot(inf);
        if (b == NULL){
            reading = false;
        } else {
            bb_insert(&bb, b);
        }
    }
    
    return bb;
}

vote_count_t bb_count(ballot_box_t bb)
{
    vote_count_t result = vc_create();

    ballot_box_t current = bb;
    while (current){
        count_ballot(result, current->ballot);
        current = current->next;
        
    }
    

    return result;
}

void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    // iterate through all the ballots and eliminate candidate each time
    ballot_box_t current = bb;
    while (current){
        ballot_eliminate((current->ballot), candidate);
        current = current->next;
    }
    
}

char* get_irv_winner(ballot_box_t bb)
{
    bool undetermined = true;
    
    while (undetermined){
        vote_count_t vc = bb_count(bb);

        //check if there are votes
        if (vc_total(vc) == 0){
            undetermined = false;
        }
        
        //check if any candidate has a majority by checking that for the max 
        if (2 * (vc_lookup(vc, vc_max(vc))) > vc_total(vc)){
            char* winner = strdupb(vc_max(vc), "get_irv_winner");
            vc_destroy(vc);
            return winner;
        }
    
        //if code reaches here, then no candidate has a majority
        //therefore eliminate a candidate
        bb_eliminate(bb, vc_min(vc));
        vc_destroy(vc);
    }

    return NULL;
    
}
