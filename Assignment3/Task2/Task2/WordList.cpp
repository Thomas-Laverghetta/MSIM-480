#include "WordList.h"

WordList::WordList()
{
	_head = nullptr;
	_numWords = 0;
}

void WordList::AddWord(std::string word, unsigned int startRow, unsigned int startCol, unsigned int wordId, WordDirection dir)
{
	// create new word
	Word * newWord = new Word(word, startRow, startCol, wordId, dir);

	// list management
	newWord->_next = _head;
	_head = newWord;
	_numWords++;
}

void WordList::AddWord(Word* newWord) {
	// list management
	newWord->_next = _head;
	_head = newWord;
	_numWords++;
}

bool WordList::Goal() {
	Word* it1 = _head;
	while (it1 != nullptr){
		Word* it2 = it1->_next;
		while (it2 != nullptr) {
			int A1 = (it1->_endIndex[1] - it1->_startIndex[1]);
			int A2 = (it2->_endIndex[1] - it2->_startIndex[1]);
			int B1 = (it1->_startIndex[0] - it1->_endIndex[0]);
			int B2 = (it2->_startIndex[0] - it2->_endIndex[0]);

			// calculating the determinet to determine parallelism:
			// A = y2 - y1, B = x1 - x2
			int det = A1 * B2 - A2 * B1;

			// if det does't equal zero, then not parallel (intersection)
			if (det != 0) {
				// C = A*x1 + B*y
				int C1 = A1 * it1->_startIndex[0] + B1 * it1->_startIndex[1];
				int C2 = A2 * it2->_startIndex[0] + B2 * it2->_startIndex[1];

				// intersection point
				int row = ((float)(B2 * C1 - B1 * C2)) / ((float)det);
				int col = ((float)(A1 * C2 - A2 * C1)) / ((float)det);

				// verifying intersection is within domain
				if (it1->_startIndex[0] <= row && row <= it1->_endIndex[0] && it2->_startIndex[0] <= row && row <= it2->_endIndex[0]
					&& it1->_startIndex[1] <= col && col <= it1->_endIndex[1] && it2->_startIndex[1] <= col && col <= it2->_endIndex[1]) {
					// Finding String index points to compare from
					int wordIndex1, wordIndex2;

					// first word's index point
					if (it1->_direction == WordDirection::Across) {
						wordIndex1 = col - it1->_startIndex[1];
					}
					else {
						wordIndex1 = row- it1->_startIndex[0];
					}

					// second word's index point
					if (it2->_direction == WordDirection::Across) {
						wordIndex2 = col - it2->_startIndex[1];
					}
					else {
						wordIndex2 = row - it2->_startIndex[0];
					}
					
					// comparing words
					if (it1->_word[wordIndex1] != it2->_word[wordIndex2])
						return false;	// conflict
				}
			}
			// checking if lines overlap
			else if (it1->_direction == WordDirection::Down && it1->_endIndex[0] == it2->_endIndex[0]) { // vertical line overlaps
				if (it1->_endIndex[1] > it2->_endIndex[1] && it1->_startIndex[1] <= it2->_endIndex[1]) {
					return false; // overlap
				}
				else if (it2->_endIndex[1] > it1->_endIndex[1] && it2->_startIndex[1] <= it1->_endIndex[1]) {
					return false; // overlap
				}
				else if (it1->_endIndex[1] == it2->_endIndex[1]) {
					return false; // overlap
				}
			}
			else if (it1->_direction == WordDirection::Across && it1->_endIndex[1] == it2->_endIndex[1]) { // harizontle line overlaps
				if (it1->_endIndex[0] > it2->_endIndex[0] && it1->_startIndex[0] <= it2->_endIndex[0]) {
					return false; // overlap
				}
				else if (it2->_endIndex[0] > it1->_endIndex[0] && it2->_startIndex[0] <= it1->_endIndex[0]) {
					return false; // overlap
				}
				else if (it1->_endIndex[0] == it2->_endIndex[0]) {
					return false; // overlap
				}
			}
			it2 = it2->_next;
		}
		it1 = it1->_next;
	}

	// No conflicts
	return true;
}

