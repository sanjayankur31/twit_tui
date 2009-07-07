#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curses.h>

struct info {
	char username[25];
	char password[25];
	int row,col;
};

typedef struct info infostruct;

