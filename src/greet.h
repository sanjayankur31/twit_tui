#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>

struct info {
	char username[25];
	char password[25];
	char proxy[50];
	char proxy_port[20];
	char proxy_username[25];
	char proxy_password[25];
	int row,col;
};

typedef struct info infostruct;
