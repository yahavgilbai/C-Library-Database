#define MAXSTRLEN 200
typedef char Str50[50];
	
typedef struct book {
	Str50 name;
	Str50 author;
	int pagecount;
	int rating;
} BOOK;

BOOK tempbook;
BOOK* bk_collection;
int bkarraylen;

int readln(char[]);
void display_bkcollection(char*);
int number_of_books_in_db(char*);
void add_book(char*);
void modify_bk(char*);