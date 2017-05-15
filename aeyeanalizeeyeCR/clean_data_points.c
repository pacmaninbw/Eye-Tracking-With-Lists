
#include <stdio.h>
#include <stdlib.h>
#include "clean_data_points.h"

void clean_data_points(PageNode* pagelist)
{
    int threshold = 0;
    int removed = 0;
    
    printf("Enter the threshold value in milliseconds: ");
    scanf("%d", &threshold);
    
    register PageNode* thisPage = pagelist;
    while(thisPage != NULL)
    {
        register FixationNode* thisFixation = thisPage->FixationList;
        
        while(thisFixation != NULL)
        {
            if (thisFixation->duration < threshold )
            {
                FixationNode* next = thisFixation->nextFixation;
                delete_fixation_from_list_by_pointer_value(thisFixation, &thisPage->FixationList);
                thisFixation = next;
                removed++;
            }
            else
            {
                thisFixation = thisFixation->nextFixation;
            }
        }
        
        thisPage = thisPage->next;
    }
    
    printf("The number of fixations removed: %d", removed);
}


