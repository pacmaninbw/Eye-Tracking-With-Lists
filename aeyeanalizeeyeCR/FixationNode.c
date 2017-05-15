#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FixationNode.h"

FixationNode *construct_fixation_node(char *inputLine)
{
    FixationNode *newFixation = NULL;

     // strlen of then because minimum 1 character per variable and 1 space separating each variable
    if ((!inputLine) || (strlen(inputLine) < 10)) {
        fprintf(stderr, "Error Fixation Node: No input dataline to create Fixation Data Node\n");
        return newFixation;
    }
    
    newFixation = (FixationNode *) calloc(1, sizeof(FixationNode));
    if (newFixation) {
        sscanf(inputLine, "%d\t%d\t%d\t%d\t%d", &newFixation->index, &newFixation->timeStamp, &newFixation->duration, &newFixation->coordinateX, &newFixation->coordinateY);
    }
    else
    {
        fprintf(stderr, "Error Fixation Node: calloc failed to allocate memory for FixationNode\n");
    }
    
    return newFixation;
}

// Insertion will be based on fixation index in Ascending order
void insert_fixation_in_list_by_index(FixationNode* insertee, FixationNode** fixationList)
{
    if (!*fixationList)
    {
        *fixationList = insertee;
        return;
    }
    
    register FixationNode* currentFixation = *fixationList;
    register FixationNode* last = NULL;
    while(currentFixation)
    {
        if (currentFixation->nextFixation && insertee->index < currentFixation->nextFixation->index)
        {
            insertee->nextFixation = currentFixation->nextFixation;
            currentFixation->nextFixation = insertee;
            return;
        }
        last = currentFixation;
        currentFixation = currentFixation->nextFixation;
    }
    // Inserting the new largest value at the end.
    last->nextFixation = insertee;
}

// This can append a single fixation node to the list
// or it can append a list of fixation nodes to the list.
void append_fixation_to_list(FixationNode* appendee, FixationNode** fixationList)
{
    register FixationNode* currentFixation = *fixationList;
    
    if (!*fixationList)
    {
        *fixationList = appendee;
    }
    
    while (currentFixation->nextFixation) {
        currentFixation = currentFixation->nextFixation;
    }
    currentFixation->nextFixation = appendee;
}

void delete_fixation_from_list_by_pointer_value(FixationNode* deletee, FixationNode** fixationList)
{
    if (!deletee)
    {
        fprintf(stderr, "In delete_fixation_from_list_by_pointer_value() Nothing to delete");
        return;
    }
    if (!fixationList || !*fixationList)
    {
        fprintf(stderr, "In delete_fixation_from_list_by_pointer_value() No list to delete from.");
        return;
    }
    if (deletee == *fixationList)
    {
        *fixationList = (*fixationList)->nextFixation;
        free(deletee);
        return;
    }
    
    register FixationNode* currentFixation = *fixationList;
    while(currentFixation)
    {
        if (currentFixation->nextFixation == deletee)
        {
            currentFixation->nextFixation = deletee->nextFixation;
            free(deletee);
            return;
        }
        currentFixation = currentFixation->nextFixation;
    }
}

void delete_fixation_from_list_by_index_value(register int index, FixationNode** fixationList)
{
    register FixationNode* currentFixation = *fixationList;
    
    while (currentFixation)
    {
        if (currentFixation->index == index)
        {
            delete_fixation_from_list_by_pointer_value(currentFixation, fixationList);
            return;
        }
        currentFixation = currentFixation->nextFixation;
    }
}

void delete_entire_fixation_list(FixationNode** fixationList)
{
    while(*fixationList)
    {
        FixationNode* deletee = *fixationList;
        *fixationList = deletee->nextFixation;
        free(deletee);
    }

}