bool WordList::Goal(Word* it2)
{
	Word* it1 = _head;
	while (it1 != nullptr) {
		int A1 = (it1->_endIndex[1] - it1->_startIndex[1]);
		int A2 = (it2->_endIndex[1] - it2->_startIndex[1]);
		int B1 = (it1->_startIndex[0] - it1->_endIndex[0]);
		int B2 = (it2->_startIndex[0] - it2->_endIndex[0]);

		// calculating the determinet to determine parallelism:
		// A = y2 - y1, B = x1 - x2
		int det = A1 * B2 - A2 * B1;

		// if det does't equal zero, then not parallel (intersection)
		if (det != 0) {
			// C = A*x1 + B*y
			int C1 = A1 * it1->_startIndex[0] + B1 * it1->_startIndex[1];
			int C2 = A2 * it2->_startIndex[0] + B2 * it2->_startIndex[1];

			// intersection point
			int row = ((float)(B2 * C1 - B1 * C2)) / ((float)det);
			int col = ((float)(A1 * C2 - A2 * C1)) / ((float)det);

			// verifying intersection is within domain
			if (it1->_startIndex[0] <= row && row <= it1->_endIndex[0] && it2->_startIndex[0] <= row && row <= it2->_endIndex[0]
				&& it1->_startIndex[1] <= col && col <= it1->_endIndex[1] && it2->_startIndex[1] <= col && col <= it2->_endIndex[1]) {
				// Finding String index points to compare from
				int wordIndex1, wordIndex2;

				// first word's index point
				if (it1->_direction == WordDirection::Across) {
					wordIndex1 = col - it1->_startIndex[1];
				}
				else {
					wordIndex1 = row - it1->_startIndex[0];
				}

				// second word's index point
				if (it2->_direction == WordDirection::Across) {
					wordIndex2 = col - it2->_startIndex[1];
				}
				else {
					wordIndex2 = row - it2->_startIndex[0];
				}

				// comparing words
				if (it1->_word[wordIndex1] != it2->_word[wordIndex2]) {
					return false;	// conflict
				}
			}
		}
		// checking if lines overlap
		else if (it1->_direction == WordDirection::Down && it1->_endIndex[0] == it2->_endIndex[0]) { // vertical line overlaps
			if (it1->_endIndex[1] > it2->_endIndex[1] && it1->_startIndex[1] <= it2->_endIndex[1]) {
				return false; // overlap
			}
			else if (it2->_endIndex[1] > it1->_endIndex[1] && it2->_startIndex[1] <= it1->_endIndex[1]) {
				return false; // overlap
			}
			else if (it1->_endIndex[1] == it2->_endIndex[1]) {
				return false; // overlap
			}
		}
		else if (it1->_direction == WordDirection::Across && it1->_endIndex[1] == it2->_endIndex[1]) { // harizontle line overlaps
			if (it1->_endIndex[0] > it2->_endIndex[0] && it1->_startIndex[0] <= it2->_endIndex[0]) {
				return false; // overlap
			}
			else if (it2->_endIndex[0] > it1->_endIndex[0] && it2->_startIndex[0] <= it1->_endIndex[0]) {
				return false; // overlap
			}
			else if (it1->_endIndex[0] == it2->_endIndex[0]) {
				return false; // overlap
			}
		}
		it1 = it1->_next;
	}

	// No conflicts
	return true;
}

void WordList::PopFront()
{
	_numWords--;
	Word* curr = _head;
	_head = _head->_next;

	delete curr;
}

void WordList::PrintPuzzle()
{
	Word* curr = _head;
	printf("Variable  |  Cell | Word\n");
	printf("==============================================\n");
	while (curr != nullptr) {
		if (curr->_direction == WordDirection::Across) {
			printf("%-2i Across | (%i,%i) | %s\n", curr->_wordId, curr->_startIndex[0], curr->_startIndex[1], curr->_word.c_str()); 
			fflush(stdout);
		}
		else {
			printf("%-2i Down   | (%i,%i) | %s\n", curr->_wordId, curr->_startIndex[0], curr->_startIndex[1], curr->_word.c_str()); 
			fflush(stdout);
		}
		curr = curr->_next;
	}
}
