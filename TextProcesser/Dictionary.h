#pragma once
#include "stdafx.h"
#include <unordered_set>
using namespace std;
class Dictionary
{
private:
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	unordered_set <string> mySet[26];
public:

	Dictionary()
	{
	}

	~Dictionary()
	{
	}
	void input(string filename)
	{
		ifstream f(filename);
		int i = 0;
		char c = alphabet[i];
		string w;
		int check = 1;

		while (!f.eof())
		{
			getline(f, w);
			char d = tolower(w[0]);
			if (c != d)
			{
				i = alphabet.find(d);
				if (i > 0 && i < 25) {
					c = alphabet[i];
				}
				else check = 0;
			}
			if (check) mySet[i].insert(w);
			check = 1;
		}
		f.close();
	}
	//will be used to search the dictionary for words taken from the tweets/posts
	bool findWord(string w)
	{
		char c = tolower(w[0]);
		int index = -1;
		for (int i = 0; i < alphabet.length(); i++)
			if (c == alphabet[i]) {
				index = i;
				i = alphabet.length();
			}
		
		if (index > -1)
		{
			std::unordered_set<std::string>::const_iterator found = mySet[index].find(w);
			if (found == mySet[index].end()) return false;
			return true;
		}
		else return false;
	}

};