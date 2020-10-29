//#include "WordList.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include "tinyxml2.h"
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <unordered_map>

enum WordDirection {
	Across,
	Down,
	ANY
};

using namespace std;
const size_t nthreads = 2;
//WordList* SolutionList;

struct ParsedWords {
	//vector<string> words;		// the words following restriction

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
		/*for (auto& w : copy.words) {
			words.push_back(w);
		}*/
	}
};

struct Intersect {
	int wordId;
	int thisWordIndex;
	int otherWordIndex;
};
// wordId, intersection
unordered_map<int, vector<Intersect>> IntersectionMap;

struct WordStates {
	int wordId;
	vector<string> words;
};


// Loading from XML parameter file
vector<ParsedWords> LoadWordRestrictions(const char* filename)
{
	vector<ParsedWords> wordSet;
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

	return wordSet;
}

void ItersectionFinder(vector<ParsedWords>& wordSet) {
	for (auto& w1: wordSet){
		for (auto& w2 : wordSet) {
			if (w1.wordId == w2.wordId)
				continue;

			unsigned int endIndex1[2], endIndex2[2];
			if (w1.dir == WordDirection::Across) {
				endIndex1[0] = w1.index[0];
				endIndex1[1] = w1.index[1] + w1.size - 1;
			}
			else {
				endIndex1[1] = w1.index[1];
				endIndex1[0] = w1.index[0] + w1.size - 1;
			}
			if (w2.dir == WordDirection::Across) {
				endIndex2[0] = w2.index[0];
				endIndex2[1] = w2.index[1] + w2.size - 1;
			}
			else {
				endIndex2[1] = w2.index[1];
				endIndex2[0] = w2.index[0] + w2.size - 1;
			}
			

			int A1 = (endIndex1[1] - w1.index[1]);
			int A2 = (endIndex2[1] - w2.index[1]);
			int B1 = (w1.index[0] - endIndex1[0]);
			int B2 = (w2.index[0] - endIndex2[0]);

			// calculating the determinet to determine parallelism:
			// A = y2 - y1, B = x1 - x2
			int det = A1 * B2 - A2 * B1;

			// if det does't equal zero, then not parallel (intersection)
			if (det != 0) {
				// C = A*x1 + B*y
				int C1 = A1 * w1.index[0] + B1 * w1.index[1];
				int C2 = A2 * w2.index[0] + B2 * w2.index[1];

				// intersection point
				int row = ((float)(B2 * C1 - B1 * C2)) / ((float)det);
				int col = ((float)(A1 * C2 - A2 * C1)) / ((float)det);

				// verifying intersection is within domain
				if (w1.index[0] <= row && row <= endIndex1[0] && w2.index[0] <= row && row <= endIndex2[0]
					&& w1.index[1] <= col && col <= endIndex1[1] && w2.index[1] <= col && col <= endIndex2[1]) {
					// Finding String index points to compare from
					int wordIndex1, wordIndex2;

					// first word's index point
					if (w1.dir == WordDirection::Across) {
						wordIndex1 = col - w1.index[1];
					}
					else {
						wordIndex1 = row - w1.index[0];
					}

					// second word's index point
					if (w2.dir == WordDirection::Across) {
						wordIndex2 = col - w2.index[1];
					}
					else {
						wordIndex2 = row - w2.index[0];
					}

					Intersect sect;
					sect.wordId = w2.wordId;
					sect.thisWordIndex = wordIndex2;
					sect.otherWordIndex = wordIndex2;

					// mapping intersection
					IntersectionMap[w1.wordId].push_back(sect);

					//// comparing words
					//if (it1->_word[wordIndex1] != it2->_word[wordIndex2]) {
					//	// conflict

					//}
				}
			}
		}
	}
}

// Data filter based on word restrictions from XML
vector<WordStates> DirectionaryFiler(vector<ParsedWords>& wordSet) {
	// Getting all words from directory
	string Directionary[21120];
	ifstream DirectionaryFile("Directionary.txt");
	string tmp; int i = 0;
	while (DirectionaryFile >> tmp) { Directionary[i] = tmp; i++; }

	vector<WordStates> wordStates(wordSet.size());

	for (int i = 0; i < wordStates.size(); i++) {
		for (int j = 0; j < 21120; j++) {
			if (Directionary[j].length() == wordSet[i].size)
				wordStates[i].words.push_back(Directionary[j]);
		}
	}
	return wordStates;
}

struct Word {
	string word;
	int wordId;
};
vector<WordStates> Query(vector<Word> currWords, vector<WordStates> otherWords) {
	vector<WordStates> newStates = otherWords;

}

