#include <iostream>
#include "AVL.h"
#include "BST.h"
#include "RBT.h"
#include "SkipList.h"
#include <fstream>
#include <time.h>

const string DIRECTORY = "C:\\Users\\Sebastian\\Documents\\eecs2510lab3\\variety\\";

const string FILENAME = "Shakespeare.txt"; // The name of the file the parser will take input from.

// Returns the execution time of an operation that begins at 'start'
// and ends at 'end' by calculating the difference between the two as
// clock cycles taken during the period, and the cycles  are converted to secs by dividing
// by CLOCKS_PER_SEC.
double getExecutionTime(clock_t start, clock_t end)
{
	return double(end - start) / double(CLOCKS_PER_SEC);
}

void parse(string filename)
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
	cout << "INPUT_FILE=\"" << filename << "\"" << endl << endl;
	for (int pass = 0; pass < 6; pass++)
	{
		start = clock();
		bool IsDelimiter = false, WasDelimiter = false;
		inFile.open(filename, ios::binary);
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
		{
			cout << "Elapsed time: " << getExecutionTime(start, end) - overheadTime << " seconds" << endl;
			cout << endl;
		}
	}

}

int main(int argc, char* argv[])
{
	// Parse the file as supplied to the program as the first argument...
	if (argc == 2)
	{
		string file = (string)argv[1];
		parse(file);
	} // ...else use the constant filename and path string provided at the top
	else
	{
		parse(DIRECTORY + FILENAME);
	}
}