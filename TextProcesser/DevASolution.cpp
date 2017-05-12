//////////////////////////////////////////////////////////////
// Input files:
// tweets.csv file will be parsed for people (@person format), topics (#topic format)
// positivewords.csv and negativewords.csv: used to calculate the emotional score of people or topics
// 
// a person/topic is mentioned in a post 
// if this post contains positive words the emotional score will increase
// if it contains negative words emotional score will decrease
//
// Output files:
//TopicsList.txt containing every topic mentioned and its emotional score
//PeopleList.txt containing every person mentioned and their emotional score
//////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Dictionary.h"
#include <unordered_set>
#include<unordered_map>
#include<regex>
using namespace std;



string author, s;
string no = "0123456789";
string alph = "abcdefghijklmnopqrstuvwxyz";
Dictionary positive, negative;

//Trying visitor pattern in order to calculate the emotional scores
class AbstractAnalyser;

class Analysable
{
public:

	virtual void accept(AbstractAnalyser &a) = 0;
};

class Topic;
class Person;

//represents the visitor interface
class AbstractAnalyser
{
public:

	virtual void analyse(Topic &t) = 0;
	virtual void analyse(Person &p) = 0;

};

class Topic : public Analysable
{
public:
	string subject;
	int EmotionalScore;
	Topic()
	{
		subject = "";
		EmotionalScore = 0;
	}
	Topic(string s)
	{
		subject = s;
		EmotionalScore = 0;
	}

	string getSubject()
	{
		return subject;
	}
	int getEmotionalScore()
	{
		return EmotionalScore;
	}
	string print()
	{
		string s = "";
		s = subject + " has EmotionalScore: " + to_string(EmotionalScore);
		return s;
	}
	void updateEmotionalScore(int a)
	{
		EmotionalScore = EmotionalScore + a;
	}
	void accept(AbstractAnalyser &analyser) override {
		analyser.analyse(*this);
	}
};

class Person : public Analysable
{
private:
	string name;
	int EmotionalScore;
public:
	Person()
	{
		name = "";
		EmotionalScore = 0;
	}

	Person(string name)
	{
		this->name = name;
		EmotionalScore = 0;
	}
	string getName()
	{
		return name;
	}
	int getEmotionalScore()
	{
		return EmotionalScore;
	}
	void updateEmotionalScore(int a)
	{
		EmotionalScore = EmotionalScore + a;
	}
	string print()
	{
		string s = "";
		s = name + " has EmotionalScore: " + to_string(EmotionalScore);
		return s;
	}
	void accept(AbstractAnalyser &analyser) override {
		analyser.analyse(*this);
	}

};

class EmotionalAnalyser : public AbstractAnalyser
{
private: int addPoints;
public:
	EmotionalAnalyser()
	{
		addPoints = 0;
	}
	void modifyPoints(int m)
	{
		addPoints = m;
	}
	void analyse(Topic &t) {
		t.updateEmotionalScore(addPoints);
	}
	void analyse(Person &p) {
		p.updateEmotionalScore(addPoints);
	}
};

EmotionalAnalyser a;
///////////////////////////////////////////////////////////////////////////////////
//These 2 functions are used to work with the words, topics and people found in posts
//
//checks if a word contains any letters
bool checkString(string w)
{
	for (int j = 0; j<w.size(); j++)
		for (int i = 0; i < alph.size(); i++)
		{
			if (w[j] == alph[i] || w[j] == toupper(alph[i])) return true;
		}
	return false;
}
//finds the first letter of a word and uses it to index said word
int firstLetter(string w)
{
	string x = alph;
	std::transform(w.begin(), w.end(), w.begin(), tolower);
	string c =w.substr(0, 1);
	int i=w.find_first_of(x+no);
	int index = -1;
 	if (c=="_") return 36;
	for (int i = 0; i < alph.length(); i++)
		if (c==alph.substr(i, 1)) {
				index = i;
				return index;
				}
	for (int i = 0; i < no.length(); i++)
			if (c==no.substr(i,1)) {
				index = i + 26;
				return index;
			}
	return 37;
}
///////////////////////////////////////////////////////////////////////////////
//
// class post represents one line inside the tweets.csv file 
// this class processes said line obtaining the topics, people and the emotional score of a post
//
//////////////////////////////////////////////////////////////////////////
class post
{
private:
	int id, AddedPoints;
	string line;
	list<string> peopleStr;
	list<string> topicStr;
public:

