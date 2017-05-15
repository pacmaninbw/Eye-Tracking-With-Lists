/*
 * https://codereview.stackexchange.com/questions/146870/a-eye-analysing-eye-tracking-data
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syslimits.h>
#include "load_data_points.h"
#include "clean_data_points.h"
#include "statistics.h"

static int get_page_name_show_AOI_statistics(PageNode *pageList)
{
    char pageName[PATH_MAX];
    int status = EXIT_SUCCESS;
    int topX = 0;
    int topY = 0;
    int bottomX = 0;
    int bottomY = 0;;
    
    printf("Enter a page name: \n");
    scanf("%s", pageName);
    
    PageNode* pageForStatistics = find_page_by_name_or_abbreviation(pageName, pageList);
    if(!pageForStatistics)
    {
        fprintf(stderr, "ERROR: Page %s not found in the list of pages.\n", pageName);
        return EXIT_FAILURE;
    }
    
    printf("Please enter the top [X,Y] and bottom [X,Y] coordinates of the AOI: \n");
    scanf("%d %d %d %d", &topX, &topY, &bottomX, &bottomY );
    
    show_AOI_statistics(pageForStatistics, topX, topY, bottomX, bottomY);
    
    return  status;
}

static int show_menu_get_option()
{
    int option = 0;
    fflush(stdin);
    
    printf("\n\n\n------------------------\n");
    printf("Eye Menu\n");
    printf("------------------------\n");
    printf("1. Clean Data Points\n");
    printf("2. Show Page Statistics\n");
    printf("3. Show AOI Statistics\n");
    printf("4. Exit from A-Eye\n");
    printf("------------------------\n");
    printf("Enter your option: ");
    scanf("%d", &option);

    fflush(stdin);

    return option;
}

typedef enum {
    UNKOWN_INPUT_OPTION = 0,
    CLEAN_DATA_POINTS = 1,
    SHOW_PAGE_STATISTICS = 2,
    SHOW_AREA_OF_INTEREST_STATISTICS = 3,
    EXIT_A_EYE_TRACKING = 4
} InputOptions;

static int execute_menu_loop(PageNode* pageList)
{
    int status = EXIT_SUCCESS;
    int option = UNKOWN_INPUT_OPTION;
    
    do
    {
        option = show_menu_get_option();
        switch(option)
        {
            case CLEAN_DATA_POINTS:
                clean_data_points(pageList);
                break;
                
            case SHOW_PAGE_STATISTICS:
                show_page_statistics(pageList);
                break;
                
            case SHOW_AREA_OF_INTEREST_STATISTICS:
                get_page_name_show_AOI_statistics(pageList);
                break;
                
            case EXIT_A_EYE_TRACKING:
                printf("\nGoodbye! \n");
                break;
                
            default:
                fprintf(stderr, "\nERROR: Please re-enter option number, unknown option %d entered!\n", option);
        }
        
    }
    while(option != EXIT_A_EYE_TRACKING);
    
    return status;
}

int main(int argc, char* argv[])
{
    int status = EXIT_SUCCESS;
    char fileName[PATH_MAX];    // system macro available on Mac OSX and Linux
    
    printf("A-Eye: Analysing Eye tracking Data \n");
    printf("Enter file name: ");
    scanf("%s", fileName);
    
    PageNode* pageList = load_data_points(fileName);
    
    if (pageList)
    {
        printf("The data file %s successfully loaded.\n", fileName);
        status = execute_menu_loop(pageList);
        delete_entire_page_list(&pageList);
    }
    else
    {
        fprintf(stderr, "A-Eye: Analysing Eye tracking Data Data Failed to load properly\n");
        status = EXIT_FAILURE;
    }
    
    return status;
}

