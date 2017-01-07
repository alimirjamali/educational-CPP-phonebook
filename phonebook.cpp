/********************************************************************************/ 
/*                                                                   		*/
/*  Very simple phonebook programing in c++, using linked lists (not OOP)	*/
/*                                                                		*/
/*  This was a university task by Prof. ..... of				*/
/*  - Sharif University of Technology - International Campus - Kish Island -	*/
/*  Please refer to project PDF for more information and project rules		*/
/*                                                         			*/
/*  This code is for educational purposes and has no other use.   		*/
/*  I helped a student to write this program and document it.			*/
/*  Note: Indexing part was left to the student to practice.			*/
/*										*/
/*  Copyright 2017 by Ali Mirjamali <ali.mirjamali@gmail.com>          		*/
/*										*/
/*  This program is free software: you can redistribute it and/or modify	*/
/*  it under the terms of the GNU General Public License as published by	*/
/*  the Free Software Foundation, either version 3 of the License, or		*/
/*  (at your option) any later version.						*/
/*										*/
/*  This program is distributed in the hope that it will be useful,		*/
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*  GNU General Public License for more details.				*/
/*										*/
/*  You should have received a copy of the GNU General Public License		*/
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*/
/*										*/
/********************************************************************************/ 

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Point E of project:
// --- phone book must be saved in a file called "phone_book.txt". ---
const char* phonebook_filename = "phone_book.txt";

// Point 1.c of project:
// --- Contact Group (one of: Family, Friends, Colleagues, VIP and Others). ---
// Notes: Enumerating Contact Groups for easier accessing
enum groups {Family, Friends, Colleagues, VIP, Others};

// Point 1 of project:
// --- Every entry in the phonebook has following information: ---
// --- a. Phone number. 
// --- b. Contact name (CAPITAL letters).
// --- c. Contact Group 
// --- which are to be taken care of using a structure called contact_info. ---
struct contact_info
{
	// string is preferred for phone numbers to allow + sign, extentions and - character
	string phone_number;
	string contact_name;
	groups contact_group;
	// Point 2 of project:
	// --- Therefore, added to information listed in item#1, the contact_info structure should be equipped ---
	// --- with a pointer to contact_info which holds the address of the next element in the list. Last ---
	// --- element has a pointer with the value of zero (pointing nowhere). ---
	contact_info* next_element=NULL;
};

// Point 2 of project:
// --- The phonebook is to be maintained as a linked list. A linked list is a chain of items which have ---
// --- links to each other. Figure (1) shows a typical linked list. Note that there is a beginning to the list ---
// --- (to which no arrow points) and an end to the list (from which no arrow is emanating). ---
contact_info* phonebook=NULL;

// Point 3 of project:
// --- There must be variable which holds the number of entries in the phonebook called N_entries. ---
unsigned int N_entries = 0;

// Point 5 of project:
// --- There must be a one-dimensional 26 element array, called look_up, whose elements are ---
// --- pointers to contact_info structure. In ith element of the look_up , pointer to the first phone ---
// --- book entry beginning with ith letter must be stored (A is the zeroth letter, B is the first letter, C ---
// --- is the second letter, , Z is the 25th letter) ---
// --- If no entry with that letter is recorded, zero must be used ---
contact_info* look_up[26] = {NULL};

void upcase (string&);			// This is the function prototype which would convert an string to uppercase
contact_info* create_entry();		// This is the function prototype which gets the input from user and stores in into a new contact object.
void contact_insert(contact_info*);	// This is the function prototype which inserts the new contact object into the phonebook
// The below function is used for both point C & D of project. It will retrun NULL (zero) if it does not find a matching name
contact_info* contact_find(string);	// This is the function prototype to search and find a contact object based on Name
void contact_show(contact_info);	// This is the function prototype to show the information of a contact object
void contact_find_show_delete();	// This is the function prototype to find and delete contacts (point C of project)
void contact_find_show();		// This is the function prototype to find and show a Name in phonebook (plus some Error handling)
void phonebook_save();			// This is the function prototype to store data into file (point E & F of project)
void phonebook_load();			// This is the function prototype to load the data file (optional, was not asked in the project)

