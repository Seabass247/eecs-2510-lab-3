#include <iostream>
#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"
#include <fstream>
#include <time.h>

// Returns the execution time of an operation that begins at 'start'
// and ends at 'end' by calculating the difference between the two as
// clock cycles taken during the period, and the cycles  are converted to secs by dividing
// by CLOCKS_PER_SEC.
double getExecutionTime(clock_t start, clock_t end)
{
	return double(end - start) / double(CLOCKS_PER_SEC);
}

void parse()
{
	char c;
	RBT* RBT_T = new RBT(); // instantiate each of the trees
	AVL* AVL_T = new AVL(); //
	BST* BST_T = new BST(); //
	SkipList* SL = new SkipList(); // and the skip list
	char chari[50]; // assumes no word is longer than 49 characters
	memset(chari, 0, 50); // zero the word buffer
	int iPtr;
	ifstream inFile;

	clock_t start, end;
	
	double overheadTime = 0;
	for (int pass = 0; pass < 3; pass++)
	{
		start = clock();
		bool IsDelimiter = false, WasDelimiter = false;
		inFile.open("Shakespeare.txt", ios::binary);
		if (inFile.fail())
		{
			cout << "Unable to open input file\n\n"
				<< "Program Exiting\n\nPress ENTER to exit\n";
			cin.get(c);
			exit(1);
		}
		iPtr = 0;
		inFile.get(c); // priming read
		while (!inFile.eof())
		{
			IsDelimiter = (c == ' ' || c == 10 || c == 13 || c == ',' || c == '.' || c == '\'' ||
				c == ':' || c == ';' || c == '"' || c == '?' || c == '!' || c == '-' ||
				c == '(' || c == ')' || c == '[' || c == ']' || c == '_' || c == '*' || c == 9);
			if (IsDelimiter && !WasDelimiter) // if THIS character IS a delimiter, and the
			// last one WASN’T, it’s the end of a word
			{
				WasDelimiter = true;
				//cout << "Inserting word" << endl;
				if (pass == 2) RBT_T->Insert(chari); // insert this word in the RBT
				else if (pass == 3) AVL_T->Insert(chari); // insert it in the AVL Tree
				else if (pass == 4) BST_T->Insert(chari); // insert it in the BST
				else if (pass == 5) SL->Insert(chari); // insert it in the skip list
				memset(chari, 0, 50); // zero the word buffer
				iPtr = 0;
			}
			else if (!IsDelimiter) chari[iPtr++] = c; // if this isn’t a delimiter, keep going
			WasDelimiter = IsDelimiter; // for the NEXT iteration
			inFile.get(c);
		}
		inFile.close();
		end = clock();
		// If the file doesn't end with a delimiter, it will leave the last word unprocessed.
		// Insert once more, and display the statistics for this structure...
		//
		if (pass == 2) { if (strlen(chari)) RBT_T->Insert(chari); RBT_T->DisplayStatistics(); } // RBT
		else if (pass == 3) { if (strlen(chari)) AVL_T->Insert(chari); AVL_T->DisplayStatistics(); } // AVL
		else if (pass == 4) { if (strlen(chari)) BST_T->Insert(chari); BST_T->DisplayStatistics(); } // BST
		else if (pass == 5) { if (strlen(chari)) SL->Insert(chari); SL->DisplayStatistics(); } // skip list

		if (pass == 1)
			overheadTime = getExecutionTime(start, end);
		else if (pass > 1)
			cout << "Elapsed time: " << getExecutionTime(start, end) - overheadTime << " seconds" << endl;
	}

}

int main()
{
	parse();
	/*
	AVL* AVL_T = new AVL();
	AVL_T->Insert("A");
	AVL_T->Insert("B");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("C");
	AVL_T->Insert("D");
	AVL_T->Insert("E");
	//AVL_T->List();
	AVL_T->DisplayStatistics();

	cout << endl;

	BST* BST_T = new BST();
	BST_T->Insert("A");
	BST_T->Insert("B");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("C");
	BST_T->Insert("D");
	BST_T->Insert("E");
	//BST_T->List();
	BST_T->DisplayStatistics();

	cout << endl;

	RBT* RBT_T = new RBT();
	RBT_T->Insert("E");
	RBT_T->Insert("D");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("C");
	RBT_T->Insert("A");
	RBT_T->Insert("B");
	//RBT_T->List();
	RBT_T->DisplayStatistics();

	cout << endl;

	SkipList* SL = new SkipList();
	SL->Insert("A");
	SL->Insert("A");
	SL->Insert("B");
	SL->Insert("C");
	SL->Insert("D");
	SL->Insert("E");
	SL->Insert("F");
	SL->Insert("G");
	SL->Insert("H");
	SL->Insert("I");
	SL->Insert("J");
	SL->Insert("K");
	SL->Insert("L");
	SL->Insert("M");
	SL->Insert("N");
	SL->Insert("O");
	SL->Insert("O");
	SL->Insert("P");
	SL->Insert("Q");
	SL->Insert("R");
	SL->Insert("S");
	SL->Insert("T");
	SL->Insert("U");
	SL->Insert("V");
	SL->Insert("V");
	SL->Insert("V");
	SL->Insert("V");
	//SL->List();
	SL->DisplayStatistics();
	cout << endl;
	*/

	
}