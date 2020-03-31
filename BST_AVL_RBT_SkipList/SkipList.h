#pragma once
#include <random>

using namespace std;

class SkipList
{
public:
    SkipList();

    void Insert(const char* X);
    void List();
    int getListHeight() { return h; };
private:
    mt19937 coin;

    const char* NEG_INF = "negInf";
    const char* POS_INF = "posInf";
    struct SkipListNode
    {
        const char* key;
        int count = 1;
        SkipListNode *up, *down, *left, *right; // all four links
    };
    SkipListNode *head, *tail;
    int h; // Height (number of lanes)
    int n; // Number of items in list (slow lane)
    
    SkipListNode* search(const char* word, bool& found);
    void traverse();
};