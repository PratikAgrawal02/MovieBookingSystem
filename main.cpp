#include <iostream>
#include <fstream>
#include <regex>
#include <typeinfo>
#include "json/json.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
class movieclass
{
private:
	string movieName;
	string movieNumber;
	string id;
	string language;
	string ticketPrice;

public:
	movieclass();
	~movieclass();
	string getname() { return movieName; }
	string getmovienumber() { return movieNumber; }
	string getid() { return id; }
	string getlan() { return language; }
	string getprice() { return ticketPrice; }
	void setname(string a) { movieName = a; }
	void setnumber(string a) { movieNumber = a; }
	void setid(string a) { id = a; }
	void setlang(string a) { language = a; }
	void setprice(string a) { ticketPrice = a; }
};

movieclass::movieclass()
{
}

movieclass::~movieclass()
{
}

Json::Value getMovieRecords()
{
	Json::Value root;
	std::ifstream file("movies.json");
	file >> root;

	file.close();
	return root;
}

void setMovieRecords(Json::Value root)
{
	// Write the output to a file
	std::ofstream outFile;
	outFile.open("movies.json");
	outFile << root;
	outFile.close();
}

string getName()
{
	string str;
	cout << "Enter Movie's Name: ";
	getline(cin, str);

	if (str == "")
	{
		cout << "Enter a name!" << endl;
		getName();
	}
	return str;
}

int movieOptions()
{
	// system("cls");
	cout << "1. Movie Enquiry" << endl;
	cout << "2. New Movie" << endl;
	cout << "3. Modify Movie" << endl;
	cout << "4. Remove Movie" << endl;
	cout << "5. Show All Movies" << endl;
	cout << "6. Book a Movie Ticket" << endl;
	cout << "7. Change Ticket Data" << endl;
	cout << "8. Show all Bought tickets" << endl;
	cout << "0. Exit" << endl;
	cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";

	int selectedOption = 0;
	cin >> selectedOption;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return selectedOption;
}

void checkMovie()
{
	cout << "--- Check Movie ---" << endl;

	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getMovieRecords();
	int len = list.size();

	for (int i = 0; i < len; i++)
	{
		string name = list[i]["name"].asString();
		string movieName = name;
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == str)
		{
			string language = list[i]["language"].asString();
			string movieNum = list[i]["movieNumber"].asString();
			string ticketPrice = list[i]["ticketPrice"].asString();

			cout << "Movie Name: " << movieName << endl;
			cout << "Movie Number: " << movieNum << endl;
			cout << "Language: " << language << endl;
			cout << "Ticket Price: " << ticketPrice << endl;
			return;
		}
	}

	cout << "Record Not Found." << endl;
}

void addMovie()
{
	string movieName, movieNumber, id, language, ticketPrice;
	movieclass movieadd;

	Json::Value records = getMovieRecords();
	int numOfMovies = records.size();
	id = std::to_string(numOfMovies);

	movieNumber = "M100" + std::to_string(numOfMovies);
	movieadd.setid(id);
	movieadd.setnumber(movieNumber);
	cout << endl
		 << "--- Provide Movie Details ---" << endl;
	cout << "Movie Name: ";
	getline(cin, movieName);
	movieadd.setname(movieName);
	cout << "Movie Language: ";
	getline(cin, language);
	movieadd.setlang(language);
	cout << "Ticket Price: ";
	std::getline(std::cin, ticketPrice);
	movieadd.setprice(ticketPrice);

	Json::Value record;
	record["id"] = movieadd.getid();
	record["name"] = movieadd.getname();
	record["movieNumber"] = movieadd.getmovienumber();
	record["language"] = movieadd.getlan();
	record["ticketPrice"] = movieadd.getprice();

	records.append(record);
	setMovieRecords(records);

	cout << movieName << " Movie added. " << endl;
	cout << "Movie Number: " << movieNumber << endl;
}

