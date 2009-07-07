#include <stdlib.h>
#include <string.h>
#include <curses.h>
#define TABLE_EXISTS 	1
#define TABLE_NEXISTS	0
#include <stdlib.h>
#include <curl/curl.h>
#include <errno.h>

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

struct MemoryStruct {
  char *memory;
  size_t size;
};

int xml_parse_dm(int,struct MemoryStruct,char *);
int display_dm(char *, WINDOW *);
int create_table_dm();
