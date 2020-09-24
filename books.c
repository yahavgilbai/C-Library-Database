#include <stdio.h>
#include "book.h"

int readln(char s[]) {
	char ch;
	int i = 0;
	int chars_remain = 1;
	while (chars_remain) {
		ch = getchar();
		if ((ch == '\n') || (ch == EOF)) {
			chars_remain = 0;
		}
		else if (i < MAXSTRLEN - 1) {
			s[i] = ch;
			i++;
		}
	}
	s[i] = '\0';
	return i;
}

static void init_tempbook() {
	strcpy(tempbook.name, "");
	strcpy(tempbook.author, "");
	tempbook.pagecount = 0;
	tempbook.rating = 0;
}

static void readbook_data() {
	char bkname[MAXSTRLEN];
	char bkauthor[MAXSTRLEN];
	char bkpages[MAXSTRLEN];
	char bkrating[MAXSTRLEN];
	int pagenum;
	int ratingnum;
	int slen = 0;

	init_tempbook();

	while (slen == 0) {
		printf("Enter book name\n> ");
		slen = readln(bkname);
	};
	slen = 0;
	while (slen == 0) {
		printf("Enter book's author\n> ");
		slen = readln(bkauthor);
	};
	slen = 0;
	while (slen == 0) {
		printf("Enter number of pages\n> ");
		slen = readln(bkpages);
	};
	slen = 0;
	while (slen == 0) {
		printf("Enter rating\n> ");
		slen = readln(bkrating);
	};

	pagenum = atoi(bkpages);
	ratingnum = atoi(bkrating);
	
	strcpy(tempbook.name, bkname);
	strcpy(tempbook.author, bkauthor);
	tempbook.pagecount = pagenum;
	tempbook.rating = ratingnum;
}

void add_book(char* filename) {
	FILE* f = fopen(filename, "a");
	if (f == 0) {
		printf("Cannot write to file: %s\n", filename);
	}
	else {
		readbook_data();
		fwrite(&tempbook, sizeof(BOOK), 1, f);	// write data saved in tempcd at end of file
		fclose(f);
	}
}

int number_of_books_in_db(char* filename) {
	FILE* f = fopen(filename, "r");
	int endpos;
	int numbooks = 0;

	if (f == 0) {
		printf("Cannot open file: %s\n", filename);
	}
	else {
		fseek(f, 0, SEEK_END);					// seek to end of file
		endpos = ftell(f);						// get the current position (now at the end of the file)
		numbooks = endpos / sizeof(BOOK);		// calculate number of records in file
		fclose(f);
	}
	return numbooks;
}


static int load_bkcollection(char* filename) {
	FILE* f = fopen(filename, "r");
	int numbooks = number_of_books_in_db(filename); //check
	int numrecsread = 0;

	if (f == 0) {
		printf("Cannot read file: %s\n", filename);
	}
	else {
		bk_collection = realloc(bk_collection, sizeof(BOOK) * numbooks);		// alloc some memory
		numrecsread = fread(bk_collection, sizeof(BOOK), numbooks, f);			// read all recs into memory
		if (numrecsread != numbooks) {
			printf("Error: %d records in file but %d were read into memory", numbooks, numrecsread);
		}
		fclose(f);
	}
	bkarraylen = numrecsread; // used when saving: I need to know the number of records to be saved
	return numrecsread;
}


void display_bkcollection(char* filename) {
	BOOK thisbk;
	int numrecs = load_bkcollection(filename);
	if (numrecs == 0)
		printf("There are no books in the current collection");
	else {
		for (int i = 0; i < numrecs; i++) {
			thisbk = bk_collection[i];
			printf("Book #%d: '%s' by %s has %d pages. My rating = %d\n", i, thisbk.name, thisbk.author, thisbk.pagecount, thisbk.rating);
		}
	}
}

static void change_bk(char* filename, int bknum) {
	FILE* f = fopen(filename, "r+");
	BOOK* bkptr;
	size_t r;
		
	if (f == 0) {
		printf("Cannot open file: %s\n", filename);
	}
	else {
		bkptr = (BOOK*)malloc(sizeof(BOOK));					// alloc some memory
		r = fseek(f, bknum * sizeof(BOOK), SEEK_SET);			// seek to cdnum record (0 offset is first)
		r = fread(bkptr, sizeof(BOOK), 1, f);					// read cdnum record into memory
		readbook_data();										// get some data for this record
		strcpy(bkptr->name, tempbook.name);					// modify cd data before writing back to disk
		strcpy(bkptr->author, tempbook.author);
		bkptr->pagecount = tempbook.pagecount;
		bkptr->rating = tempbook.rating;
		r = fseek(f, bknum * sizeof(BOOK), SEEK_SET);			// seek again to cdnum record
		r = fwrite(bkptr, sizeof(BOOK), 1, f);				// write the modified record out
		fclose(f);
	}
}


void modify_bk(char* filename) {
	char input[MAXSTRLEN];
	int bknum;
	int error = 0;

	printf("Enter book number to modify:\n> ");
	int slen = readln(input);
	if (slen > 0) {
		if (input[0] == '0') {		// allow 0 as a valid index
			bknum = 0;
		}
		else {
			bknum = atoi(input);	// otherwise atoi returns 0 on conversion error
			if (bknum == 0) {
				error = 1;
			}
		}

	}
	else {
		error = 1;
	}
	if (error) {
		printf("Error: invalid number!\n");
	}
	else if ((bknum < 0) || (bknum > (number_of_books_in_db(filename) - 1))) {
		printf("Error: Cannot find cd number %d\n", bknum);
	}
	else {
		change_bk(filename, bknum);
	}
}
