#pragma once
#include "Event.h"

enum importance { critical, required, optional};
class task
{
private: string toDo;
		 importance level;
public:
	//constructor
	task()
	{
		toDo = "sth";
		level = optional;
	}

	~task()
	{}
	//sets the values of a task
	void setTask(string t, importance l)
	{
		toDo = t;
		level = l;
	}
	//string taskString() gives the format for a task instance saved as string
	string taskString()
	{
		string str = toDo;
		switch (level) {
		case critical:
			str = str + " This is critical.";
			break;
		case required:
			str = str + " This is required.";
			break;
		case optional:
			str = str + " This is optional.";
			break;
		}
		return str;

	}
};


