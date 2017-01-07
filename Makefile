phonebook.out: phonebook.o
	g++ -o phonebook.out phonebook.o

phonebook.o: phonebook.cpp
	g++ -c phonebook.cpp
	
