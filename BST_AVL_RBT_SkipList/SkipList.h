// SkipList.h
// Sebastian Hamel
// EECS 2510, Spring 2020
// 04/03/2020
// Contains the definitions for the SkipList class and its internal node structure
#pragma once
#include <random>

using namespace std;

class SkipList
{
public:
    SkipList();

    void Insert(const char* word);
    void List();
    int getListHeight() { return h; };
    void DisplayStatistics();
private:
    int statKeyComparison = 0;
    int statPointerChange = 0;
    int statTossedHeads = 0;
    int statTossedCoin = 0;
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