#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <gtk/gtk.h>*/
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <curses.h>

struct temp_tweet{
	char *t_created_at;
	char *t_id;
	char *t_text;
	char *t_source;
	char *t_truncated;
	char *t_reply_to_user_id;
	char *t_reply_to_status_id;
	char *t_favorited;
	char *u_id;
	char *u_screen_name;
};

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct temp_tweet tweet_list;
int db_put_data(tweet_list );
