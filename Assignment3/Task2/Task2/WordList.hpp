#pragma once
#include <string>
#include <vector>

struct WordElementSet {
	int wordId;
	std::vector<std::string> words;
};

struct Word {
	std::string word;
	int wordId;
};

class WordList {
public:
	WordList() {
		_head = nullptr;
		_counter = 0;
	}

	// adding word to list
	void AddWord(Word& word) {
		WordNode* newWord = new WordNode(word);

		newWord->_next = _head;
		_head = newWord;

		_counter++;
	}

	// deleting top of list
	void pop() {
		WordNode* curr = _head;
		_head = _head->_next;

		delete curr;
		_counter--;
	}

	// returning top of list
	Word* top() {
		return &_head->_word;
	}

	// returns size of list
	unsigned int size() { return _counter; }

	// printing list
	void Print() {
		printf("Word ID | Word\n");
		printf("========================\n");

		WordNode* curr = _head;
		while (curr != nullptr) {
			printf("%-4i | %s\n", curr->_word.wordId, curr->_word.word.c_str());
			curr = curr->_next;
		}

		printf("\n\t *Negative IDs are the horizontal (across) counterpart for vertical/horizontal words\n");
	}
private:
	struct WordNode {
		WordNode(Word& word) : _word(word) {}
		Word _word;
		WordNode* _next = nullptr;
	};

	WordNode* _head;
	unsigned int _counter;
};