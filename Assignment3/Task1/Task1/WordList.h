/* 
* MSIM 480 - Assignment 3
* Creator: Thomas J Laverghetta
* 
* Linked List for holding and comparing crossword puzzle words
*/

#pragma once
#include <string>

enum WordDirection {
	Across, 
	Down
};

class WordList {
public:
	WordList();

	/// Adds word to current word list
	void AddWord(std::string word, unsigned int startRow, unsigned int startCol, unsigned int wordId, WordDirection dir);
	
	/// Determines if word list is valid (no word conflicts)
	bool Goal();
	
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
private:
	class Word {
	public:
		Word(std::string& word, unsigned int& startRow, unsigned int& startCol, unsigned int& wordId, WordDirection& dir) 
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

		std::string _word;

		// indexing in col, row format (to support x, y formatting)
		unsigned int _startIndex[2];	// starting point
		unsigned int _endIndex[2];		// ending point
		unsigned int _wordId;
		
		WordDirection _direction;
		
		Word* _next = nullptr;

		~Word() {}
	};
	Word* _head;
	unsigned int _numWords;
};

