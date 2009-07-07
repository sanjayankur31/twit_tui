#include "send_reply.h"

/* under construction */

int get_reply_to_post(infostruct user,char *reply, char *sno){
	WINDOW *send_reply;
	WINDOW *msgwind;
	char conf;
	int retval = 2;
	int x,y;
	char *header = "*** Send A Reply ***";
	int i = 0;
	char *screen_name;
	double message_id;
	char *message;
	screen_name = (char *)malloc(25 * sizeof(char));
	if(screen_name == NULL){
		return 1;
	}
	/* i dont need this here.. needed in the retweet */
	message = (char *)malloc(140 * sizeof(char));
	if(message == NULL){
		return 1;
	}

	echo();
	cbreak();
	send_reply = newwin(10,80,user.row/2, (user.col - 80)/2);
	keypad(send_reply,TRUE);
	msgwind = derwin(send_reply,5,50,4,5 + strlen("Enter reply: "));
	keypad(msgwind,TRUE);
	box(send_reply,0,0);
	wattron(send_reply,A_BOLD);
	mvwprintw(send_reply,2,(getmaxx(send_reply) -strlen(header))/2,"%s",header);
	wattroff(send_reply,A_BOLD);
	mvwprintw(send_reply,4,5,"%s","Reply to sno :");
	wrefresh(send_reply);
	wscanw(send_reply,"%s",sno);

	if((message_id = get_message_id(0,sno,&screen_name,&message)) == 1)
		return 1;
	mvwprintw(send_reply,4,5,"%s","Enter reply: ");
	noecho();
	wrefresh(send_reply);
	getyx(send_reply,y,x);
	touchwin(send_reply);
	mvwprintw(msgwind,0,0,"@%s " ,screen_name);
	wrefresh(msgwind);

	while (1){
		int k;

		if(i > (140 - (int)strlen(screen_name) -2))
			wattron(send_reply,COLOR_PAIR(2));
		mvwprintw(send_reply,8,76,"   ");
		mvwprintw(send_reply,8,76,"%i",140 - strlen(screen_name) - 2 -i);
		if(i > (140 - (int)strlen(screen_name) -2))
			wattroff(send_reply,COLOR_PAIR(2));

		wrefresh(send_reply);
		wrefresh(msgwind);

		k = wgetch(msgwind);
		if(k == '\n')
			break;

		if ((k == KEY_BACKSPACE) && i > 0){
			getyx(msgwind,y,x);
			/* if on 1st of line 2, please go back to the last of line 1*/
			if((i + strlen(screen_name) + 2) == 50){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if((i + strlen(screen_name) + 2) == 100){
				mvwaddch(msgwind,y-1,49,' ');
				wmove(msgwind,y-1,49);
			}
			else if((i + strlen(screen_name) + 2 ) == 150){
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
				reply[i] = k;
				waddch(msgwind,reply[i]);
				i++;
			}
		}

	}

	reply[i+1] = '\0';
	wrefresh(send_reply);
	echo();
		

	getyx(send_reply,y,x);
	mvwprintw(send_reply,getmaxy(send_reply)- 2,5,"%s","Send? [y/N]");
	wrefresh(send_reply);
	do {
		conf = wgetch(send_reply);
		if(conf == 'Y' || conf == 'y')
			retval = 0;
		else if (conf == 'N' || conf == 'n')
			retval = 1;
	}while(conf != 'y' && conf != 'Y' && conf != 'N' && conf != 'n');
	
	noecho();
	refresh();
	delwin(send_reply);
	return retval;
}

int get_sno(int stat,infostruct user, char *sno,char *header,char *ask){
	WINDOW *send_reply;
	char conf;
	int retval = 2;
	int x,y;
	char *screen_name;
	double message_id;
	char *message;
	screen_name = (char *)malloc(25 * sizeof(char));
	if(screen_name == NULL){
		return 1;
	}

	message = (char *)malloc(200 * sizeof(char));
	if(message == NULL){
		return 1;
	}

	echo();
	send_reply = newwin(10,80,user.row/2, (user.col - 80)/2);
	keypad(send_reply,TRUE);
	box(send_reply,0,0);
	wattron(send_reply,A_BOLD);
	mvwprintw(send_reply,2,(getmaxx(send_reply) -strlen(header))/2,"%s",header);
	wattroff(send_reply,A_BOLD);
	mvwprintw(send_reply,4,5,"%s",ask);
	wrefresh(send_reply);
	wscanw(send_reply,"%s",sno);

	if((message_id = get_message_id(stat,sno,&screen_name,&message)) == 1)
		return 1;

	getyx(send_reply,y,x);
	mvwprintw(send_reply,getmaxy(send_reply)- 2,5,"%s","Confirm? [y/N]");
	wrefresh(send_reply);
	do {
		conf = wgetch(send_reply);
		if(conf == 'Y' || conf == 'y')
			retval = 0;
		else if (conf == 'N' || conf == 'n')
			retval = 1;
	}while(conf != 'y' && conf != 'Y' && conf != 'N' && conf != 'n');
	
	noecho();
	refresh();
	delwin(send_reply);
	return retval;
}

int send_reply(infostruct user,char * error){
	char *update  = "http://twitter.com/statuses/update.xml";
	CURL *easy_handle;
	CURLcode success;
	char *reply;
	int retval = 0;
	char *status_send;
	FILE *temp_file;
	char sno[4];
	double message_id = 0;
	char *screen_name;
	char *message;

	reply = (char*)malloc(300 * sizeof(char));
	if(get_reply_to_post(user,reply,sno)){
		free(reply);
		refresh();
		return 2;
	}

	screen_name = (char *)malloc(25 * sizeof(char));
	if(screen_name == NULL){
		return 1;
	}

	message = (char *)malloc(300 * sizeof(char));
	if(message == NULL){
		return 1;
	}
	if((message_id = get_message_id(0,sno,&screen_name,&message)) == 1)
		return 1;

	if(!(temp_file = fopen("/dev/null","w"))){
		return 1;
	}
	/* debug 
	printw("%.0f",message_id);
	refresh();
	sleep(2);
	*/


	status_send = (char*)malloc(strlen("status=") + strlen(reply)*sizeof(char) + sizeof(message_id) + 50);
	/*strcpy(status_send,"status=");
	strcat(status_send,reply);*/
	sprintf(status_send,"status=@%s %s&in_reply_to_status_id=%.0f",screen_name,reply,message_id);
	/*sprintf(status_send + strlen(status_send) -1,"&in_reply_to_status_id=%.0f",message_id);*/
	
	/* debug
	clear();
	printw("%s",status_send);
	refresh();
	sleep(2);*/

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

	free(reply);
	free(message);
	free(screen_name);
	free(status_send);
	fclose(temp_file);
	refresh();
	return retval;

}

/* retweet : almost same as send reply.. small alterations.. */
int retweet(infostruct user,char * error){
	char *update  = "http://twitter.com/statuses/update.xml";
	CURL *easy_handle;
	CURLcode success;
	int retval = 0;
	char *status_send;
	FILE *temp_file;
	char sno[4];
	double message_id = 0;
	char *screen_name;
	char *message;
	char *header = "*** Retweet ***";
	char *ask = "Sno to retweet: ";

	if(get_sno(0,user,sno,header,ask)){
		refresh();
		return 2;
	}

	screen_name = (char *)malloc(25 * sizeof(char));
	if(screen_name == NULL){
		return 1;
	}
	message = (char *)malloc(300 * sizeof(char));
	if(message == NULL){
		return 1;
	}

	if((message_id = get_message_id(0,sno,&screen_name,&message)) == 1)
		return 1;

	if(!(temp_file = fopen("/dev/null","w"))){
		return 1;
	}
	/* debug 
	printw("%.0f",message_id);
	refresh();
	sleep(2);
	*/


	status_send = (char*)malloc(strlen("status=") + 50 + strlen(message) * sizeof(char));
	/*strcpy(status_send,"status=");
	strcat(status_send,reply);*/
	sprintf(status_send,"status=RT @%s %s&in_reply_to_status_id=%.0f",screen_name,message,message_id);
	/*sprintf(status_send + strlen(status_send) -1,"&in_reply_to_status_id=%.0f",message_id);*/
	
	/* debug
	clear();
	printw("%s",status_send);
	refresh();
	sleep(2);*/

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

	free(screen_name);
	/*free(status_send);*/
	fclose(temp_file);
	refresh();
	return retval;

}
