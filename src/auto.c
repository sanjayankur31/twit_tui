/* for autologin and proxy support
 * make a file ~/.twit_tui and ill parse it and fill the user struct up
 * data is in plain text so make sure only your user has access
 * */

#include "auto.h"

int parse_file(infostruct *user){
	FILE *conf_handle;
	char *home;
	char *buff_lval;
	char *buff_rval;

	char *conf_file = "twit_tui.conf";
	int i;
	
	if((home = (char *) malloc(50)) == NULL)
		return E_MALLOC;

	sprintf(home,"%s/%s",getenv("HOME"),conf_file);
	
	/* only if there's a file */

	
	if((conf_handle = fopen(home,"r")) == NULL){
		free(home);
		if(errno == ENOENT)
			return ENOENT;
		else 
			return errno;
	}
	else {	
		clear();
		printw("Conf file found..");
		refresh();
		sleep(1);
		printw("\nPlease wait..Parsing");
		refresh();

		if((buff_lval = (char *) malloc(50)) == NULL)
			return E_MALLOC;
		if((buff_rval = (char *) malloc(50)) == NULL)
			return E_MALLOC;

		while (!feof(conf_handle)){

			strcpy(buff_lval,"0");
			strcpy(buff_rval,"0");
			/* username */
			fscanf(conf_handle,"%s = %s",buff_lval,buff_rval);

			if(!(strncmp("username",buff_lval,strlen("username")))){
				strcpy(user->username,buff_rval);
				printw("\nFound Username : %s",user->username);
				refresh();
			}
			if(!(strncmp("proxy",buff_lval,strlen("proxy")))){
				strcpy(user->proxy,buff_rval);
				printw("\nFound Proxy : %s",user->proxy);
				refresh();
			}
			if(!(strncmp("proxy_username",buff_lval,strlen("proxy_username")))){
				strcpy(user->proxy_username,buff_rval);
				printw("\nFound Proxy Username : %s",user->proxy_username);
				refresh();
			}
			if(!(strncmp("proxy_port",buff_lval,strlen("proxy_port")))){
				strcpy(user->proxy_port,buff_rval);
				printw("\nFound Username : %s",user->proxy_port);
				refresh();
			}

			if(!(strncmp("password",buff_lval,strlen("password")))){
				strcpy(user->password,buff_rval);
				printw("\nFound Password : ");
				/* just for nuts */
				for(i = 0; i < (int)strlen(user->password); i++){
					usleep(50000);
					printw("*");
					refresh();
				}
			}
			if(!(strncmp("proxy_password",buff_lval,strlen("proxy_password")))){
				strcpy(user->proxy_password,buff_rval);
				printw("\nFound Proxy Password : ");
				/* just for nuts */
				for(i = 0; i < (int)strlen(user->proxy_password); i++){
					usleep(50000);
					printw("*");
					refresh();
				}
			}

		}
	}
	free(buff_lval);
	free(buff_rval);

/*	printw("%s:%slalalala",user->username,user->password);
	refresh();
	sleep(2);
	*/

	return 0;
}





