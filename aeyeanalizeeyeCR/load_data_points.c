
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "load_data_points.h"

static FILE* get_file_pointer_from_file_name(char *fileName)
{
    FILE *eyeTrackingDataFileP = NULL;
    int errorCount = 0;
    
    do
    {
        eyeTrackingDataFileP = fopen(fileName, "r");
        if (!eyeTrackingDataFileP)
        {
            errorCount++;
            fprintf(stderr, "Failed to open the file %s\n", fileName);
            printf("Please re-enter file name: ");
            scanf("%s", fileName);
        }
    } while(eyeTrackingDataFileP == NULL && errorCount < 3);
    
    return  eyeTrackingDataFileP;
}

static int has_valid_data(char* inputLine)
{
    return (!strstr(inputLine, "No Media")) && (!strstr(inputLine, "ScreenRec"));
}

// This function assumes line is a null terminated string.
static int has_digit(char* inputLine)
{
    register unsigned char* thisCharacter = (unsigned char *) inputLine;
    
    while (*thisCharacter)
    {
        if (isdigit(*thisCharacter))
        {
            return (int)(*thisCharacter);
        }
        thisCharacter++;
    }
    
    return (int)(*thisCharacter);
}

static PageNode* cleanup_memory_allocation_error_return_null(PageNode* pageList)
{
    delete_entire_page_list(&pageList);
    return pageList;
}

static PageNode* creat_page_list(char*textLineInFile, PageNode* pageList)
{
    PageNode* thisPage = construct_page_node_by_input_line(textLineInFile);
    if (thisPage)
    {
        pageList = thisPage;
    }
    else
    {
        fprintf(stderr, "Error in creat_page_list - can't create pageList\n");
    }

    return pageList;
}

static PageNode* find_proper_stimuli_and_add_fixation(char*textLineInFile, PageNode* pageList)
{
    char* pageName = extract_page_name_from_input_line(textLineInFile);
    
    PageNode* thisPage = find_page_by_name_or_abbreviation(pageName, pageList);
    
    if (thisPage)
    {
        construct_fixation_and_add_to_page(textLineInFile, thisPage);
    }
    else
    {
        thisPage = construct_page_node_by_input_line(textLineInFile);
        if (thisPage)
        {
            append_page_to_list(thisPage, pageList);
        }
        else
        {
            return cleanup_memory_allocation_error_return_null(pageList);
        }
    }
    
    return pageList;
}

PageNode *load_data_points(char *fileName)
{
    PageNode* pageList = NULL;
    char textLineInFile[BUFSIZ];
    
    FILE *eyeTrackingDataFileP = get_file_pointer_from_file_name(fileName);
    if (!eyeTrackingDataFileP)
    {
        return pageList;
    }
    
    int errorOccurred = 0;
    while(fgets(textLineInFile, BUFSIZ, eyeTrackingDataFileP) && !errorOccurred)
    {
        // Ignore lines that don't have any numbers such as header lines or
        // or lines where the stimuli is No Media or ScreenRec
        if ((has_digit(textLineInFile)) && (has_valid_data(textLineInFile)))
        {
            if (!pageList) {
                if (!(pageList = creat_page_list(textLineInFile, pageList)))
                {
                    errorOccurred++;
                }
            }
            else
            {
                if (!(pageList = find_proper_stimuli_and_add_fixation(textLineInFile, pageList)))
                {
                    errorOccurred++;
                }
            }
        }
    }

    fclose(eyeTrackingDataFileP);
    
    return pageList;
}
