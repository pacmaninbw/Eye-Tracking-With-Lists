
#ifndef FixationNode_h
#define FixationNode_h

typedef struct fixationNode
{
    int index;
    int timeStamp;
    int duration;
    int coordinateX;
    int coordinateY;
    struct fixationNode *nextFixation;
} FixationNode;

extern FixationNode *construct_fixation_node(char *inputLine);
extern void insert_fixation_in_list_by_index(FixationNode *insertee, FixationNode** fixationList);
extern void append_fixation_to_list(FixationNode* appendee, FixationNode** fixationList);
extern void delete_fixation_from_list_by_pointer_value(FixationNode* deletee, FixationNode** fixationList);
extern void delete_fixation_from_list_by_index_value(int index, FixationNode** fixationList);
extern void delete_entire_fixation_list(FixationNode** fixationList);

#endif