void updateMovie()
{
	cout << endl
		 << "--- Update Movie ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getMovieRecords();
	int len = records.size();
	bool found = false;

	for (int i = 0; i < len; i++)
	{
		string value = records[i]["name"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (str == value)
		{
			string name, language, ticketPrice;

			cout << "New Name: ";
			std::getline(std::cin, name);

			cout << "Movie Language: ";
			std::getline(std::cin, language);

			cout << "Ticket Price: ";
			std::getline(std::cin, ticketPrice);

			records[i]["name"] = name;
			records[i]["language"] = language;
			records[i]["ticketPrice"] = ticketPrice;

			cout << "Updated" << endl;
			found = true;
			break;
		}
	}

	if (!found)
	{
		cout << "Record Not Found." << endl;
	}
	else
		setMovieRecords(records);
}

void removeMovie()
{
	string movieNum;
	cout << endl
		 << "--- Remove Movie ---" << endl;
	cout << "Movie's Number to remove " << endl;
	getline(cin, movieNum);

	string &str = movieNum;
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getMovieRecords();
	int len = records.size();

	Json::Value newRecords;
	bool movieExists = false;

	for (int i = 0; i < len; i++)
	{
		string value = records[i]["movieNumber"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (str == value)
		{
			movieExists = true;
			continue;
		}
		newRecords.append(records[i]);
	}

	if (movieExists)
	{
		setMovieRecords(newRecords);
		cout << "Movie Removed";
	}
	else
	{
		cout << "Movie does not exist";
	}
}

void showMovies()
{
	cout << "--- List of Movies ---" << endl;

	Json::Value list = getMovieRecords();
	int len = list.size();

	cout << "No. | Movie Name | Movie No. | Language | Ticket Price " << endl;
	for (int i = 0; i < len; i++)
	{
		string name = list[i]["name"].asString();
		string num = list[i]["movieNumber"].asString();
		string language = list[i]["language"].asString();
		string ticketPrice = list[i]["ticketPrice"].asString();

		int padding = 12 - name.length();
		if (padding > 0)
		{
			for (int j = 0; j < padding; j++)
			{
				name += " ";
			}
		}

		cout << i + 1 << ".   " << name << "  " << num << "    " << language << "   Rs." << ticketPrice << endl;
	}
}

// Ticket
Json::Value getTicketRecords()
{

	Json::Value root;
	std::ifstream file("tickets.json");
	file >> root;

	file.close();
	return root;
}

void setTicketRecords(Json::Value root)
{
	// Write the output to a file
	std::ofstream outFile;
	outFile.open("tickets.json");
	outFile << root;
	outFile.close();
}

class Ticketer
{
private:
	string name, phone, address;

public:
	Ticketer()
	{
	}
	string getname() { return name; }
	string getphone() { return phone; }
	string getaddress() { return address; }

	friend void addTicketRecord();
	friend void changeTicketRecords();
};

void addTicketRecord()
{
	string movieNumber, MovieDate, MovieTime, id;
	Ticketer r;

	Json::Value records = getTicketRecords();
	int numOfTickets = records.size();
	id = std::to_string(numOfTickets);

	cout << endl
		 << "--- Provide Ticket Details ---" << endl;
	cout << "Movie Number: ";
	getline(cin, movieNumber);
	cout << "Customer's Name: ";
	getline(cin, r.name);
	cout << "Customer's Phone: ";
	getline(cin, r.phone);
	cout << "Customer's Address: ";
	getline(cin, r.address);
	// dd-mm-yy . format for date . eg: 01-03-2021
	cout << "Date of Show: ";
	getline(cin, MovieDate);
	cout << "Timing of Show: ";
	getline(cin, MovieTime);

	Json::Value record;
	record["id"] = id;
	record["ticketer"]["name"] = r.name;
	record["ticketer"]["phone"] = r.phone;
	record["ticketer"]["address"] = r.address;
	record["movie"]["movieNumber"] = movieNumber;
	record["MovieDate"] = MovieDate;
	record["MovieTime"] = MovieTime;

	records.append(record);
	setTicketRecords(records);

	cout << endl
		 << r.name << " booked a Movie Ticket. " << endl;
	cout << "Movie Number: " << movieNumber << endl;
}

void showTicketRecords()
{
	cout << "--- Ticket Records ---" << endl;

	Json::Value list = getTicketRecords();
	int len = list.size();

	cout << "ID. | "
		 << "Customer's Name | "
		 << "Movie No. | "
		 << "Movie Timing: " << endl;
	for (int i = 0; i < len; i++)
	{
		string name = list[i]["ticketer"]["name"].asString();
		string num = list[i]["movie"]["movieNumber"].asString();
		string MovieTime = list[i]["MovieTime"].asString();
		string ticketid = list[i]["id"].asString();
		int padding = 18 - name.length();
		if (padding > 0)
		{
			for (int j = 0; j < padding; j++)
			{
				name += " ";
			}
		}

		cout << ticketid << ". " << name << "  " << num << "           " << MovieTime << endl;
	}
}

void changeTicketRecords()
{
	string movieNumber, MovieDate, MovieTime;
	Ticketer r;
	bool found = false;

	cout << endl
		 << "--- Update Ticket Details ---" << endl;

	string str;
	cout << "Ticket Id: ";
	getline(cin, str);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getTicketRecords();
	int len = records.size();

	for (int i = 0; i < len; i++)
	{
		string value = records[i]["id"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (str == value)
		{
			cout << endl
				 << "--- Provide New Ticket Details ---" << endl;
			cout << "New Movie Number: ";
			getline(cin, movieNumber);
			cout << "New Customer's Name: ";
			getline(cin, r.name);
			cout << "New Customer's Phone: ";
			getline(cin, r.phone);
			cout << "New Customer's Address: ";
			getline(cin, r.address);

			// dd-mm-yy . format for date . eg: 01-03-2021
			cout << "Movie Show Date: ";
			getline(cin, MovieDate);
			cout << "Movie Show Timing: ";
			getline(cin, MovieTime);

			records[i]["ticketer"]["name"] = r.getname();
			records[i]["ticketer"]["phone"] = r.phone;
			records[i]["ticketer"]["address"] = r.address;
			records[i]["movie"]["movieNumber"] = movieNumber;
			records[i]["MovieDate"] = MovieDate;
			records[i]["MovieTime"] = MovieTime;

			setTicketRecords(records);
			cout << "Update Successful " << endl;
			return;
		}
	}

	cout << "Ticket not found." << endl;
}

void actions(int &option)
{
	switch (option)
	{
	case 1:
		checkMovie();
		break;
	case 2:
		addMovie();
		break;
	case 3:
		updateMovie();
		break;
	case 4:
		removeMovie();
		break;
	case 5:
		showMovies();
		break;
	case 6:
		addTicketRecord();
		break;
	case 7:
		changeTicketRecords();
		break;
	case 8:
		showTicketRecords();
		break;
	}
}

void home()
{
	int option = movieOptions();
	if (option != 0 && option <= 8)
	{
		actions(option);
	}
	else if (option > 8)
	{
		cout << endl
			 << "!!! Enter Valid Option !!!" << endl;
		option = movieOptions();
	}
	else
	{
		cout << "ThankYou for choosing Us!\nHave a Great day Good Bye!" << endl;
		exit(0);
	}
}

int main()
{

	cout << "*** WELCOME ***";
	string yesOrNo;
	while (true)
	{
		system("cls");
		cout << endl
			 << "--- Movie Ticket Booking System ---" << endl;
		home();
		cout << endl
			 << "Continue? (y/n) :";
		cin >> yesOrNo;
		if (yesOrNo != "y")
			break;
	}

	cout << "ThankYou for choosing Us!\nHave a Great day Good Bye!" << endl;
}