	post()
	{
		id = 0;
		line = "";
		AddedPoints = 0;
	}
	post(int n, string line)
	{
		id = n;
		this->line = line;
		AddedPoints = 0;
	}
	int getAddedPoints()
	{
		return AddedPoints;
	}
	list<string> getTopics()
	{
		return topicStr;
	}
	list<string> getPeople()
	{
		return peopleStr;
	}
	//erases any double spaces from the post
	void trimLine()
	{
		int n = line.size();
		for (int i = 0; i < n - 1; i++)
		{
			if (line[i] == line[i + 1] && line[i] == ' ')
			{
				line.erase(i, 1);
				n--;
			}
		}
	}
	//used to remove quotes from the line as well as invalid characters
	void modifyLine()
	{
		line.erase(remove(line.begin(), line.end(), '\"'), line.end());
		string x = alph;
		std::transform(x.begin(), x.end(), x.begin(), toupper);
		string valid = no + alph + x + "-_@#!^'$%(),.;:?*";
		for (int j = 0; j < line.size(); j++)
			if (!valid.find(line[j]))
			{
				line[j] = ' ';
				line.erase(j, 1);
			}
		trimLine();
	}
	//parses a line/past searching for people(referenced with @), topics(referenced with #) and normal words to compute emotional score 
	void analysePost()
	{
		modifyLine();
		int x0 = line.find_first_of(',');
		author = line.substr(0, x0);
		peopleStr.push_back(author);
		s = line.substr(x0 + 1);
		int i = 0, x = 0, y = 0, z = 0;
		string w;
		i = s.find_first_of(" ,.!?", 2) - 1;
		if (i > 1)
		{
			w = s.substr(1, i);
			i++;
		}
		//here we check for the first word in the line(post) we just read from the tweets file
		if (s[0] == '@')
		{
			if (checkString(w)) peopleStr.push_back(w);
		}
		else
			if (s[0] == '#')
			{
				if (checkString(w)) topicStr.push_back(w);
				i ++;
			}
			else
			{

				if (positive.findWord(w))
				{
					AddedPoints++;
				}
				if (negative.findWord(w))
				{
					AddedPoints--;
				}
			}
		//the loop repeats while there are words left in the line(post) read
		while (s != "")
		{
			x = s.find_first_of(" @#,.!?:;", i);
			if (x != std::string::npos) {//1st check
				y = s.find_first_not_of(" ,.?!@#;:", x);
				if (y != std::string::npos)
				{//2nd check
					z = s.find_first_of(" ,.?!@#:;", y );
					if (z != std::string::npos)
					{//3rd check
						if (y < z) {//4th check
							w = s.substr(y, z - y);
							if (s[x] == '@' || s[y - 1] == '@' || w[0] == '@')
							{

								if (checkString(w)) peopleStr.push_back(w);
							}
							else
								if (s[x] == '#' || s[y - 1] == '#' || w[0] == '#') {
									if (checkString(w)) topicStr.push_back(w);
								}
								else
								{

									if (positive.findWord(w))
									{
										AddedPoints++;
									}
									if (negative.findWord(w))
									{
										AddedPoints--;
									}
								}
						}//4th check over
						string s2 = s.substr(z);
						s = s2;
						i = 0;
					}//3rd check over
					else
					{//3eelse
						w = s.substr(y);
						//cout << w << "|";
						if (y < s.length()) {//4else
							if (s[x] == '@' || s[y - 1] == '@' ||w[0]=='@')
							{
								if (checkString(w)) peopleStr.push_back(w);
							}
							else
								if (s[x] == '#' || s[y - 1] == '#'||w[0]=='3') {
									if (checkString(w)) topicStr.push_back(w);
								}
								else
								{

									if (positive.findWord(w))
									{
										AddedPoints++;
									}
									if (negative.findWord(w))
									{
										AddedPoints--;
									}
								}
						}
						s = "";
					}//3else over
				}//2nd else over
				else
				{
					s = "";
				}
			}//1st else over
			else
			{
				//todo check for pos or neg
				w = s;
				if (positive.findWord(w))
				{
					AddedPoints++;
				}
				if (negative.findWord(w))
				{
					AddedPoints--;
					//cout << "found nw ^^";
				}
				s = "";
			}
			//cout << w << "|";
		}
		if (AddedPoints != 0)
			a.modifyPoints(AddedPoints);
	}




};

