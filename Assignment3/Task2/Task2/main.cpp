#include "WordList.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include "tinyxml2.h"
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
const size_t nthreads = 2;
WordList* SolutionList;

struct ParsedWords {
	vector<string> words;		// the words following restriction

	WordDirection dir;
	int size = 0;

	// restrictions
	unsigned int index[2];
	int wordId;
	
	void operator=(const ParsedWords& copy) {
		this->dir = copy.dir;
		this->index[0] = copy.index[0];
		this->index[1] = copy.index[1];
		this->size = copy.size;
		this->wordId = copy.wordId;
		for (auto& w : copy.words) {
			words.push_back(w);
		}
	}
};
vector<ParsedWords> wordSet;

// Loading from XML parameter file
bool LoadWordRestrictions(const char* filename)
{
	std::cout << "Current Working Directory =" << filename << std::endl;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError result = doc.LoadFile(filename);

	if (result != tinyxml2::XML_SUCCESS) {
		printf("ERROR: Invalid Parameter File\a\n");
		return false;
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
				temp.size = V;
				temp.dir = WordDirection::Down;
			}
			else if (H > 0) {
				temp.size = H;
				temp.dir = WordDirection::Across;
			}

			wordSet.push_back(temp);
		}
	}

	return true;
}

//void sortWordSet()
//{
//	int n = wordSet.size() - 1;
//
//	int i, j;
//	bool swapped;
//	for (i = 0; i < n - 1; i++)
//	{
//		swapped = false;
//		for (j = 0; j < n - i - 1; j++)
//		{
//			if (wordSet[j].size > wordSet[j + 1].size)
//			{
//				ParsedWords tmp = wordSet[j];
//				wordSet[j] = wordSet[j + 1];
//				wordSet[j + 1] = tmp;
//				swapped = true;
//			}
//		}
//
//		// IF no two elements were swapped by inner loop, then break 
//		if (swapped == false)
//			break;
//	}
//}

// Data filter based on word restrictions from XML
void DirectionaryFiler() {
	// Getting all words from directory
	string Directionary[21120];
	ifstream DirectionaryFile("Directionary.txt");
	string tmp; int i = 0;
	while (DirectionaryFile >> tmp) { Directionary[i] = tmp; i++; }

	for (auto& set : wordSet) {
		for (int j = 0; j < 21120; j++) {
			if (Directionary[j].length() == set.size)
				set.words.push_back(Directionary[j]);
		}
	}
}


