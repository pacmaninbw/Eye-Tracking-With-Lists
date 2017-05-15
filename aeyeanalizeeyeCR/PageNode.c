
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "PageNode.h"

/*
 * Local functions.
 */
static char* convert_entire_string_to_lowercase(char *stringToLower)
{
    register char* charToLower = stringToLower;
    
    while (*charToLower)
    {
        *charToLower = tolower((unsigned char)(*charToLower));
        charToLower++;
    }
    
    return stringToLower;
}

static PageNode* find_page_node_by_page_name(char* pageName, PageNode* pageList)
{
    register PageNode* foundPageNode = pageList;
    
    while (foundPageNode != NULL) {
        if(!strcmp(foundPageNode->pageURL, pageName))
        {
            return foundPageNode;
        }
        foundPageNode = foundPageNode->next;
    }
    
    return foundPageNode;
}

static PageNode* find_page_node_by_abbreviated_name(char *abbreviatedPageName, PageNode* pageList)
{
    register PageNode *foundPageNode = pageList;
    
    while(foundPageNode != NULL)
    {
        if (strstr(foundPageNode->pageURL, abbreviatedPageName))
        {
            return foundPageNode;
        }
        foundPageNode = foundPageNode->next;
    }
    
    return foundPageNode;
}

static void delete_page_node(PageNode *pageToDelete)
{
    if (pageToDelete->FixationList)
    {
        delete_entire_fixation_list(&pageToDelete->FixationList);
    }
    
    if (pageToDelete->pageURL)
    {
        free(pageToDelete->pageURL);
        pageToDelete->pageURL = NULL;
    }
    
    free(pageToDelete);
}

/*
 * External API's start here
 */
PageNode* find_page_by_name_or_abbreviation(char* pageName, PageNode* pageList)
{
    register PageNode *foundPageNode = NULL;
    
    pageName = convert_entire_string_to_lowercase(pageName);
    foundPageNode = find_page_node_by_page_name(pageName, pageList);
    if (!foundPageNode) {
        foundPageNode = find_page_node_by_abbreviated_name(pageName, pageList);
    }

    return foundPageNode;
}

/*
 * The format of the raw input data is basically CSV although rather than comma's
 * it is separated by white space (tabs seem to be what the separation is).
 * The format of the data is 5 numeric fields and then a text field that may
 * have spaces in it.
 */
char* extract_page_name_from_input_line(char* rawInputLine)
{
    char* pageName = NULL;
    register unsigned char* thisCharacter = ((unsigned char *)rawInputLine);
    
    while (*thisCharacter)
    {
        if (isalpha(*thisCharacter))
        {
            pageName = (char*) thisCharacter;
            break;
        }
        thisCharacter++;
    }

    return pageName;
}

PageNode *construct_page_node_by_page_name(char *name)
{
    PageNode *newPageNode = NULL;
    
    if ((!name) || (strlen(name) < 1))
    {
        fprintf(stderr, "Error Page Node: No name to create Page Data Node\n");
        return newPageNode;
    }
    
    // Calloc is used to set all fields to zero/NULL
    newPageNode = (PageNode *) calloc(1, sizeof(PageNode));
    if (newPageNode)
    {
        // create a copy of the name to store in the page node
        // all stimuli/web page names converted to lower case to reduce the complexity of the search.
        if (!(name = strdup(name)))
        {
            fprintf(stderr, "Error construct_page_node_by_page_name: strdup failed failed to copy page name\n");
            delete_page_node(newPageNode);
            return NULL;
        }
        newPageNode->pageURL = convert_entire_string_to_lowercase(name);
    }
    else
    {
        fprintf(stderr, "Error Page Node: calloc failed to allocate memory for PageNode\n");
    }
    
    return newPageNode;
}

PageNode *construct_page_node_by_input_line(char *rawInputLine)
{
    PageNode *newPageNode = NULL;
    
    if ((!rawInputLine) || (strlen(rawInputLine) < 10)) { // minimum 1 character per variable and 1 space separating each variable
        fprintf(stderr, "Error Fixation Node: No input dataline to create Fixation Data Node\n");
        return newPageNode;
    }
    
    char* pageName = extract_page_name_from_input_line(rawInputLine);
    newPageNode = construct_page_node_by_page_name(pageName);
    if (newPageNode)
    {
        construct_fixation_and_add_to_page(rawInputLine, newPageNode);
        if (!newPageNode->FixationList)
        {
            delete_page_node(newPageNode);
            newPageNode = NULL;
        }
    }
    
    return newPageNode;
}

// Pages will be inserted in alphbetical order based on the page name/page URL
// Assumes it won't be called to insert duplicates.
void insert_page_in_list(PageNode *insertee, PageNode* pageList)
{
    register PageNode* currentPage = pageList;
    register PageNode* tail = NULL;
    while(currentPage)
    {
        int difference = strcmp(insertee->pageURL, currentPage->pageURL);
        if (currentPage->next && difference > 0)
        {
            insertee->next = currentPage->next;
            currentPage->next = insertee;
            return;
        }
        tail = currentPage;
        currentPage = currentPage->next;
    }
    // Inserting the new largest value at the end.
    tail->next = insertee;
}

void append_page_to_list(PageNode* appendee, PageNode* pageList)
{
    register PageNode* tail = pageList;
    while (tail->next) {
        tail = tail->next;
    }
    tail->next = appendee;
}

void delete_page_from_list(PageNode* nodeToDelete, PageNode* pageList)
{
    register PageNode *pageDataNode = pageList;
    
    while (pageDataNode)
    {
        if (pageDataNode->next == nodeToDelete)
        {
            pageDataNode->next = nodeToDelete->next;
            delete_page_node(nodeToDelete);
            return;
        }
        
        pageDataNode = pageDataNode->next;
    }
}

void construct_fixation_and_add_to_page(char* inputLine, PageNode* pageDataNode)
{
    FixationNode* newFixation = construct_fixation_node(inputLine);
    if (!newFixation)
    {
        fprintf(stderr, "ERROR in construct_fixation_and_add_to_page: First fixation node creation failed\n");
        return;
    }
    insert_fixation_in_list_by_index(newFixation, &pageDataNode->FixationList);
}

void delete_entire_page_list(PageNode** pageList)
{
    while(*pageList)
    {
        PageNode* deletee = *pageList;
        *pageList = deletee->next;
        delete_page_node(deletee);
    }
}
