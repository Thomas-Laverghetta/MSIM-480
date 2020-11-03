/* 
* MSIM 480 - Assignment 3
* Creator: Thomas J Laverghetta
* 
* Linked List for holding and comparing crossword puzzle words
*/

#pragma once
#include <string>

class WordList {
public:
	class Word {
	public:
		Word(std::string word, unsigned int startRow, unsigned int startCol, unsigned int wordId, WordDirection dir)
		{
			_word = word;
			_startIndex[0] = startRow;
			_startIndex[1] = startCol;
			_direction = dir;
			_wordId = wordId;

			// calculating endpoint
			if (dir == WordDirection::Across) {
				_endIndex[0] = startRow;
				_endIndex[1] = startCol + word.length() - 1;
			}
			else { // down
				_endIndex[1] = startCol;
				_endIndex[0] = startRow + word.length() - 1;
			}

			_next = nullptr;
		}

		Word(const Word* word) {
			this->_direction = word->_direction;
			this->_endIndex[0] = word->_endIndex[0];
			this->_endIndex[1] = word->_endIndex[1];
			this->_startIndex[0] = word->_startIndex[0];
			this->_startIndex[1] = word->_startIndex[1];
			this->_word = word->_word;
			this->_wordId = word->_wordId;
			this->_next = nullptr;
		}

		std::string _word;

		// indexing in col, row format (to support x, y formatting)
		unsigned int _startIndex[2];	// starting point
		unsigned int _endIndex[2];		// ending point
		int _wordId;

		WordDirection _direction;

		Word* _next;

		~Word() {}
	};

	WordList();
	WordList(const WordList* wordList) {
		_numWords = wordList->_numWords;
		if (wordList->_head == nullptr)
			return;

		Word* tmp = wordList->_head;

		_head = new Word(tmp);

		Word* curr = _head;

		tmp = tmp->_next;
		while (tmp != nullptr) {
			curr->_next = new Word(tmp);

			curr = curr->_next;
			tmp = tmp->_next;
		}
	}

	/// Adds word to current word list
	void AddWord(std::string word, unsigned int startRow, unsigned int startCol, unsigned int wordId, WordDirection dir);
	void AddWord(Word* newWord);
	
	/// Determines if word list is valid (no word conflicts)
	bool Goal();
	bool Goal(Word * it2);

	/// returns the number of words in list
	unsigned int GetNumWords() { return _numWords; }
	
	/// Removes word from the front of the list
	void PopFront();
	
	/// Prints the current puzzle
	void PrintPuzzle();

	/// Destructor
	~WordList() {
		while (_head != nullptr) {
			Word* curr = _head;
			_head = _head->_next;
			delete curr;
		}
		
		_numWords = 0;
	}

	static unsigned int crosswordDimensions[2];
private:
	Word* _head;
	unsigned int _numWords;
};



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