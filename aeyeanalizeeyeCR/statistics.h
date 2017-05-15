
#ifndef statistics_h
#define statistics_h

#ifndef PageNode_h
#include "PageNode.h"
#endif

extern void show_page_statistics(PageNode* page);
extern void show_AOI_statistics(PageNode* webName, int topX, int topY, int bottomX, int bottomY);

#endif 
