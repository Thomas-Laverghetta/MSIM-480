#include "WordList.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include "tinyxml2.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include <thread>

using namespace std;

struct ParsedWords {
	vector<string> words;		// the words following restriction

	/*WordDirection dir;*/
	int Vsize = 0;
	int Hsize = 0;

	// restrictions
	/*WordDirection dir;*/
	unsigned int index[2];
	int wordId;

	// size restrictions
	/*vector<int> sizes;*/
};

// Loading from XML parameter file
vector<ParsedWords> LoadWordRestrictions(const char* filename)
{
	vector<ParsedWords> wordRestrictions;
	std::cout << "Current Working Directory =" << filename << std::endl;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError result = doc.LoadFile(filename);

	if (result != tinyxml2::XML_SUCCESS) {
		printf("ERROR: Invalid Parameter File\a\n");
		exit(0);
	}

	tinyxml2::XMLElement* pRoot = doc.RootElement();

	for (tinyxml2::XMLElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
	{
		std::string elementName = pElem->Value();

		if (elementName == "word") {
			ParsedWords temp;
			/*const char* stateName;
			pElem->QueryStringAttribute("name", &stateName);*/
				
			pElem->QueryAttribute("wordId", &temp.wordId);	
			pElem->QueryAttribute("row", &temp.index[0]);
			pElem->QueryAttribute("col", &temp.index[1]);


			int V, H;
			pElem->QueryAttribute("VSize", &V);
			pElem->QueryAttribute("HSize", &H);

			if (V > 0) {
				temp.Vsize = V;
			}
			else if (H > 0) {
				temp.Hsize = H;
			}

			wordRestrictions.push_back(temp);
		}
	}

	return wordRestrictions;
}

// Data filter based on word restrictions from XML
vector<ParsedWords> DirectionaryFiler(vector<ParsedWords>& wordRestrictions) {
	// Getting all words from directory
	string Directionary[21120];
	ifstream DirectionaryFile("Directionary.txt");
	string tmp; int i = 0;
	while (DirectionaryFile >> tmp) { Directionary[i] = tmp; i++; }

	for (auto& set : wordRestrictions) {
		for (int j = 0; j < 21120; j++) {
			if (Directionary[j].length() == set.Hsize || Directionary[j].length() == set.Vsize)
				set.words.push_back(Directionary[j]);
		}
	}

	return wordRestrictions;
}

WordList * SolutionList;
bool Backtracking(vector<ParsedWords>& wordSet) {
	stack<WordList*> queue;
	for (int i = 0; i < wordSet[0].words.size(); i++) {
		if (wordSet[0].words[i].length() == wordSet[0].Hsize) {
			WordList* set = new WordList;
			set->AddWord(wordSet[0].words[i], wordSet[0].index[0], wordSet[0].index[1], wordSet[0].wordId, WordDirection::Across);
			queue.push(set);
		}
		else{ //if (wordSet[0].words[i].length() == wordSet[0].Vsize) {
			WordList* set = new WordList;
			set->AddWord(wordSet[0].words[i], wordSet[0].index[0], wordSet[0].index[1], wordSet[0].wordId, WordDirection::Down);
			queue.push(set);
		}
	}
	while (queue.size() > 0) {
		WordList * list = queue.top();
		for (int i = 0; i < wordSet[list->GetNumWords()].words.size() - 1; i++) {
			if (wordSet[list->GetNumWords()].words[i].length() == wordSet[list->GetNumWords()].Hsize) {
				WordList* tempList = new WordList(list);
				tempList->AddWord(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
					wordSet[list->GetNumWords()].wordId, WordDirection::Across);

				// Testing if new list follow strictions
				if (tempList->Goal()) 
				{
					if (tempList->GetNumWords() == wordSet.size())
					{
						SolutionList = tempList;
						return true;
					}
					queue.push(tempList);
				}
				else {
					delete tempList;
				}
			}
			else { //if (wordSet[list->GetNumWords()].words[i].length() == wordSet[list->GetNumWords()].Vsize) {
				WordList* tempList = new WordList(list);
				tempList->AddWord(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
					wordSet[list->GetNumWords()].wordId, WordDirection::Down);

				// Testing if new list follow strictions
				if (tempList->Goal())
				{
					if (tempList->GetNumWords() == wordSet.size())
					{
						SolutionList = tempList;
						return true;
					}
					queue.push(tempList);
				}
				else {
					delete tempList;
				}
			}
		}
		if (wordSet[list->GetNumWords()].words.size()) {
			int i = wordSet[list->GetNumWords()].words.size() - 1;
			if (wordSet[list->GetNumWords()].words[i].length() == wordSet[list->GetNumWords()].Vsize) {
				list->AddWord(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
					wordSet[list->GetNumWords()].wordId, WordDirection::Down);
				// Testing if new list follow strictions
				if (list->Goal())
				{
					if (list->GetNumWords() == wordSet.size())
					{
						SolutionList = list;
						return true;
					}
				}
				else {
					queue.pop();
					delete list;
				}
			}
			else {
				list->AddWord(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
					wordSet[list->GetNumWords()].wordId, WordDirection::Across);
				// Testing if new list follow strictions
				if (list->Goal())
				{
					if (list->GetNumWords() == wordSet.size())
					{
						SolutionList = list;
						return true;
					}
				}
				else {
					queue.pop();
					delete list;
				}
			}
			continue;
		}
	}

	// No Solution Found
	return false;
}

int main() {
	// vector<ParsedWords> wordSet = ParseFile("test.csv");
	
	vector<ParsedWords> wordRestriction = LoadWordRestrictions("heartCrossword.xml");
	wordRestriction = DirectionaryFiler(wordRestriction);
	if (Backtracking(wordRestriction)) {
		SolutionList->PrintPuzzle();
	}
	else {
		printf("NO SOLUTION\n\a"); fflush(stdout);
	}

	return 0;
}