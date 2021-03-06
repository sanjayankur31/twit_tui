#include "send_update.h"

int get_message_to_post(infostruct user,char *message){
	WINDOW *send_message;
	WINDOW *msgwind;
	char conf;
	int retval = 2;
	int x,y;
	char *header = "*** Send An Update ***";
	int i = 0;

	
	noecho();
	cbreak();
	send_message = newwin(10,80,user.row/2, (user.col - 80)/2);
	msgwind = derwin(send_message,5,50,4,5 + strlen("Enter status update: "));
	keypad(msgwind,TRUE);
	box(send_message,0,0);
	wattron(send_message,A_BOLD);
	mvwprintw(send_message,2,(getmaxx(send_message) -strlen(header))/2,"%s",header);
	wattroff(send_message,A_BOLD);
	mvwprintw(send_message,5,5,"%s","Enter status update: ");
	wrefresh(send_message);
	getyx(send_message,y,x);
	wmove(msgwind,1,0);
	touchwin(send_message);
	wrefresh(msgwind);

	while (1){
		int k;

		if(i > 140)
			wattron(send_message,COLOR_PAIR(2));
		mvwprintw(send_message,8,76,"   ");
		mvwprintw(send_message,8,76,"%i",140 - i);
		if( i > 140)
			wattroff(send_message,COLOR_PAIR(2));

		wrefresh(send_message);
		wrefresh(msgwind);

		k = wgetch(msgwind);
		if(k == '\n')
			break;

		if ((k == KEY_BACKSPACE) && i > 0){
			getyx(msgwind,y,x);
			/* if on 1st of line 2, please go back to the last of line 1*/
			if( i == 50){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if(i == 100){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if(i == 150){
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
				message[i] = k;
				waddch(msgwind,message[i]);
				i++;
			}
		}


	}

	message[i+1] = '\0';
	wrefresh(send_message);
	echo();

	getyx(send_message,y,x);
	mvwprintw(send_message,getmaxy(send_message)- 2,5,"%s","Send? [y/N]");
	wrefresh(send_message);

	do {
		conf = wgetch(send_message);
		if(conf == 'Y' || conf == 'y')
			retval = 0;
		else if (conf == 'N' || conf == 'n')
			retval = 1;
	}while(conf != 'y' && conf != 'Y' && conf != 'N' && conf != 'n');
	
	noecho();
	refresh();
	delwin(send_message);
	return retval;
}


int send_update(infostruct user,char * error){
	char *update  = "http://twitter.com/statuses/update.xml";
	CURL *easy_handle;
	CURLcode success;
	char *message;
	int retval = 0;
	char *status_send;
	FILE *temp_file;

	message = (char*)malloc(300 * sizeof(char));

	if(get_message_to_post(user,message)){
		free(message);
		refresh();
		return 2;
	}
	/* if i ever feel like logging output.. wil use it for errors etc sometime */
	if(!(temp_file = fopen("/dev/null","w"))){
		return 1;
	}

	status_send = (char*)malloc(strlen("status=") + strlen(message) * (sizeof(char)));
	strcpy(status_send,"status=");
	strcat(status_send,message);

	easy_handle = curl_easy_init();
	curl_easy_setopt(easy_handle,CURLOPT_NOPROGRESS,1);
	curl_easy_setopt(easy_handle,CURLOPT_VERBOSE, 0);
	curl_easy_setopt(easy_handle,CURLOPT_URL,update);
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
	curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,temp_file);	
	curl_easy_setopt(easy_handle,CURLOPT_POST,1);
	curl_easy_setopt(easy_handle,CURLOPT_COPYPOSTFIELDS,status_send);
	if((success = curl_easy_perform(easy_handle)))
		retval = 1;
	
	curl_easy_cleanup(easy_handle);

	free(message);
	fclose(temp_file);
	refresh();
	return retval;

}