bool Backtracking() {
	/*mutex lock;
	bool * Solved = new bool[nthreads]; for (int i = 0; i < nthreads; i++) { Solved[i] = false; }
	thread* cT = new thread[nthreads];*/

	stack<WordList*> queue;
	for (int i = 0; i < wordSet[0].words.size(); i++) {
		WordList* set = new WordList;
		set->AddWord(wordSet[0].words[i], wordSet[0].index[0], wordSet[0].index[1], wordSet[0].wordId, wordSet[0].dir);
		queue.push(set);
	}
	while (queue.size() > 0) {
		WordList * list = queue.top();	
		queue.pop();

		//// lambda function
		//auto nextLayer = [&](unsigned int start, unsigned int numIts, bool& solution) {
		//	for (int i = 0; i < numIts; i++) {
		//		if (wordSet[list->GetNumWords()].words[i + start].length() == wordSet[list->GetNumWords()].Hsize) {
		//			// Testing if new list follow strictions
		//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i + start], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
		//				wordSet[list->GetNumWords()].wordId, WordDirection::Across);

		//			if (list->Goal(newWord))
		//			{
		//				WordList* tempList = new WordList(list);
		//				tempList->AddWord(newWord);
		//				if (tempList->GetNumWords() == wordSet.size())
		//				{
		//					SolutionList = tempList;
		//					solution = true;
		//					return; //return true;
		//				}
		//				lock.lock();
		//				queue.push(tempList);
		//				lock.unlock();
		//			}
		//			else {
		//				delete newWord;
		//			}
		//		}
		//		else { //if (wordSet[list->GetNumWords()].words[i].length() == wordSet[list->GetNumWords()].Vsize) {
		//			// Testing if new list follow strictions
		//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i + start], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
		//				wordSet[list->GetNumWords()].wordId, WordDirection::Down);

		//			if (list->Goal(newWord))
		//			{
		//				WordList* tempList = new WordList(list);
		//				tempList->AddWord(newWord);
		//				if (tempList->GetNumWords() == wordSet.size())
		//				{
		//					SolutionList = tempList;
		//					solution = true;
		//					return; //return true;
		//				}
		//				lock.lock();
		//				queue.push(tempList);
		//				lock.unlock();
		//			}
		//			else {
		//				delete newWord;
		//			}
		//		}
		//	}
		//};

		//
		//unsigned int numIts = (wordSet[list->GetNumWords()].words.size() - 1) / nthreads;
		//unsigned int remainingIts = (wordSet[list->GetNumWords()].words.size() - 1) % nthreads;
		//unsigned int start = 0;
		//for (int n = 0; n < nthreads; n++) {
		//	int its = numIts + (remainingIts > 0 ? 1 + (remainingIts - (--remainingIts)) : 0);
		//	cT[n] = thread(nextLayer, start, its, ref(Solved[n]));
		//	start += its;
		//}

		//for (int n = 0; n < nthreads; n++) {
		//	cT[n].join();
		//	if (Solved[n]) return true;
		//}
		
		for (int i = 0; i < wordSet[list->GetNumWords()].words.size() - 1; i++) {
			// Testing if new list follow strictions
			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
				wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);

			if (list->Goal(newWord))
			{
				WordList* tempList = new WordList(list);
				tempList->AddWord(newWord);
				if (tempList->GetNumWords() == wordSet.size())
				{
					SolutionList = tempList;
					return true;
				}
				queue.push(tempList);
			}
			else {
				delete newWord;
			}
			
		}
		if (wordSet[list->GetNumWords()].words.size()) {
			int i = wordSet[list->GetNumWords()].words.size() - 1;
			// Testing if new list follow strictions
			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
				wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);

			if (list->Goal(newWord))
			{
				list->AddWord(newWord);
				if (list->GetNumWords() == wordSet.size())
				{
					SolutionList = list;
					return true;
				}
				queue.push(list);
			}
			else {
				delete newWord;
				delete list;
			}
		}
	}

	// No Solution Found
	return false;
}

bool Recursion_Backtracking(WordList* list) {
	for (int i = 0; i < wordSet[list->GetNumWords()].words.size() - 1; i++) {
		// Testing if new list follow strictions
		WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
			wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);

		if (list->Goal(newWord))
		{
			WordList* tempList = new WordList(list);
			tempList->AddWord(newWord);
			if (tempList->GetNumWords() == wordSet.size())
			{
				SolutionList = tempList;
				return true;
			}
			if (Recursion_Backtracking(tempList))
				return true;
		}
		else {
			delete newWord;
		}

	}
	if (wordSet[list->GetNumWords()].words.size()) {
		int i = wordSet[list->GetNumWords()].words.size() - 1;
		// Testing if new list follow strictions
		WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
			wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);

		if (list->Goal(newWord))
		{
			list->AddWord(newWord);
			if (list->GetNumWords() == wordSet.size())
			{
				SolutionList = list;
				return true;
			}
			if (Recursion_Backtracking(list)) {
				return true;
			}
		}
		else {
			delete newWord;
			delete list;
		}
	}
	return false;
}

int main() {
	LoadWordRestrictions("heartCrossword.xml");
	DirectionaryFiler();

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	WordList list;
	if (Backtracking()) {
		SolutionList->PrintPuzzle();
	}
	else {
		printf("NO SOLUTION\n\a"); fflush(stdout);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "\nTime difference = " << ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) * 1e-6 << "[s]" << std::endl;

	return 0;
}