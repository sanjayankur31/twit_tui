#define _GNU_SOURCE
#include <unistd.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* make_dir();

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

struct temp_dm{
	char *d_id;
	char *d_text;
	char *d_created_at;

	char *ds_id;
	char *ds_screen_name;

	char *dr_id;
	char *dr_screen_name;
};

typedef struct temp_tweet tweet_list;
typedef struct temp_dm dm_list;
