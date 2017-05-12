#pragma once
#include "stdafx.h"
#include "calendar.h"
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////////////////
//
// Event class defines the main structure used for the planner
//
// 
//////////////////////////////////////////////////////////////////////////////////////////////

enum frequency { daily, weekly, monthly };
struct cal{
	cal* next;
	calendar currentDate,endDate;
	frequency f;

};
class Event
{
private:
	std::string EventID;
	std::string event_descr, location;
	cal* current;
	boolean repeating, complex;
	task* toDoList; //store tasks when complex=True
	int n; //count the current task 
	//Event() default constructor initializes objects of Event type
public:
	Event()
	{
		current = new cal;
		EventID = "0";
		current->currentDate = calendar(1, 1, 2016);
		event_descr = "-";
		location = "-";
		repeating = false;
		complex = false;
	}
	///////////////////////////////////////////////////////////////////
	//void setValues(...)
	//
	// used to give values to an object of type event
	/////////////////////////////////////////////////////////////////////
	void setValues(std::string ID, calendar d, std::string ed, std::string l, boolean r, boolean compl)
	{
		EventID = ID;
		current->currentDate = d;
		event_descr = ed;
		location = l;
		repeating = r;
		complex = compl;
		if (complex)
		{
			toDoList = new task[5];
			n = 0;
		}
		if (repeating)
		{
			frequency f;
			char x;
			cout << "Would you like this event to repeat daily (d) or weekly (w)?: ";
			cin >> x;
			cout << endl;
			if (x = 'd')
				 f = frequency(daily);
			else 
				if (x = 'w')
				{
					f = frequency(weekly);
				}
			else cout << "Invalid input." << endl;

			cout << "What is the last date for this event? ";
			cout << "Enter the day(1 to 31): "; int d; cin >> d;
			cout << "Enter the month(1 to 12): "; int m; cin >> m;
			calendar today;
			calendar end;
			if (today.compare(calendar(d, m, today.getYear())) > 0)
				end = calendar(d, m, today.getYear() + 1);
			else end = calendar(d, m, today.getYear());
		}
	}


	/////////////////////////////////////////////////////////
	//setRepeat()
	//
	// sets the parameters required to repeat an event
	////////////////////////////////////////////////////////
	void setRepeat(frequency f, calendar end)
	{
		current->f = f;
		current->endDate = end;
	}
	/////////////////////////////////////////////////////////
	//repeat()
	//
	// used for repeating events to get the next date of the event
	////////////////////////////////////////////////////////
	void repeat()
	{
		cal *n;
		repeating = true;
		calendar next, date, end;
		end = current->endDate;
		frequency f = current->f;
		date = current->currentDate;
		if (date.compare(end) < 0)
		{
			n->f = f;
			n->endDate = end;
			switch (f)
			{
			case daily:
			{
				if (date.getNext().compare(end) < 0)
				{
					next = current->currentDate.getNext();
				}
				else
					if (date.getNext().equals(end))
					{
						next = date.getNext();
						n->endDate = next;
						repeating = false;
					}
			}
			break;
			case weekly:
			{
				if (date.getNextWeek().compare(end) < 0)
				{
					next = date.getNextWeek();
				}
				else
					if (date.getNextWeek().equals(end))
					{
						next = date.getNextWeek();
						n->endDate = next;
						repeating = false;
					}
			}
			break;
			}
			n->currentDate = next;
			n->f = f;
			current->next = n;
		}
		else repeating = false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// string getID() returns the unique id of each event
	//////////////////////////////////////////////////////////////////////////////////////////////
	std::string getID()
	{
		return EventID;

	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// string printEvent() returns a string with the variables and values of objects of Event type
	// it is also used to define the format of Events in the txt file Planner.txt
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////
	std::string printEvent()
	{

		std::string toPrint;
		toPrint = "Event ID: " + EventID + ", description: " + event_descr + ", location: " + location + ". Date: " + current->currentDate.toString() + ".";
		if (repeating) toPrint = toPrint + "Event repeats; ";
		if (complex) {
			toPrint = toPrint + "Event has subtasks: ";
			for (int i = 0; i < n; i++)
				toPrint = toPrint + "Task " + to_string(i + 1) + toDoList[i].taskString() + "; ";
		}

		return toPrint;
	}
	//copies the Event given as parameter into current Event
	void copy(Event o)
	{
		EventID = o.getID();
		current->currentDate = o.current->currentDate;
		event_descr = o.event_descr;
		location = o.location;
		repeating = o.repeating;
		complex = o.complex;
	}

	//destructor for Event objects
	~Event()
	{
		EventID = "0";
		current->currentDate = calendar(1, 1, 2016);
		event_descr = "";
		location = "";
		repeating = false;
		complex = false;
	}
	///////////////////////////////////////////////////////////////////
	//addTask()
	//
	//adds tasks in the toDoList given as an option for complex events
	////////////////////////////////////////////////////////////////
	void addTask(string toDo, importance l)
	{
		if (n < 4)
		{
			task t;
			t.setTask(toDo, l);
			toDoList[n] = t;
			n++;
			cout << "Task added ";
		}
		else cout << "Exceeded maximum number of tasks";
	}

	void update()
	{
		calendar d =calendar();
		calendar c = d.getDateNow();
		if ((current->currentDate.compare(c) > 0) && repeating)
		{
			repeat();
		}
	}
	void setDate(calendar date)
	{
		this->current->currentDate = date;

	}
	calendar getDate()
	{
		return current->currentDate;
	}
	string getDescr()
	{
		return event_descr;
	}
	string getLoc()
	{
		return location;
	}
};

