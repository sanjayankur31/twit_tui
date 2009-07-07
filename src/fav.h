#include <curses.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct info {
	char username[25];
	char password[25];
	int row,col;
};

typedef struct info infostruct;
int get_sno(int stat,infostruct user, char *sno,char *header,char *ask);
