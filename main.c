#include <stdio.h>
#include "book.h"
#define filename "librarydatabase.txt"


char getcommand() {
	char command[MAXSTRLEN];
	int commandlen;
	char c;

	commandlen = readln(command);
	if (commandlen != 1) {
		c = 'x';
	}
	else {
		c = command[0];
	}
	return c;
}

int main() {
	int cont = 1;
	while (cont) {
		char command;
		printf("\nEnter a command or 'q' to quit.");
		printf("\nCommands: 'a'=add book, 'd'=display books, 'm'=modify book info");
		printf("\n          'n'=number of books\n> ");
		switch (getcommand()) {
		case 'a':
			printf("Add book\n");
			add_book(filename);
			break;
		case 'd':
			printf("Display books\n");
			display_bkcollection(filename);
			break;
		case 'm':
			printf("Modify book\n");
			modify_bk(filename);
			break;
		case 'n':
			printf("Number of books\n");
			printf("Database contains %d books\n", number_of_books_in_db(filename));
			break;
		case 'q':
			printf("Ending...\n");
			cont = 0;
			break;
		default:
			printf("Invalid command.\n");
			break;
		}
	}
	return 0;
}