#define _XOPEN_SOURCE	501
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <curses.h>
#include "errorcodes.h"

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
