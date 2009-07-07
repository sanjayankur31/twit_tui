#include "make_dir.h"

struct dir make_dir(){
	char *dir_name = ".twit_tui";
	struct dir home;
	

	/* creating the  app homedir */

	sprintf(home.dir_name,"%s/%s",getenv("HOME"),dir_name);

	home.dir_len = strlen(home.dir_name);

	if(!mkdir(home.dir_name,0733)){
	/*	printf("directory created\n");*/
	}

	/*if(errno == EEXIST)
		printf("Directory already exists\n");*/

	return (home);	/* returns the final dir name */
}
