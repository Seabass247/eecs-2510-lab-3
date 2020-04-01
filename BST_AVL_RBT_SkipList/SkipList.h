#pragma once
#include <random>

using namespace std;

class SkipList
{
public:
    SkipList();

    void Insert(char X[50]);
    void List();
    int getListHeight() { return h; };
    void DisplayStatistics();
private:
    int statKeyComparison = 0;
    int statPointerChange = 0;
    int debugSearchDepth = 0;
    mt19937 coin;

    const char* NEG_INF = "negInf";
    const char* POS_INF = "posInf";

    struct SkipListNode
    {
        char key[50] = { 0 };
        int count = 1;
        SkipListNode* up, * down, * left, * right; // all four links
    };
    SkipListNode *head, *tail;
    int h; // Height (number of lanes)
    int n; // Number of items in list (slow lane)
    
    SkipListNode* search(const char* word, bool& found);
    void traverse(int& distinctNodes, int& totalNodes, int& totalWords, int* nodesInLevel);
};