//bool Backtracking() {
//	/*mutex lock;
//	bool * Solved = new bool[nthreads]; for (int i = 0; i < nthreads; i++) { Solved[i] = false; }
//	thread* cT = new thread[nthreads];*/
//
//	stack<WordList*> queue;
//	for (int i = 0; i < wordSet[0].words.size(); i++) {
//		WordList* set = new WordList;
//		set->AddWord(wordSet[0].words[i], wordSet[0].index[0], wordSet[0].index[1], wordSet[0].wordId, wordSet[0].dir);
//		queue.push(set);
//	}
//	while (queue.size() > 0) {
//		WordList * list = queue.top();	
//		queue.pop();
//
//		//// lambda function
//		//auto nextLayer = [&](unsigned int start, unsigned int numIts, bool& solution) {
//		//	for (int i = 0; i < numIts; i++) {
//		//		if (wordSet[list->GetNumWords()].words[i + start].length() == wordSet[list->GetNumWords()].Hsize) {
//		//			// Testing if new list follow strictions
//		//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i + start], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//		//				wordSet[list->GetNumWords()].wordId, WordDirection::Across);
//
//		//			if (list->Goal(newWord))
//		//			{
//		//				WordList* tempList = new WordList(list);
//		//				tempList->AddWord(newWord);
//		//				if (tempList->GetNumWords() == wordSet.size())
//		//				{
//		//					SolutionList = tempList;
//		//					solution = true;
//		//					return; //return true;
//		//				}
//		//				lock.lock();
//		//				queue.push(tempList);
//		//				lock.unlock();
//		//			}
//		//			else {
//		//				delete newWord;
//		//			}
//		//		}
//		//		else { //if (wordSet[list->GetNumWords()].words[i].length() == wordSet[list->GetNumWords()].Vsize) {
//		//			// Testing if new list follow strictions
//		//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i + start], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//		//				wordSet[list->GetNumWords()].wordId, WordDirection::Down);
//
//		//			if (list->Goal(newWord))
//		//			{
//		//				WordList* tempList = new WordList(list);
//		//				tempList->AddWord(newWord);
//		//				if (tempList->GetNumWords() == wordSet.size())
//		//				{
//		//					SolutionList = tempList;
//		//					solution = true;
//		//					return; //return true;
//		//				}
//		//				lock.lock();
//		//				queue.push(tempList);
//		//				lock.unlock();
//		//			}
//		//			else {
//		//				delete newWord;
//		//			}
//		//		}
//		//	}
//		//};
//
//		//
//		//unsigned int numIts = (wordSet[list->GetNumWords()].words.size() - 1) / nthreads;
//		//unsigned int remainingIts = (wordSet[list->GetNumWords()].words.size() - 1) % nthreads;
//		//unsigned int start = 0;
//		//for (int n = 0; n < nthreads; n++) {
//		//	int its = numIts + (remainingIts > 0 ? 1 + (remainingIts - (--remainingIts)) : 0);
//		//	cT[n] = thread(nextLayer, start, its, ref(Solved[n]));
//		//	start += its;
//		//}
//
//		//for (int n = 0; n < nthreads; n++) {
//		//	cT[n].join();
//		//	if (Solved[n]) return true;
//		//}
//		
//		for (int i = 0; i < wordSet[list->GetNumWords()].words.size() - 1; i++) {
//			// Testing if new list follow strictions
//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//				wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);
//
//			if (list->Goal(newWord))
//			{
//				WordList* tempList = new WordList(list);
//				tempList->AddWord(newWord);
//				if (tempList->GetNumWords() == wordSet.size())
//				{
//					SolutionList = tempList;
//					return true;
//				}
//				queue.push(tempList);
//			}
//			else {
//				delete newWord;
//			}
//			
//		}
//		if (wordSet[list->GetNumWords()].words.size()) {
//			int i = wordSet[list->GetNumWords()].words.size() - 1;
//			// Testing if new list follow strictions
//			WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//				wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);
//
//			if (list->Goal(newWord))
//			{
//				list->AddWord(newWord);
//				if (list->GetNumWords() == wordSet.size())
//				{
//					SolutionList = list;
//					return true;
//				}
//				queue.push(list);
//			}
//			else {
//				delete newWord;
//				delete list;
//			}
//		}
//	}
//
//	// No Solution Found
//	return false;
//}
//
//bool Recursion_Backtracking(WordList* list) {
//	for (int i = 0; i < wordSet[list->GetNumWords()].words.size() - 1; i++) {
//		// Testing if new list follow strictions
//		WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//			wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);
//
//		if (list->Goal(newWord))
//		{
//			WordList* tempList = new WordList(list);
//			tempList->AddWord(newWord);
//			if (tempList->GetNumWords() == wordSet.size())
//			{
//				SolutionList = tempList;
//				return true;
//			}
//			if (Recursion_Backtracking(tempList))
//				return true;
//		}
//		else {
//			delete newWord;
//		}
//
//	}
//	if (wordSet[list->GetNumWords()].words.size()) {
//		int i = wordSet[list->GetNumWords()].words.size() - 1;
//		// Testing if new list follow strictions
//		WordList::Word* newWord = new WordList::Word(wordSet[list->GetNumWords()].words[i], wordSet[list->GetNumWords()].index[0], wordSet[list->GetNumWords()].index[1],
//			wordSet[list->GetNumWords()].wordId, wordSet[list->GetNumWords()].dir);
//
//		if (list->Goal(newWord))
//		{
//			list->AddWord(newWord);
//			if (list->GetNumWords() == wordSet.size())
//			{
//				SolutionList = list;
//				return true;
//			}
//			if (Recursion_Backtracking(list)) {
//				return true;
//			}
//		}
//		else {
//			delete newWord;
//			delete list;
//		}
//	}
//	return false;
//}

int main() {
	vector<ParsedWords> wordSet = LoadWordRestrictions("heartCrossword.xml");
	vector<WordStates> states = DirectionaryFiler(wordSet);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	if () {
		
	}
	else {
		printf("NO SOLUTION\n\a"); fflush(stdout);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "\nTime difference = " << ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) * 1e-6 << "[s]" << std::endl;

	return 0;
}