int main()
{
	// Reading phonebook file (optional part, not a project task)
	phonebook_load();
	// Point A of project:
	// --- After running the program, it must enter an infinity loop which provides the below menu: ---
	char userchoice;
	do
	{
		cout << endl;
		cout << "a. Create a new entry." << endl;
		cout << "b. Delete an entry." << endl;
		cout << "c. Find an entry and show its information. " << endl;
		cout << "d. Save the whole phonebook in a file. " << endl;
		cout << "e. Exit. " << endl;
		cout << endl << "Please enter your choice: ";
		cin >> userchoice;
		switch (userchoice)
		{
		// Point B of project:
		// --- If the user chooses (a), he must be asked to enter the related information ---
		// This is implemented in two functions
		case 'a': case 'A':
			contact_insert(create_entry());
			break;
		// Point C of project
		// --- If the user chooses (b), he must be asked for the name of the person whose entry is to be deleted...
		// This is implemented in 3 functions (find, show & delete);
		case 'b': case 'B':
			contact_find_show_delete();
			break;
		// Point D of project:
		// --- If the user chooses (c), he must be asked for the name of the person.
		// This is implemented in 2 functions
		case 'c': case 'C':
			contact_find_show();
			break;
		// Point E & F of project
		// --- the whole phone book must be saved ---  the phone book must be stored ---
		case 'd': case 'D': case 'e': case 'E':
			phonebook_save();
			break;
		// Otherwise we have an invalid choice and should show an error message
		default:
			cout << endl << "!!! Invalid choice !!!" << endl;
		}
	// Point F of project
	// --- If the user chooses (e), ... the program must be terminated ---
	} while ((userchoice != 'e') && (userchoice != 'E'));
	cout << "Exiting program..." << endl;
	return 0;
}

// Point B of project
// --- Everything, even if entered in small letters, must be stored in CAPITAL letters.
void upcase(string &str)
{
	for (int i=0; i<str.length(); i++) str[i]=toupper(str[i]);
}

// Point B of project
// --- If the user chooses (a), he must be asked to enter the related information ---
// --- (phone number, contact name and group). 
contact_info* create_entry()
{
	// Creating a new blank contact
	contact_info* new_entry = new contact_info;
	cout << "Please enter name: ";
	cin.ignore();	// We are clearing input buffer to use getline function
	getline (cin,new_entry->contact_name);
	upcase(new_entry->contact_name);
	cout << "Please enter phone number: ";
	getline (cin,new_entry->phone_number);
	upcase(new_entry->phone_number);
	cout << "Please enter group" << endl;
	cout << "1, f or F = Family" << endl;
	cout << "2, r or R = Friends" << endl;
	cout << "3, c or C = Colleagues" << endl;
	cout << "4, c or c = VIP" << endl;
	cout << "or enter any other character for (Others) group" << endl;
	cout << "Your choice: ";
	char groupchoice;
	cin >> groupchoice;
	switch (groupchoice)
	{
	case '1': case 'f': case 'F':
		new_entry->contact_group=Family;
		break;
	case '2': case 'r': case 'R':
		new_entry->contact_group=Friends;
		break;
	case '3': case 'c': case 'C':
		new_entry->contact_group=Colleagues;
		break;
	case '4': case 'v': case 'V':
		new_entry->contact_group=VIP;
		break;
	default:
		new_entry->contact_group=Others;
	}
	new_entry->next_element=NULL;
	cout << "New contact successfully created." << endl;
	return new_entry;
}

// Point 6 of project:
// --- Every time a new entry is introduced, the look_up array must be consulted to find out where, in 
// --- the linked list, is the suitable place to put the entry. If an entry for BASTANI is to be created, it 
// --- must be inserted so that, there is a pointer from BAHMANI to BASTANI and a pointer from 
// --- BASTANI to BEIGI (Figure 4). Note that if the name was BAGHERI the look_up[1] needed 
// --- modification, too; because, "BAGHERI" < "BAHMANI" and look_up[1] needed to be modified to 
// --- hold the pointer to the entry corresponding to BAGHERI. 
void contact_insert(contact_info* newcontact)
{
	// If the new item is the 1st item, we have special case
	if (N_entries==0 || phonebook->contact_name.compare(newcontact->contact_name)>0)
	{
		newcontact->next_element=phonebook;
		phonebook=newcontact;
	}
	else
	{
		contact_info* previous=phonebook;
		contact_info* next=phonebook->next_element;
		while(next!=NULL)
		{
			if (newcontact->contact_name.compare(next->contact_name)<0) break;
			previous=next;
			next=next->next_element;
		}
		previous->next_element=newcontact;
		newcontact->next_element=next;		
	}	
	N_entries++;
}

// This function is used in both points of C & D of the project
// It searchs & returns the pointer to contact based on contact name (or NULL if not found).
contact_info* contact_find(string Name)
{
	contact_info* contact = phonebook;
	while (contact != NULL)
	{
		if (Name.compare(contact->contact_name)==0)
			return contact;
		else
			contact=contact->next_element;
	}
	return NULL;
}

