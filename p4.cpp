// Jason Wang
// File: p4.cpp
// Date: 11/17/2020
// Purpose: The program uses a Hash Table to efficiently solve the following
// problem: Given a natural language text, generate a table of distinct words
// with the number of occurrences of each word in the text.

#include "WordCounter.h"
#include "CommonWordList.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

string cleanWord(string);
//  Takes in a word and converts it to lower case and removes any special
//  characters that are not hyphen or apostrophe.

void removeCommonWords(WordCounter &);
// Remove common words from input the contents

void testLoop(int &);
// Execute whole test program one time

void testFile(string &, int &, int &);
// Display the test message and prompt for file name and a capacity

void readInFile(WordCounter &, const string&);
// Read in contents from a file

void statistics(WordCounter &);
// Display the statistics of the contents' unique, total, and load F. numbers

void promptForRemovedWords(string &);
// Prompt for removed words

void analysis(WordCounter &, string&);
// Display removed words and its frequencies and them remove them.

const char APOSTROPHE_CHAR = '\''; 	// apostrophe character used in cleanWord
const char HYPHEN_CHAR = '-'; 		// hyphen/dash character used in cleanWord

int main() {
    int testTime = 0;

    // populate common words
    CommonWordList::populateWords();

    // test 1
    testLoop(testTime);

    // test 2
    testLoop(testTime);

    return 0;
}

void testLoop(int & testTime){
    string fileName, RemovedWord;    // file name and removed words
    int capacity;                    // capacity of hash table

    testFile(fileName, capacity, testTime);

    WordCounter hashTable(capacity);        // declare a WordCounter class

    readInFile(hashTable, fileName);

    removeCommonWords(hashTable);

    WordCounter hashTable2(hashTable);      // test copy constructor

    statistics(hashTable);

    promptForRemovedWords(RemovedWord);

    analysis(hashTable2, RemovedWord);

    WordCounter hashTable3 = hashTable2;    // test copy assignment operator

    statistics(hashTable3);
}

void testFile(string & fileName, int & capacity, int & testTime){
    testTime++;

    cout << "\n*********************\n"
            "*   Test file # " << testTime << ":  *\n"
            "*********************\n\n";

    // prompt for a file name
    cout << "What is the filename: ";
    cin >> fileName;

    // prompt for the capacity of hash table
    cout << "What is the capacity: ";
    cin >> capacity;
}

void statistics(WordCounter & hashTable){
    double loadF = hashTable.getUniqueWordCount() / hashTable.getCapacity();

    cout << "\nWord Counter statistics:\n"
            "  #Unique   #Total   Load Factor \n"
            "+---------+--------+-----------+\n";

    // display unique, total, and load factor numbers
    cout << setw(7) << (int)hashTable.getUniqueWordCount();
    cout << setw(10) << (int)hashTable.getTotalWordCount();
    cout << setw(12) << fixed << setprecision(3) << loadF << endl;
}

void promptForRemovedWords(string & RemovedWord){
    cout << "\nEnter words (separated by space): ";

    // ignore blank line
    cin.ignore();

    getline(cin,RemovedWord);
}

void analysis(WordCounter & hashTable, string & RemovedWord){
    stringstream ss(RemovedWord);
    string token;

    cout << "\nAnalysis of words:\n"
            "  Given Word   Count \n"
            "+------------+-------+\n";

    // display the removed words and their frequencies
    while(getline(ss,token,' ')){
        cout << setw(10) << token << setw(8) <<
                setprecision(0) << hashTable.getWordCount(token) << endl;

        hashTable.removeWord(token);
    }

    cout << "\nRemove the following words: " << RemovedWord << endl;
}

void readInFile(WordCounter & hashTable, const string& fileName){
    string allString, strLine, newStr;
    stringstream ss;
    ifstream inFile;

    // open the file
    inFile.open(fileName.c_str());
    if (!inFile.is_open()) {
        cout << "ERROR loading file!\n";
    }

    // read each line from the file
    while(getline(inFile,strLine)) {
        if (strLine[strLine.length() - 1] != '-')
            strLine += " ";

        ss << strLine;
    }

        // add words into hashTable
    while(getline(ss,newStr, ' ')) {
        if (!newStr.empty() && newStr != "-") {
            hashTable.addWord(cleanWord(newStr));
        }
    }

    // close file
    inFile.close();
}

string cleanWord(string s) {
    string result = "";                  // Keep track of the cleaned result.
    unsigned i = 0, length = s.length(); // Input for the for loop.
    for (; i < length; i++) {
        if (isalnum(s[i]))
            result += tolower(s[i]);
        else if (s[i] == APOSTROPHE_CHAR) {
            if (i > 0 && isalnum(s[i-1]))
                result += s[i];
        } else if (s[i] == HYPHEN_CHAR) {
            if (i > 0 && i < (length-1) && isalnum(s[i-1]) &&
                                                            isalnum(s[i+1]))
                result += s[i];
                // If a word ends in a hyphen, we assume it is residue that
                // needs to be paired with the next word. Return the word with
                // the hyphen as a signal to the caller to retain it as residue.
            else if (i > 0 && i == (length-1) && isalnum(s[i-1]))
                result += s[i];
        }
    }
    return result;
}

void removeCommonWords(WordCounter &wc) {
    for (string &s : CommonWordList::getWords())
        wc.removeWord(s);
}