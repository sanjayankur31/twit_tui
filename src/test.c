#include "test.h"
#include "errorcodes.h"

int main(){
	infostruct user;
	int choice;
	char *time_line  = "http://twitter.com/statuses/friends_timeline.xml";
	char *dm = "http://twitter.com/direct_messages.xml";
	char *dm_sent = "http://twitter.com/direct_messages/sent.xml";
	char *xml_to_download;
	char error[CURL_ERROR_SIZE + 20];
	int stat;
	WINDOW *main_pad;
	WINDOW *d_pad;
	WINDOW *d_pad_sent;
	int pad_use = 0;
	int ret_code;
	int *line_index;
	
	curl_global_init(CURL_GLOBAL_ALL);		/* curl initialized, code now */
	initscr();
	idlok(stdscr,TRUE);
	keypad(stdscr, TRUE);
	scrollok(stdscr, TRUE);	
	start_color();
	if(!has_colors()){
		printf("Your terminal does not support color. Exiting\n");
		endwin();
		exit(E_TERMINAL_NO_COLOR);
	}
	use_default_colors();

	getmaxyx(stdscr,user.row,user.col);

	/* color pairs defs to go here */
	init_pair(1,COLOR_CYAN,-1);
	init_pair(2,COLOR_RED,-1);
	init_pair(3,COLOR_BLUE,-1);
	init_pair(4,COLOR_BLACK,-1);
	init_pair(5,COLOR_MAGENTA,-1);
	init_pair(6,COLOR_YELLOW,-1);
	init_pair(7,COLOR_GREEN,COLOR_WHITE);

	/* make the database and the table */	
	if((ret_code = create_table()))
		errorapp("Error while creating tweet db table",&user,ret_code);

	if((ret_code = create_table_dm(1)))
		errorapp("Error while creating dm db table",&user,ret_code);

	if((ret_code = create_table_dm(2)))
		errorapp("Error while creating dm sent db table",&user,ret_code);

	/* the app begins doing things here */
	clear();
	refresh();
	strcpy(user.username,"0");
	strcpy(user.password,"0");
	strcpy(user.proxy,"0");
	strcpy(user.proxy_port,"0");
	strcpy(user.proxy_username,"0");
	strcpy(user.proxy_password,"0");

	greet(user);
	sleep(2);
	refresh();
	parse_file(&user);

	if(!strcmp(user.username,"0"))
		authenticate(&user);
	noecho();
	
	main_pad = newpad(1000,COLS);
	

	if(main_pad == 0)
		errorapp("Error while creating tweet pad, Not enough memory?",&user,E_PAD_NO_CREATE);
	
	d_pad = newpad(1000,COLS);

	if(d_pad == 0)
		errorapp("Error while creating dm pad, Not enough Memory?",&user,E_PAD_NO_CREATE);

	d_pad_sent = newpad(1000,COLS);

	if(d_pad_sent == 0)
		errorapp("Error while creating dm pad, Not enough Memory?",&user,E_PAD_NO_CREATE);

	/* im assuming that a valid username password will work every time :D */
	if((ret_code = update_dm(1,user,dm,error))){
		if (ret_code == CURLE_HTTP_RETURNED_ERROR){
			strcat(error,"\nPlease check your user name, password and retry");
		}
		errorapp(error,&user,E_NO_UPDATE);
	}
	if((ret_code = update_dm(2,user,dm_sent,error))){
		if (ret_code == CURLE_HTTP_RETURNED_ERROR){
			strcat(error,"\nPlease check your user name, password and retry");
		}
		errorapp(error,&user,E_NO_UPDATE);
	}
	if((ret_code = update(user,time_line,error))){
		if (ret_code == CURLE_HTTP_RETURNED_ERROR){
			strcat(error,"\nPlease check your user name, password and retry");
		}
		errorapp(error,&user,E_NO_UPDATE);
	}
	if((ret_code = display_time_line(user.username,main_pad)))return ret_code;

		

	do {	
		/* check this */
					
		choice = getch();
		xml_to_download = (char*)malloc(sizeof(char) * 150);

		switch(choice){
			case 'Q':
				break;
			case '1':
				display_time_line(user.username,main_pad);
				pad_use = 0;
				break;

			case '2':
				display_dm(1,user.username,d_pad);
				pad_use = 1;
				break;
			case '3':
				display_dm(2,user.username,d_pad_sent);
				pad_use = 2;
				break;
			case 'u':
				strcpy(xml_to_download,time_line);
				sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id());
				if(update(user,xml_to_download,error))
					errorapp(error,&user,E_NO_UPDATE);

				strcpy(xml_to_download,dm);
				sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id_dm(1));
				if(update_dm(1,user,xml_to_download,error))
					errorapp(error,&user,E_NO_UPDATE);
					
				strcpy(xml_to_download,dm_sent);
				sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id_dm(2));
				if(update_dm(2,user,xml_to_download,error))
					errorapp(error,&user,E_NO_UPDATE);

				if(pad_use == 0){
					if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
					break;
				}
				else if(pad_use == 1){
					if((ret_code = display_dm(1,user.username,d_pad))) return ret_code;
					break;
				}
				else if(pad_use == 2){

					if((ret_code = display_dm(2,user.username,d_pad_sent))) return ret_code;
					break;
				}
			case 's':
				if(pad_use == 0){
					stat = send_update(user,error);
					if(stat != 0 && stat != 2)
						errorapp(error,&user,E_NO_SEND_UPDATE);
					if(stat == 0){
						strcpy(xml_to_download,time_line);
						sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id());
						if(update(user,xml_to_download,error))
							errorapp(error,&user,E_NO_UPDATE);
						pad_use = 0;
											
						if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
					}
					else 
						if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
				}
				
				else if (pad_use == 1 || pad_use == 2){
					stat = send_dm(user,error);
					if(stat != 0 && stat != 2)
						errorapp(error,&user,E_NO_SEND_REPLY);
					if(stat == 0){
						if((display_dm(2,user.username,d_pad_sent))) return ret_code;
						strcpy(xml_to_download,dm_sent);
						sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id_dm(2));
						if(update_dm(2,user,xml_to_download,error))
							errorapp(error,&user,E_NO_UPDATE);
						pad_use = 2;
					}
				
				if((display_dm(2,user.username,d_pad_sent))) return ret_code;
				}
				break;
			case 'r':
				if(pad_use == 0){
					stat = send_reply(user,error);
					if(stat != 0 && stat != 2)
						errorapp(error,&user,E_NO_SEND_REPLY);
					strcpy(xml_to_download,time_line);
					sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id());
					if(update(user,xml_to_download,error))
						errorapp(error,&user,E_NO_UPDATE);
					if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
				
				}
				break;
			/*case 'F':
				if(pad_use == 0){
					stat = fav(user,error);
					if(stat)
						errorapp(error,&user,E_NO_FAV);*/

			case 't':
				if(pad_use == 0){
					stat = retweet(user,error);
					if(stat != 0 && stat != 2)
						errorapp(error,&user,E_NO_RETWEET);
					strcpy(xml_to_download,time_line);
					sprintf((xml_to_download + strlen(xml_to_download)),"?since_id=%.0f",get_max_id());
					if(update(user,xml_to_download,error))
						errorapp(error,&user,E_NO_UPDATE);
					if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
					break;
				}
			case 'D':
				if(pad_use == 0){
					stat = destroy(0,user,error);
					if(stat)
						errorapp(error,&user,E_NO_SEND_UPDATE);
					else{
						strcpy(xml_to_download,time_line);
						drop_table();
						create_table();
						if(update(user,xml_to_download,error))
							errorapp(error,&user,E_NO_UPDATE);
											
						if((ret_code = display_time_line(user.username,main_pad)))return ret_code;
					}
				}
				else if(pad_use == 1){
					stat = destroy(1,user,error);
					if(stat)
						errorapp(error,&user,E_NO_SEND_UPDATE);
					else{
						strcpy(xml_to_download,dm);
						drop_table_dm(1);
						create_table_dm(1);
						if(update_dm(1,user,xml_to_download,error))
							errorapp(error,&user,E_NO_UPDATE);
											
						if((ret_code = display_dm(1,user.username,d_pad)))return ret_code;
					}
				}
				else if(pad_use == 2){
					stat = destroy(2,user,error);
					if(stat)
						errorapp(error,&user,E_NO_SEND_UPDATE);
					else{
						strcpy(xml_to_download,dm_sent);
						drop_table_dm(2);
						create_table_dm(2);
						if(update_dm(2,user,xml_to_download,error))
							errorapp(error,&user,E_NO_UPDATE);
											
						if((ret_code = display_dm(2,user.username,d_pad_sent)))return ret_code;
					}
				}
				
				break;
							
			case KEY_UP:
				if(pad_use == 0){
					prefresh(main_pad,getcury(main_pad) -1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(main_pad,getcury(main_pad) -1,getmaxx(stdscr)/2);
				}
				else if(pad_use == 1){
					prefresh(d_pad,getcury(d_pad) -1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(d_pad,getcury(d_pad) -1,getmaxx(stdscr)/2);
				}
				else if(pad_use == 2){
					prefresh(d_pad_sent,getcury(d_pad_sent) -1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(d_pad_sent,getcury(d_pad_sent) -1,getmaxx(stdscr)/2);
				}
				break;
			case KEY_DOWN:
				if(pad_use == 0){
					prefresh(main_pad,getcury(main_pad) + 1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(main_pad,getcury(main_pad) +1,getmaxx(stdscr)/2);
				}
				else if(pad_use == 1){
					prefresh(d_pad,getcury(d_pad) + 1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(d_pad,getcury(d_pad) +1,getmaxx(stdscr)/2);
				}
				else if(pad_use == 2){
					prefresh(d_pad_sent,getcury(d_pad_sent) + 1,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
					wmove(d_pad_sent,getcury(d_pad_sent) +1,getmaxx(stdscr)/2);
				}
				break;
			case KEY_LEFT:
				move(getcury(stdscr),getcurx(stdscr) - 1);
				refresh();
				break;
			case KEY_RIGHT:
				move(getcury(stdscr),getcurx(stdscr) + 1);
				refresh();
				break;

		}
		free(xml_to_download);
	}while(choice != 'Q');

	delwin(main_pad);
	endwin();
	curl_global_cleanup();				/* curl closed,clean up */
	unlink(".twit_ui_db");
	exit(EXIT_SUCCESS);
}
