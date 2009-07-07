#include "greet.h"

/* greet : initial greeting */
void greet(infostruct user){
	WINDOW *greetwindow;
	char *appname = "Twit-tui";
	char *summary = "A command line Twitter client written in C.";

	greetwindow = newwin(user.row,user.col,0,0);

	curs_set(0);
	/*wcolor_set(greetwindow,1,NULL);*/
	wattron(greetwindow,A_BOLD);

	mvwprintw(greetwindow,user.row/2 -10,(user.col - strlen(appname))/2,"%s",appname);
	mvwprintw(greetwindow,user.row/2 -9,(user.col -strlen(summary))/2,"%s",summary);

	wrefresh(greetwindow);
	sleep(2);
	wattroff(greetwindow,A_BOLD);

	curs_set(1);
	delwin(greetwindow);
	
}


