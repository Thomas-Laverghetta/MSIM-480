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

