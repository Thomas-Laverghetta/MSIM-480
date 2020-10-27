#pragma once
#include "WordList.h"

class WordQueue
{
public:
	WordQueue() {
		_head = nullptr;
		_size = 0;
	}

	void Push(WordList& wordList) {
		WordSet* newSet = new WordSet(wordList);

		newSet->_next = _head;
		_head = newSet;
		_size++;
	}

	WordList Pop() {
		WordSet* curr = _head;
		_head = _head->_next;

		WordList* wordSet = &curr->_wordList;

		delete curr;
		_size--;

		return *wordSet;
	}

	unsigned int Size() { return _size; }
private:
	class WordSet {
	public:
		WordSet(WordList& wordList) : _wordList(wordList), _next(nullptr){}
		WordList _wordList;
		WordSet* _next;

		~WordSet() {}
	};

	WordSet* _head;
	unsigned int _size;
};

