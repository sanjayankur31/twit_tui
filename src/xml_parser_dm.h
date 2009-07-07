#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <gtk/gtk.h>*/
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <curses.h>

struct temp_dm{
	char *d_id;
	char *d_text;
	char *d_created_at;

	char *ds_id;
	char *ds_screen_name;

	char *dr_id;
	char *dr_screen_name;
};

struct MemoryStruct {
  char *memory;
  size_t size;
};

typedef struct temp_dm dm_list;
int db_put_data_dm(int,dm_list);