// This function is used in both points of C & D of the project
// It prints a contact information.
void contact_show(contact_info* contact)
{
	cout << "Contact Name: " << contact->contact_name << endl;
	cout << "Contact Number: " << contact->phone_number << endl;
	cout << "Contact Group: ";
	switch (contact->contact_group)
	{
		case Family: cout << "FAMILY" << endl; break;
		case Friends: cout << "FRIENDS" << endl; break;
		case Colleagues: cout << "COLLEAGUES" << endl; break;
		case VIP: cout << "VIP" << endl; break;
		default: cout << "OTHERS" << endl;
	}
}

// Point C of project (Finding, Showing, confirming and deleting a contact)
void contact_find_show_delete()
{
	cout << "Please enter contact name to be deleted: ";
	string Name;
	cin.ignore();
	getline (cin,Name);
	upcase(Name);
	cout << endl;
	contact_info* contact = contact_find(Name);
	if(contact != NULL)
	{
		cout << "We are going to delete following contact:" << endl;
		contact_show(contact);
		cout << "Enter Y/y to confirm delete:";
		char confirm;
		cin >> confirm;
		if (confirm == 'Y' || confirm == 'y')
		{
			// If the 1st Item is to be deleted, we have special case
			if (contact==phonebook)
			{
				phonebook=phonebook->next_element;
			}
			// Else, we should find previous contact for linking
			else
			{
				contact_info* previous=phonebook;
				while(previous->next_element != contact) previous=previous->next_element;
				previous->next_element=contact->next_element;				
			}
			// Free memory for deleted contact
			delete contact;
			N_entries--;
			cout << "Contact successfully deleted" << endl;
		}
		else cout << "Did not delete contact." << endl;
	}
	else cout << "Did not found " << Name << " in phonebook!!!" << endl;
}

// Point C of project
void contact_find_show()
{
	cout << "Please enter contact name to be searched for: ";
	string Name;
	cin.ignore();
	getline (cin,Name);
	cout << endl;
	upcase(Name);
	contact_info* contact = contact_find(Name);
	if(contact != NULL)
		contact_show(contact);
	else
		cout << "Did not found " << Name << " in phonebook!!!" << endl;
}

// Point E (and F) of project
// --- whole phone book must be saved in a file
void phonebook_save()
{
	ofstream phonebook_file;
	// --- called "phone_book.txt".
	phonebook_file.open(phonebook_filename);
	cout << "Writing " << N_entries << " records to " << phonebook_filename << " ..." << endl;
	// --- On the first line the number of entries must be saved
	phonebook_file << N_entries << endl;
	contact_info* current_item=phonebook;
	while (current_item != NULL)
	{
		// --- Entries must be separated from each other and from the first line by a line of twenty *s (********************).
		phonebook_file << "********************" << endl;
		// ---  For each entry, the name must be stored on the first line
		phonebook_file << current_item->contact_name << endl;
		phonebook_file << current_item->phone_number << endl;
		switch (current_item->contact_group)
		{
			case Family: phonebook_file << "FAMILY" << endl; break;
			case Friends: phonebook_file << "FRIENDS" << endl; break;
			case Colleagues: phonebook_file << "COLLEAGUES" << endl; break;
			case VIP: phonebook_file << "VIP" << endl; break;
			default: phonebook_file << "OTHERS" << endl;
		}
		current_item = current_item->next_element;
	}
	phonebook_file.close();
}

// This function reads the default contact file (optional part. Not a project task)
void phonebook_load()
{
	ifstream phonebook_file;
	phonebook_file.open(phonebook_filename);
	if(!phonebook_file.is_open())
	{
		cout << "Phonebook file could not be openned !!!" << endl;
	} else
	{
		phonebook_file >> N_entries;
		contact_info** previous=&phonebook;
		string text;
		// Skiping a carriage return here (because of previous reading of Integer number)
		getline (phonebook_file,text);
		cout << "Reading " << N_entries << " of contact records..." << endl;
		for (int i=0;i<N_entries;i++)
		{
			contact_info* new_entry = new contact_info;
			// Skipping ********** line
			getline (phonebook_file,text);
			getline (phonebook_file,new_entry->contact_name);
			getline (phonebook_file,new_entry->phone_number);
			getline (phonebook_file,text);
			if      (text.compare("FAMILY") == 0)  	new_entry->contact_group=Family;
			else if (text.compare("FRIENDS") == 0)	new_entry->contact_group=Friends;
			else if (text.compare("COLLEAGUES")==0)	new_entry->contact_group=Colleagues;
			else if (text.compare("VIP") == 0)	new_entry->contact_group=VIP;
			else new_entry->contact_group=Others;
			new_entry->next_element=NULL;
			*previous=new_entry;
			previous=&new_entry->next_element;
		}
		phonebook_file.close();
	}
}


