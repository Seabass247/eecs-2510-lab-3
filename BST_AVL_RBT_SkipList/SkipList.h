#pragma once
class SkipList
{
public:
    SkipList();

    void Insert(const char* X);
    void List();
    void Height();
private:
    const char* NEG_INF = "negInf";
    const char* POS_INF = "posInf";
    struct SkipListNode
    {
        const char* key;
        int count = 1;
        int level;
        SkipListNode *up, *down, *left, *right; // all four links
    };
    SkipListNode *head, *tail;
    int h; // Height (number of lanes)
    int n; // Number of items in list (slow lane)
    
    SkipListNode* search(const char* word, bool& found);
};