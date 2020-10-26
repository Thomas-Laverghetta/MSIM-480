#include "WordQueue.h"

int numberOfWords;
WordList * SolutionList;
bool Backtracking() {
	
	WordQueue queue;

	while (queue.Size() > 0) {
		WordList list = queue.Pop();
		if (list.Goal()) {
			if (list.GetNumWords() == numberOfWords) {
				SolutionList = &list;
				return true;
			}
			else {
				
			}
		}
	}

	// No Solution Found
	return false;
}

int main() {


	return 0;
}