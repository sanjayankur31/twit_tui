#include "send_dm.h"

/* under construction */

int get_dm(infostruct user,char *dm, char *screen_name){
	WINDOW *send_dm;
	WINDOW *msgwind;
	char conf;
	int retval = 2;
	int x,y;
	char *header = "*** Send A Direct Message ***";
	int i = 0;
	
	echo();
	nocbreak();
	send_dm = newwin(10,80,user.row/2, (user.col - 80)/2);
	keypad(send_dm,TRUE);
	msgwind = derwin(send_dm,5,50,4,5 + strlen("Enter Message: "));
	keypad(msgwind,TRUE);
	box(send_dm,0,0);
	wattron(send_dm,A_BOLD);
	mvwprintw(send_dm,2,(getmaxx(send_dm) -strlen(header))/2,"%s",header);
	wattroff(send_dm,A_BOLD);
	mvwprintw(send_dm,4,5,"%s","Receipient Screen name: ");
	wrefresh(send_dm);
	wscanw(send_dm,"%s",screen_name);
	mvwprintw(send_dm,4,5,"                                                                 ");	/* to clear the line.. cannot use wclrtoeol */	
	mvwprintw(send_dm,4,5,"%s","Enter message: ");
	noecho();
	cbreak();
	wrefresh(send_dm);
	getyx(send_dm,y,x);
	touchwin(send_dm);
	wrefresh(msgwind);

	while (1){
		int k;

		if(i > 140)
			wattron(send_dm,COLOR_PAIR(2));
		mvwprintw(send_dm,8,76,"   ");
		mvwprintw(send_dm,8,76,"%i",140 -i);
		if(i > 140)
			wattroff(send_dm,COLOR_PAIR(2));

		wrefresh(send_dm);
		wrefresh(msgwind);

		k = wgetch(msgwind);
		if(k == '\n')
			break;

		if ((k == KEY_BACKSPACE) && i > 0){
			getyx(msgwind,y,x);
			/* if on 1st of line 2, please go back to the last of line 1*/
			if((i) == 50){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if((i) == 100){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if((i) == 150){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else {
				mvwaddch(msgwind,y,x -1,' ');
				wmove(msgwind,y,x-1);
			}
				
			i = i-1;
			
			
		}
		else {
			/* if its at 0 and i press a bcakspace, i dont want anything to happen, without the if, it was printing ^G etc */
			if(k != KEY_BACKSPACE){
				dm[i] = k;
				waddch(msgwind,dm[i]);
				i++;
			}
		}

	}

	dm[i+1] = '\0';
	wrefresh(send_dm);
	echo();
		

	getyx(send_dm,y,x);
	mvwprintw(send_dm,getmaxy(send_dm)- 2,5,"%s","Send? [y/N]");
	wrefresh(send_dm);
	do {
		conf = wgetch(send_dm);
		if(conf == 'Y' || conf == 'y')
			retval = 0;
		else if (conf == 'N' || conf == 'n')
			retval = 1;
	}while(conf != 'y' && conf != 'Y' && conf != 'N' && conf != 'n');
	
	noecho();
	refresh();
	delwin(send_dm);
	return retval;
	nocbreak();
}


int send_dm(infostruct user,char * error){
	char *dm_new  = "http://twitter.com/direct_messages/new.xml";
	CURL *easy_handle;
	CURLcode success;
	char *dm;
	int retval = 0;
	char *dm_send;
	char *screen_name;

	dm = (char*)malloc(300 * sizeof(char));
	if(dm == NULL)
		return 1;

	screen_name = (char *)malloc(25 * sizeof(char));
	if(screen_name == NULL){
		return 1;
	}
	if(get_dm(user,dm,screen_name)){
		free(dm);
		free(screen_name);
		refresh();
		return 2;
	}

	dm_send = (char*)malloc(strlen("text=") + strlen(dm)*sizeof(char) + strlen(screen_name) + strlen("user="));
	sprintf(dm_send,"text=%s&user=%s",dm,screen_name);
	
	easy_handle = curl_easy_init();
	curl_easy_setopt(easy_handle,CURLOPT_NOPROGRESS,1);
	curl_easy_setopt(easy_handle,CURLOPT_VERBOSE, 0);
	curl_easy_setopt(easy_handle,CURLOPT_URL,dm_new);
	curl_easy_setopt(easy_handle,CURLOPT_HTTPAUTH,CURLAUTH_ANY);
	curl_easy_setopt(easy_handle,CURLOPT_ERRORBUFFER,error);
	curl_easy_setopt(easy_handle,CURLOPT_USERNAME,user.username);
	curl_easy_setopt(easy_handle,CURLOPT_PASSWORD,user.password);
	if(strcmp(user.proxy,"0")){
		curl_easy_setopt(easy_handle,CURLOPT_PROXY,user.proxy);

		if(strcmp(user.proxy_port,"0"))
			curl_easy_setopt(easy_handle,CURLOPT_PROXYPORT,user.proxy_port);
		if(strcmp(user.proxy_username,"0"))
			curl_easy_setopt(easy_handle,CURLOPT_PROXYUSERNAME,user.proxy_username);
		if(strcmp(user.proxy_password,"0"))
			curl_easy_setopt(easy_handle,CURLOPT_PROXYPASSWORD,user.proxy_password);
	}
	curl_easy_setopt(easy_handle,CURLOPT_POST,1);
	curl_easy_setopt(easy_handle,CURLOPT_COPYPOSTFIELDS,dm_send);
	if((success = curl_easy_perform(easy_handle)))
		retval = 1;
	
	curl_easy_cleanup(easy_handle);

	free(dm);
	free(screen_name);
	refresh();
	return retval;

}
