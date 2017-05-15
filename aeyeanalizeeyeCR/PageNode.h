
#ifndef PageNode_h
#define PageNode_h

#ifndef FixationNode_h
#include "FixationNode.h"
#endif

typedef struct pageNode
{
    struct pageNode *next;
    char *pageURL;
    FixationNode *FixationList;
} PageNode;

extern char* extract_page_name_from_input_line(char* rawInputLine);
extern PageNode* find_page_by_name_or_abbreviation(char* pageName, PageNode* pageList);
extern PageNode *construct_page_node_by_page_name(char *pageName);
extern PageNode *construct_page_node_by_input_line(char *inputLine);
extern void insert_page_in_list(PageNode *insertee, PageNode* pageList);
extern void append_page_to_list(PageNode* appedee, PageNode* pageList);
extern void delete_page_from_list(PageNode* deletee, PageNode* pageList);
extern void construct_fixation_and_add_to_page(char* inputLine, PageNode* pageDataNode);
extern void delete_entire_page_list(PageNode** pageList);

#endif
