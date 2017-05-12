#include <iostream>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <cstdlib>


using namespace std;
//void clrscr();
//void gotoxy(int, int);

//////////////////////////////////////////////////////////////////////////////////////////////
//
// void clrscr()
//
// clears the console window
//////////////////////////////////////////////////////////////////////////////////////////////
void clrscr()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// void gotoxy(int x,int y)
// 
// moves cursor to x,y in windows console
//
//////////////////////////////////////////////////////////////////////////////////////////////
void gotoxy(int x, int y)
{
	COORD point;
	point.X = x; point.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

class calendar
{
private: int valid = 1; //variable to check the validity of the date 
public:
	calendar(int d, int m, int y); 
	calendar();
	virtual ~calendar() {} // do nothing destructor virtual in case of use of inheritance
	void printcalendar();
	int getDay();
	int getMonth();
	int getYear();
	bool isleapyear(); // returns true if year is leap
	int checkday(int); // checks legality of date
	int whatdayisfirstofmonth(); // returns 0 to 6 sunday to saturday
	int days_in_month(); // returns number of days in month
	bool equals(calendar);
	int compare(calendar);
	int day; 
	int month;
	int year;
	calendar getNext();
	calendar getNextWeek();
	calendar getDateNow();
	string toString();
	// getValid() used to see whether the entered date was valued or not
	// if it wasn't valid we can't create and add the event
	int getValid()
	{
		return valid;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// void printEventsCalender()
	//
	// prints the calender for the entered date 
	//
	//////////////////////////////////////////////////////////////////////////////////////////////
	void calendar::printMonth( calendar* days, int l)
	{

		clrscr();
		cout << "The date entered was: " << day << "/" << month << "/" << year << endl;
		cout << endl << setw(8) << "SUNDAY" << setw(8) << "MONDAY" << setw(9) << "TUESDAY" << setw(11) << "WEDNESDAY" << setw(10)
			<< "THURSDAY" << setw(8) << "FRIDAY" << setw(10) << "SATURDAY" << endl;
		int startday = whatdayisfirstofmonth();
		int endday = days_in_month();
		for (int i = 0; i<startday; i++)
		{
			if (i == 0)
			{
				gotoxy(4, 4);
				cout << "-";
			}
			if (i == 1)
			{
				gotoxy(12, 4);
				cout << "-";
			}
			if (i == 2)
			{
				gotoxy(21, 4);
				cout << "-";
			}
			if (i == 3)
			{
				gotoxy(31, 4);
				cout << "-";
			}
			if (i == 4)
			{
				gotoxy(42, 4);
				cout << "-";
			}
			if (i == 5)
			{
				gotoxy(50, 4);
				cout << "-";
			}
		} // end of for loop


		int rows = 4;
		int count = 1;
		for (int j = startday; j<(startday + endday); j++)
		{
			if (j % 7 == 0)
			{
				rows += 2;
				gotoxy(4, rows);
			}
			if (j % 7 == 1) gotoxy(12, rows);
			if (j % 7 == 2) gotoxy(21, rows);
			if (j % 7 == 3) gotoxy(31, rows);
			if (j % 7 == 4) gotoxy(42, rows);
			if (j % 7 == 5) gotoxy(50, rows);
			if (j % 7 == 6) gotoxy(60, rows);
			boolean found = false;
			for (int i = 0; i < l; i++)
				if (count == days[i].getDay()) {
					found = true;
					i = l;
				}

			if (found) // set text to bright red if count is the day entered
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
			}
			cout << count;
			count++;
		} // end of for loop
		cout << endl << endl << endl;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
// default constructor
//
// sets date as the current date
//
//////////////////////////////////////////////////////////////////////////////////////////////

calendar::calendar()
{
	time_t t;
	time(&t);
	tm  nowT;
	tm * now;
	localtime_s(&nowT, &t);
	now = &nowT;
	this->year = now->tm_year + 1900;
	this->month = now->tm_mon + 1;
	this->day = now->tm_mday;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// constructor
//
// checks validity of date and sets members
//
//////////////////////////////////////////////////////////////////////////////////////////////

calendar::calendar(int d, int m, int y)
{

	if (y<2016) // can't accept dates before the current year
	{
		cout << endl << "The year " << y << " invalid! Can't add events in previous years " << "Setting it to 2016." << endl;
		year = 2016; 
		valid = 0;
	}
	else
	{
		year = y; // y is valid so use it to set year
	}
	if ((m >= 1) && (m <= 12)) // check month between 1 and 12
	{
		month = m; // if it is set month
	}
	else
	{
		cout << endl << "The month " << m << " is invalid. Setting to month 1" << endl;
		month = 1;
		valid = 0;
	}
	day = checkday(d); // validate the day
	if (d != day) valid = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// isleapyear()
//
// checks whether year is a leap year in the gregorian calender
// a year is leap if it is divisable by four but not if divisible by 100
// unless it is divisible by 400
//
// returns true if leap and false if not
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool calendar::isleapyear()
{
	if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
	{
		return true; // its a leap year
	}

	else
	{
		return false; // its not a leap year
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// checkday(int)
//
// checks day is legal for month
//
// in:- day to be tested
//
// returns day if legal and 1 if not legal
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::checkday(int testday)
{

	if ((testday>0) && (testday <= days_in_month()))
	{
		return testday; // day is valid for month
	}
	cout << endl << "Invalid day entered " << testday << "/" << month << "/" << year
		<< ". Setting to the first of the month." << endl;
	return 1; // hopefully wont get here but if invalid day entered day is set to 1
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// int howmanydays()
//
// returns number of days in month
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::days_in_month()
{
	if ((month == 2) && isleapyear())
	{
		return 29; // feb has 29 days in a leap year
	}
	static const int daysinmonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	return daysinmonth[month - 1];
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// int whatdayisfirstofmonth()
//
// uses zeller's algorithm to find out what day the first of the month falls on falls on.
//
// returns 0 to 6 corresponding to sunday to saturday.
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::whatdayisfirstofmonth()
{
	int c = year / 100; // # of centuries
	int d = year % 100; // # of years through century
	int m = (month + 10) % 12; // # of month march is 1,feb is 12
	int k = 1; // set the day part to 1 so we get back the day for first of month
	if ((month == 1) || (month == 2))// treat jan and feb as if they were in previous year
	{
		if (d == 0) // if d is 0 then to go back a year d beomes 99 and c become c-1
		{
			d = 99;
			c -= 1;
		}
		else
		{
			d -= 1; // jan and feb are treated as previous year
		}
	}
	float g = (k + (floor(((13 * m) - 1) / 5)) + d + (floor(d / 4)) + (floor(c / 4)) - (2 * c));
	int f = static_cast<int>(g) % 7; // cast result of algorithm to int to take modulus
	if (f<0) // if negative add 7
	{
		f += 7;
	}
	return f; // returns 0 to 6 corresponding to sunday to saturday
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// void printcalender()
//
// prints the calender for the entered date
//
//////////////////////////////////////////////////////////////////////////////////////////////
void calendar::printcalendar()
{
	clrscr();
	cout << "The date entered was: " << day << "/" << month << "/" << year << endl;
	cout << endl << setw(8) << "SUNDAY" << setw(8) << "MONDAY" << setw(9) << "TUESDAY" << setw(11) << "WEDNESDAY" << setw(10)
		<< "THURSDAY" << setw(8) << "FRIDAY" << setw(10) << "SATURDAY" << endl;
	int startday = whatdayisfirstofmonth();
	int endday = days_in_month();
	for (int i = 0; i<startday; i++)
	{
		if (i == 0)
		{
			gotoxy(4, 4);
			cout << "-";
		}
		if (i == 1)
		{
			gotoxy(12, 4);
			cout << "-";
		}
		if (i == 2)
		{
			gotoxy(21, 4);
			cout << "-";
		}
		if (i == 3)
		{
			gotoxy(31, 4);
			cout << "-";
		}
		if (i == 4)
		{
			gotoxy(42, 4);
			cout << "-";
		}
		if (i == 5)
		{
			gotoxy(50, 4);
			cout << "-";
		}
	} // end of for loop


	int rows = 4;
	int count = 1;
	for (int j = startday; j<(startday + endday); j++)
	{
		if (j % 7 == 0)
		{
			rows += 2;
			gotoxy(4, rows);
		}
		if (j % 7 == 1) gotoxy(12, rows);
		if (j % 7 == 2) gotoxy(21, rows);
		if (j % 7 == 3) gotoxy(31, rows);
		if (j % 7 == 4) gotoxy(42, rows);
		if (j % 7 == 5) gotoxy(50, rows);
		if (j % 7 == 6) gotoxy(60, rows);
		if (count == day) // set text to bright red if count is the day entered
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
		}
		cout << count;
		count++;
	} // end of for loop
	cout << endl << endl << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// int getDay()
//
// returns the day
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::getDay()
{
	return day;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// int getMonth()
//
// returns the month
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::getMonth()
{
	return month;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// int getYear()
//
// returns the year
//
//////////////////////////////////////////////////////////////////////////////////////////////
int calendar::getYear()
{
	return year;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
// string toString
//
// returns a date as a string
//
//////////////////////////////////////////////////////////////////////////////////////////////
string calendar::toString()
{
	string str;
	str = to_string(day)+"/"+to_string(month) + "/"+to_string(year);
	return str;
}

///////////////////////////////////////////////////////////////////
//Compares 2 dates of type calendar
//returns true if they are equal
///////////////////////////////////////////////////////////////////

bool calendar::equals(calendar d)
{
	if (d.getDay() != this->day)
		return false;
	if (d.getMonth() != this->month)
		return false;
	if (d.getYear() != this->month)
		return false;
	return true;
}

///////////////////////////////////////////////////////////////////
//Compares 2 dates of type calendar
//if return values is <0 this date is chronologically before the argument date
///////////////////////////////////////////////////////////////////
int calendar::compare(calendar d)
{
	if (d.getYear() != this->year)
		return ( this->year-d.getYear());
	if (d.getMonth() != this->month)
		return (this->month - d.getMonth());
	if (d.getDay() != this->day)
		return (this->day - d.getDay());
	return 0;
}
///////////////////////////////////////////////////////////////////
//
//Returns the next date
///////////////////////////////////////////////////////////////////
calendar calendar::getNext()
{
	int nextDay, nextMonth, nextYear;
	nextMonth = month;
	nextYear = year;
	if (this->day == days_in_month())
	{
		nextDay = 1;
		if (month == 12)
		{
			nextMonth = 1;
			nextYear = year + 1;
		}
		else
		{
			nextMonth = month + 1;
		}
	}
	else nextDay = day + 1;
	return calendar(nextDay, nextMonth, nextYear);
}
calendar calendar::getNextWeek()
{
	int nextDay, nextMonth, nextYear;
	nextMonth = month;
	nextYear = year;
	if ((this->day +7) > days_in_month())
	{
		nextDay = (this->day+7)-days_in_month();
		if (month == 12)
		{
			nextMonth = 1;
			nextYear = year + 1;
		}
		else
		{
			nextMonth = month + 1;
		}
	}
	else nextDay = day + 7;
	return calendar(nextDay, nextMonth, nextYear);
}
calendar calendar::getDateNow()
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
	return calendar(day, month, year);
}