///////////////////////////////////////////////////////////////////////////////
//These functions deal with storing people and topics found in the tweets file 
//either in a data structure or in the text files we use as output
//////////////////////////////////////////////////////////////////////////////
void addPerson(string onePerson, unordered_map<string, Person>* people)
{
	Person p(onePerson);
		auto search = people->find(onePerson);
		if (search != people->end()) {
			search->second.accept(a);
		}
	else {
		p.accept(a);
		people->insert(std::pair<string, Person>(p.getName(), p));
	}
}

void addTopic(string oneTopic, unordered_map<string, Topic>* topics)
{

	//add to topic list
	Topic p(oneTopic);
		auto search = topics->find(oneTopic);
		if (search != topics->end()) {
			search->second.accept(a);
		}
	else {
		p.accept(a);
		topics->insert(std::pair<string, Topic>(p.getSubject(), p));
	}
}

void storePeople(unordered_map<string, Person> people[])
{

	ofstream g("PeopleList.txt");
	for (int i = 0; i < 38; i++)
	{
		for (std::unordered_map<string, Person>::iterator iter = people[i].begin(); iter != people[i].end(); ++iter)
		{
			g << iter->second.print() << endl;
		}
	}
	g.close();
}

void storeTopics(unordered_map<string, Topic> topics[])
{
	ofstream t("TopicsList.txt");
	for (int i = 0; i < 38; i++)
	{
		for (std::unordered_map<string, Topic>::iterator iter = topics[i].begin(); iter != topics[i].end(); ++iter)
		{
			t << iter->second.print() << endl;
		}
	}
	t.close();
}

int main()
{
	string positivewords = "positive.words.csv";
	string negativewords = "negative.words.csv";
	string tweets = "tweets.csv";
	unordered_map<string,Person>* people;
	people = new unordered_map<string, Person>[38]();
	unordered_map<string, Topic>* topics;
	topics = new unordered_map<string, Topic>[38]();
	string line;
	int i = 0;
	string full = no + alph + "_";
	positive.input(positivewords);
	negative.input(negativewords);
	//prelucreaza tweetsurile

	ifstream in("tweets.csv");
	int timer = 0;
	//this while passes once through the whole tweets file	
	while (!in.eof())
	{
			timer++;
			list<string> peopleStr ;
			list<string> topicStr ;
			post tweet;
			getline(in,line);
			tweet=post(timer, line);
			tweet.analysePost();
				peopleStr = tweet.getPeople();
				if (!peopleStr.empty())
					for (std::list<string>::iterator ippl = peopleStr.begin(); ippl != peopleStr.end(); ++ippl)
					{
						string w = *ippl;
						int index = firstLetter(w);
						if(index>=0)	addPerson(w, &people[index]);						
					}

				topicStr = tweet.getTopics();
				for (std::list<string>::iterator itp = topicStr.begin(); itp != topicStr.end(); ++itp)
				{

					string w = *itp;
					int index = firstLetter(w);
					if (index >= 0)
					{
						addTopic( w, &topics[index]);
					}
				}
			}
		
	storePeople(people);
	storeTopics(topics);

	std::cout << "Ending now";
	char c;
	std::cin >> c;
	return 0;
}