//#include "WordList.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <string>
#include <chrono>
#include <unordered_map>
#include "WordList.hpp"

using namespace std;

// direction of word in crossword
enum WordDirection {
	Across,
	Down
};
struct ParsedWords {
	WordDirection dir;	// direction of words in set
	int size = 0;		// size of word

	// restrictions
	unsigned int index[2];	// location of word
	int wordId;				// word identifier
	
	void operator=(const ParsedWords& copy) {
		this->dir = copy.dir;
		this->index[0] = copy.index[0];
		this->index[1] = copy.index[1];
		this->size = copy.size;
		this->wordId = copy.wordId;
	}
};

// intersection indexing
struct Intersect {
	Intersect(int wordID, int wordIndex, int originWordindex)
		: wordId(wordID), wordIndex(wordIndex), originWordIndex(originWordindex) {}
	int wordId;				// word id 
	int wordIndex;
	int originWordIndex;
};
// wordId, intersection
unordered_map<int, vector<Intersect>> IntersectionMap;


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

					/*Intersect sect;
					sect.wordId = w2.wordId;
					sect.thisWordIndex = wordIndex2;
					sect.otherWordIndex = wordIndex2;*/

					// mapping intersection
					IntersectionMap[w1.wordId].push_back(Intersect(w2.wordId, wordIndex2, wordIndex1));
				}
			}
		}
	}
}

// Data filter based on word restrictions from XML
vector<WordElementSet> DirectionaryFiler(vector<ParsedWords>& wordSet) {
	// Getting all words from directory
	string Directionary[21120];
	ifstream DirectionaryFile("Directionary.txt");
	string tmp; int i = 0;
	while (DirectionaryFile >> tmp) { Directionary[i] = tmp; i++; }

	vector<WordElementSet> wordStates(wordSet.size());

	for (int i = 0; i < wordStates.size(); i++) {
		wordStates[i].wordId = wordSet[i].wordId;
		for (int j = 0; j < 21120; j++) {
			if (Directionary[j].length() == wordSet[i].size)
				wordStates[i].words.push_back(Directionary[j]);
		}
	}
	return wordStates;
}


bool NextWordSet(const Word& newWord, vector<WordElementSet>& wordSet, WordElementSet& nextWords) {
	vector<WordElementSet> tmpSet;

	// finding intersecting word elements then determining if element set contains word that conforms with restrictions
	for (auto& sect : IntersectionMap[newWord.wordId]) {
		// determining if intersect exists
		auto it = find_if(wordSet.begin(), wordSet.end(), [&sect](const WordElementSet& obj) { return obj.wordId == sect.wordId; });
		if (it != wordSet.end()) {
			WordElementSet newState;
			newState.wordId = it->wordId;
			for (auto& word : it->words) {
				// checking intersection point between new word and this word
				if (word[sect.wordIndex] == newWord.word[sect.originWordIndex])
					newState.words.push_back(word);
			}
			// determining if anywords conformed to restrictions
			if (newState.words.size() > 0)
				tmpSet.push_back(newState);
			else // no word conformed
				return false;
		}
	}

	// if all intersections are in crossword
	if (tmpSet.size() == 0) {
		// bubble sort for number of solutions
		bool swapped;
		for (int i = wordSet.size() - 1; i >= 0; i--)
		{
			swapped = false;
			for (int j = wordSet.size() - 1; j > wordSet.size() - i - 1; j--)
			{
				if (wordSet[j].words.size() > wordSet[j - 1].words.size())
				{
					WordElementSet tmp = wordSet[j];
					wordSet[j] = wordSet[j - 1];
					wordSet[j - 1] = tmp;
					swapped = true;
				}
			}

			// IF no two elements were swapped by inner loop, then break 
			if (swapped == false)
				break;
		}


		// get the last element and set it to next word
		nextWords = wordSet.back();
		wordSet.pop_back();			// remove last element
		return true;
	}

	// bubble sort for number of solutions
	bool swapped;
	for (int i = 0; i < tmpSet.size() - 1; i++)
	{
		swapped = false;
		for (int j = 0; j < tmpSet.size() - i - 1; j++)
		{
			if (tmpSet[j].words.size() > tmpSet[j + 1].words.size())
			{
				WordElementSet tmp = tmpSet[j];
				tmpSet[j] = tmpSet[j + 1];
				tmpSet[j + 1] = tmp;
				swapped = true;
			}
		}

		// IF no two elements were swapped by inner loop, then break 
		if (swapped == false)
			break;
	}

	// set next element and remove same element from set
	nextWords = tmpSet[0];
	wordSet.erase(find_if(wordSet.begin(), wordSet.end(), [&tmpSet](const WordElementSet& obj) { return obj.wordId == tmpSet[0].wordId; }));

	// replacing old words with new words (filtered words)
	for (int i = 1; i < tmpSet.size(); i++) {
		auto it = find_if(wordSet.begin(), wordSet.end(), [&tmpSet, &i](const WordElementSet& obj) { return obj.wordId == tmpSet[i].wordId; });
		(*it) = tmpSet[i];
	}

	return true;
}

bool Backtracking(WordList& currWords, vector<WordElementSet> wordSet) {
	WordElementSet nextWords;
	if (NextWordSet(*currWords.top(), wordSet, nextWords)) {
		Word newWord;
		newWord.wordId = nextWords.wordId;
		currWords.AddWord(newWord);
		for (auto& word : nextWords.words) {
			currWords.top()->word = word;
			if (wordSet.size() == 0) {
				return true;
			}
			if (Backtracking(currWords, wordSet))
				return true;
		}
		// remove top word
		currWords.pop();
	}
	return false;
}

int main() {
	// parsing words from XML file
	vector<ParsedWords> parsedWords = LoadWordRestrictions("heartCrossword.xml");

	// populating intersection map
	ItersectionFinder(parsedWords);
	
	// getting words
	vector<WordElementSet> wordSet = DirectionaryFiler(parsedWords);

	// starting timer
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	// Getting first word set to iterate
	WordElementSet startingWords = wordSet.back();
	wordSet.pop_back();
	
	WordList set; Word initWord; initWord.wordId = startingWords.wordId;
	set.AddWord(initWord);
	for (auto& word : startingWords.words) {
		set.top()->word = word;
		if (Backtracking(set, wordSet))
			break;
	}

	// stopping timer
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	if (set.size() > 1) {
		set.Print();
	}
	else {
		printf("NO SOLUTION\n\a");
	}
	fflush(stdout);


	std::cout << "\nTime difference = " << ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) * 1e-6 << "[s]" << std::endl;

	return 0;
}