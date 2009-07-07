/* Error handlong function 
 * starts a new win and prints the error message. 
 * waits for user response, cleans up and exits the program 
 * */
#include "errorapp.h"

void errorapp(char *error_message,infostruct *user, int error_code){
	char *header = "*** Error ***";
	WINDOW *error_win;
	WINDOW *msgwind;

	clear();
	refresh();
	
	error_win = newwin(8,80,user->row/2, (user->col - 80)/2);
	msgwind = derwin(error_win,6,76,1,2);
	box(error_win,0,0);
	wattron(error_win,A_BOLD);
	mvwprintw(error_win,1,(getmaxx(error_win) - (strlen(header)))/2,"%s",header);
	wattroff(error_win,A_BOLD);			
	wrefresh(error_win);

	mvwprintw(msgwind,2,0, "Error : %s",error_message);
	wprintw(msgwind,"\n\nPress a key to exit");
	wrefresh(msgwind);
	wgetch(error_win);
	delwin(error_win);


	endwin();
	curl_global_cleanup();				/* curl closed,clean up */
	unlink(".twit_ui_db");
	exit(error_code);
}


