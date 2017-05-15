#include <stdio.h>
#include <string.h>
#include "statistics.h"

static int total_fixation_duration(FixationNode* fixationList)
{
    register int total = 0;
    register FixationNode* thisFixation = fixationList;
    while(thisFixation != NULL)
    {
        total += thisFixation->duration;
        thisFixation = thisFixation->nextFixation;
    }
    
    return total;
}

static int count_fixation_points(FixationNode* fixationList)
{
    register int count = 0;
    register FixationNode* thisFixation = fixationList;
    while(thisFixation != NULL)
    {
        count++;
        thisFixation = thisFixation->nextFixation;
    }
    
    return count;
}

void show_page_statistics(PageNode* pageList)
{
    char website[BUFSIZ];
    
    printf("\nEnter a website name: ");
    scanf("%s", website);
    PageNode* thisPage = NULL;
    
    thisPage = find_page_by_name_or_abbreviation(website, pageList);
    if(!thisPage)
    {
        fprintf(stderr, "ERROR: web page %s not in the list of pages\n", website);
        return;
    }
    
    FixationNode* fixationList = thisPage->FixationList;
    long totalDuration = total_fixation_duration(fixationList);
    int fixationPointCount = count_fixation_points(fixationList);
    
    printf("The number of fixations: %d\nThe total duration of fixations: %ld milliseconds\n", fixationPointCount, totalDuration);
    printf("The average duration of fixations is %8.2f milliseconds\n", (double) ((double)totalDuration)/((double)fixationPointCount));
}

static int is_in_range(int coordinate, int top, int bottom)
{
   return (coordinate >= top && coordinate <= bottom);
}

void show_AOI_statistics(PageNode* thisPage, int topX, int topY, int bottomX, int bottomY)
{
    int count = 0;
    long total = 0;
    register FixationNode* thisFixation = thisPage->FixationList;
    
    while (thisFixation != NULL)
    {
        int tempX = thisFixation->coordinateX;
        int tempY = thisFixation->coordinateY;
        
        if (is_in_range(tempX, topX, bottomX) && is_in_range(tempY, topY, bottomY))
        {
            total += thisFixation->duration;
            count++;
        }
        
        thisFixation = thisFixation->nextFixation;
    }
    
    printf("The number of fixations: %d \nThe total duration of fixations: %ld milliseconds\n", count, total );
}
