#define _XOPEN_SOURCE 500 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curses.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <stdio.h>

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


void authenticate();
void greet();
int update(infostruct ,char *,char *);
int update_dm(int stat,infostruct ,char *,char *);
int create_table();
int create_table_dm(int);
int drop_table();
int drop_table_dm(int);
double get_max_id();
double get_max_id_dm(int);
int display_time_line(char *, WINDOW *);
int display_dm(int ,char *, WINDOW *);
int send_update(infostruct user,char *);
int send_reply(infostruct user,char * error);
int send_dm(infostruct user,char * error);
int retweet(infostruct user,char * error);
void errorapp(char *error_message,infostruct *user, int error_code);
int parse_file(infostruct *user);
int destroy(int stat,infostruct user, char *error);
