#include "WordQueue.h"
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

struct ParsedWords {
	vector<string> words;
	WordDirection dir;
	unsigned int index[2];
	unsigned int wordId;
};

WordList * SolutionList;
bool Backtracking(vector<ParsedWords>& wordSet) {
	
	WordQueue queue;
	for (int i = 0; i < wordSet[0].words.size(); i++) {
		WordList set;
		set.AddWord(wordSet[0].words[i], wordSet[0].index[0], wordSet[0].index[1], wordSet[0].wordId, wordSet[0].dir);
		queue.Push(set);
	}
	while (queue.Size() > 0) {
		WordList list = queue.Pop();
		if (list.Goal()) {
			if (list.GetNumWords() == wordSet.size()) {
				SolutionList = &list;
				return true;
			}
			else {
				for (int i = 0; i < wordSet[list.GetNumWords()].words.size(); i++) {
					WordList tempList(list);
					tempList.AddWord(wordSet[list.GetNumWords()].words[i], wordSet[list.GetNumWords()].index[0], wordSet[list.GetNumWords()].index[1],
						wordSet[list.GetNumWords()].wordId, wordSet[list.GetNumWords()].dir);
					queue.Push(tempList);
				}
			}
		}
	}

	// No Solution Found
	return false;
}


vector<ParsedWords> ParseFile(string filename) {
	/* 
	Files are formatted with:
	word set Id, Direction (V == vertical or H == Horizotel), row (1 to N), col (1 to M), words (each word seperated by comma)
	*/
	
	vector<ParsedWords> wordSet;

	ifstream file(filename);
	string s;
	string delimiter = ",";
	while (getline(file, s)) {
		ParsedWords tempSet;
		size_t pos = 0;
		string token;
		int i = 0;
		while ((pos = s.find(delimiter)) != string::npos) {
			token = s.substr(0, pos);
			
			switch (i)
			{
			case 0: // word id
				tempSet.wordId = stoi(token);
				break;
			case 1: // direction
				if (token == "V")
					tempSet.dir = WordDirection::Across;
				else // H
					tempSet.dir = WordDirection::Down;
				break;
			case 2:	// row
				tempSet.index[0] = stoi(token);
				break;
			case 3:	// col
				tempSet.index[1] = stoi(token);
				break;
			default: // words
				tempSet.words.push_back(token);
				break;
			}
			s.erase(0, pos + delimiter.length());
			i++;
		}
		tempSet.words.push_back(s);
		wordSet.push_back(tempSet);
	}

	return wordSet;
}

int main() {
	vector<ParsedWords> wordSet = ParseFile("test.csv");
	Backtracking(wordSet);

	return 0;
}