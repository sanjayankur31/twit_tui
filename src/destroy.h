#include <curses.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
double get_message_id(int,char *,char **,char **);
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
int get_sno(int,infostruct user, char *sno,char *header,char *ask);
