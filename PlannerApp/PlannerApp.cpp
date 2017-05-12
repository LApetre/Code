// PlannerApp.cpp : Defines the entry point for the console application.
//

#pragma once

#include "stdafx.h"
#include "Event.h"
#include <cstdio>
#include <fstream>
#include <sstream>

using namespace std;

Event* events;
int index = 0;


void quickSort(Event* e, int left, int right) {

	int i = left, j = right;
	Event pivot = e[(left + right) / 2];

	/* partition */

	while (i <= j) {

		while (e[i].getDate().compare(pivot.getDate()) < 0)
			i++;
		while (e[j].getDate().compare(pivot.getDate())>0)
			j--;

		if (i <= j) {
			Event aux;
			aux = e[i];
			e[i] = e[j];
			e[j] = aux;
			i++;
			j--;
		}
	};
	/* recursion */
	if (left < j)
		quickSort(e, left, j);
	if (i < right)
		quickSort(e, i, right);

}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void getEvents()
//
// opens the file we use to store information about events and parses it 
// stores each line as a variable of Event type inside Event* events;
//////////////////////////////////////////////////////////////////////////////////////////////
void getEvents()
{
	events = new Event[200];
	ifstream f("plannerFile.txt");
	string line;
	boolean a = false;
	while (!f.eof())
	{
		getline(f, line);
		if (line != "") {
			int I1 = strlen("Event ID: ");
			int I2 = line.find_first_of(',', I1);

			string id = line.substr(I1, I2 - I1);//extracting event id from the text file
			

			I1 = line.find("description: ") + strlen("description: ");
			I2 = line.find_first_of(',', I1);
			string descr = line.substr(I1, I2 - I1);//extracting descr from the text file
			
			I1 = line.find("location: ") + strlen("location: ");
			I2 = line.find_first_of('.', I1);
			string loc = line.substr(I1, I2 - I1);//extracting location from the text file
			
			I1 = line.find("Date: ") + strlen("Date: ");
			I2 = line.find_first_of('.', I1);
			string d = line.substr(I1, I2 - I1);//extracting date from the text file
			
			//turning the string date into a calendar date
			int day, month, year;
			I1 = d.find_first_of('/');
			day = stoi(d.substr(0, I1));
			I2 = d.find_first_of('/', I1 + 1);
			month = stoi(d.substr(I1 + 1, I2 - I1));
			year = stoi(d.substr(I2 + 1));
			calendar c1(day, month, year);

			boolean r, c;
			r = false; c = false;
			if (line.find("repeats") != -1)
				r = true;
			if (line.find("subtasks") != -1) {
				c = true;
			}
			//creating and adding the Event to the events array
			Event e;
			e.setValues(id, c1, descr, loc, r, c);
			events[index] = e;
			index++;
			a = true;
		}
	}
	f.close();
	if (!a)
	{
		cout << endl << "No event was found... " << endl << endl;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void showAllEvents()
//
// opens the file we use to store information about events and prints its contents to the console
// 
//////////////////////////////////////////////////////////////////////////////////////////////

void showAllEvents()
{
	ifstream f("plannerFile.txt");
	string line;
	cout << endl;
	bool a = false;
	cout << "Here is a list of all events : " << endl;
	while (!f.eof())
	{
		getline(f, line);
		cout << line << endl;
		a = true;
	}
	f.close();
	if (!a)
	{
		cout << endl << "No event was found... " << endl << endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
//Prints the events that will take place after today 
//
//////////////////////////////////////////////////////////////////////////////////////////////

void showEventsAfterToday()
{
	
	time_t t;
	time(&t);   // get time now
	tm  nowT;
	tm * now;
	localtime_s(&nowT, &t);
	now = &nowT;
	int year = now->tm_year + 1900;
	int month = now->tm_mon + 1;
	int day = now->tm_mday;

	if (index == 0) {
		getEvents();
	}
	Event* e;
	e = new Event[50];
	int c = 0;
	for (int i = 0; i < index; i++)
		if (calendar(day, month, year).compare(events[i].getDate())<0) {
			e[c].copy(events[i]);
			c++;
		}
	//create an array containing incoming events found
	quickSort(e, 0, c-1);

	for (int i = 0; i < c; i++)
		std::cout << e[i].printEvent() << endl;

}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//Prints the events that took place before and including today
//
//////////////////////////////////////////////////////////////////////////////////////////////

void showEventsBeforeToday()
{
	int year, month, day;
	time_t t;
	time(&t);   // get time now
	tm  nowT;
	tm * now;
	localtime_s(&nowT, &t);
	now = &nowT;
	year = now->tm_year + 1900;
	month = now->tm_mon + 1;
	day = now->tm_mday;

	if (index == 0) {
		getEvents();
	}
	Event* e;
	e = new Event[50];
	int c = 0;
	for (int i = 0; i < index; i++)
		if (calendar(day, month, year).compare(events[i].getDate())>=0) {
			e[c].copy(events[i]);
			c++;
		}//create an array containing the events we found before today
	quickSort(e, 0, c - 1);
	/*
	calendar* d = new calendar[50];
	for (int i = 0; i < c; i++)
	{
		d[i] = e[i].getDate();
	}//create dat array in order to be given to the calendar::prinMonth() function as parameter
	calendar first(1, month, 2016);
	first.printMonth(d, c);*/
	for (int i = 0; i < c; i++)
		cout << e[i].printEvent() << endl;

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// void showEventsByMonth(int month)
//
// iterates over the events array and prints only the events in a certain month
// 
//////////////////////////////////////////////////////////////////////////////////////////////

void showEventsByMonth(int month)
{
	if (index == 0) {
		getEvents();
	}
	Event* e;
	e = new Event[50];
	int c = 0;
	for (int i = 0; i < index; i++)
		if (events[i].getDate().getMonth() == month) {
			e[c].copy(events[i]);
			c++;
		}//create an array containing events of only month m

	int changed = 1;
	do {
		changed = 0;
		for (int i = 0; i < c - 1; i++)
		{
			if (e[i].getDate().getDay() > e[i + 1].getDate().getDay())
			{
				Event aux;
				aux.copy(e[i]);
				e[i].copy(e[i + 1]);
				e[i + 1].copy(aux);
				changed = 1;
			}
		}
	} while (changed);//sort the array in ascending order by date
	calendar* d = new calendar[50];
	for (int i = 0; i < c; i++)
	{
		d[i] = e[i].getDate();
	}//create dat array in order to be given to the calendar::prinMonth() function as parameter
	calendar first(1, month, 2016);
	first.printMonth(d, c);
	for (int i = 0; i < c; i++)
		cout << e[i].printEvent() << endl;

}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void showEventsByDay(calendar d)
//
// iterates over the events array and prints only the events on a certain date
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void showEventsByDay(calendar d)
{
	if (index == 0) {
		getEvents();
	}
	boolean found=false,printed=false;
	for (int i = 0; i < index; i++)
		if (events[i].getDate().getDay() == d.getDay() && events[i].getDate().getMonth() == d.getMonth() && events[i].getDate().getYear() == d.getYear())
		{
			found = true;
			if (found && !printed) {
				d.printcalendar();
				printed = true;
			}
			cout << events[i].printEvent() << endl;
		}
	if (!found) cout << "No event on that day" << endl;

}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void calculateID()
//
//calculates the id of an event based on the number of already existing events
// 
//////////////////////////////////////////////////////////////////////////////////////////////
string calculateID()
{
	if (index == 0) {
		getEvents();
	}
	string id;
	id = "e" + to_string(index + 1);
	return id;


}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void addEvent(Event e)
//
//add Event both to the events array and to our file
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void addEvent()
{
	int year, month, day;
	time_t t;
	time(&t);   // get time now
	tm  nowT;
	tm * now;
	localtime_s(&nowT, &t);
	now = &nowT;
	year = now->tm_year + 1900;
	month = now->tm_mon + 1;
	day = now->tm_mday;
	/////////////////////////////////////
	Event e;
	clrscr();
	boolean compl = false;
	string descr, loc, s;
	cout << "Enter your event's description: ";
	cin.ignore(1000, '\n');
	getline(cin, descr);
	cout << "Enter your event's location: ";
	getline(cin, loc);
	cout << "Enter the day(1 to 31): "; int d; cin >> d;
	cout << "Enter the month(1 to 12): "; int m; cin >> m;
	calendar c1(d, m, year);
	cout << "Would you like to add a to-do list for your event?" << endl << "(y) Yes " << endl << "(n) No " << endl;
	cin >> s;
	if (s == "y") compl= true;
	if (c1.getValid()) {
		string id = calculateID();
		e.setValues(id, c1, descr, loc, false, compl);

		if (compl)
		{
			do {
				cout << "Add a task to your to-do list:" << endl << "(y) Yes " << endl << "(n) No ";
				cin >> s;
				if (s == "y") {
					cout << "Add task description: ";
					cin.ignore(1000, '\n');
					getline(cin, descr);
					cout << "How important is your task: " << endl << "(1) critical (2) required (3) optional";
					int imp;
					cin >> imp;
					if (imp == 1) e.addTask(descr, critical);
					else if (imp == 2) e.addTask(descr, required);
					else e.addTask(descr, optional);
				}
				else s = "n";

			} while (s == "y");
		}

		cout << "Event was succesfully added. " << endl;
	}
	else {
		cout << "Invalid date. Please enter a new one" << endl;
	}




	////////////////////////////////
	events[index] = e;
	index++;
	ofstream g;
	g.open("plannerFile.txt", ios::out | ios::app);
	g << endl << e.printEvent();
	g.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
// void menu()
//
//implements a menu with option for the user to choose from
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void menu()
{

	char input = 'a';
	cout << "Welcome to your personal planner !" << endl;
	while (input != 'q')
	{
		clrscr();
		cout << "What do you want to do ?" << endl << "(a) Add new event." << endl << "(b) See all events." << endl << "(c) Show events by month." << endl << "(d) Show events by day." << endl << "(e) Show past events"<<endl<<"(f) Show incoming events"<<endl<<"(q) Quit." << endl;
		cin >> input;
		switch (input)
		{
		case 'a':
			addEvent();
			break;
		case 'b':
		{
			clrscr();
			cout << "Showing all events";
			showAllEvents();

		}
		break;
		case 'c':
		{
			clrscr();
			cout << "Enter the month(1 to 12): "; int m; cin >> m;
			showEventsByMonth(m);

		}
		break;
		case 'd':
		{
			clrscr();
			cout << "Enter the day(1 to 31): "; int d; cin >> d;
			cout << "Enter the month(1 to 12): "; int m; cin >> m;
			calendar c1(d, m, 2016);
			showEventsByDay(c1);
		}
		break;
		case 'e':
		{
			clrscr();
			showEventsBeforeToday();
		}
		break;
		case 'f':
		{
			clrscr();
			showEventsAfterToday();
		}
		break;
		default:
		{
			cout << endl << "Invalid input... Please try again" << endl << endl;
		}
		}
		cout << "(m) Go back to menu. " << endl << "(q) Quit";
		char x;
		cin >> x;
		if (x != 'm') input = 'q';
	}
	cout << "Goodbye !" << endl;
}

int main()
{
	menu();
    return 0;
}

