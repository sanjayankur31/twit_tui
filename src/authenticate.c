#include <curses.h>
#include <string.h>
#include "authenticate.h"

/* authenticate : takes the username and password of the useer and stores it in the infostruct */
void authenticate(infostruct *user){
	WINDOW *auth;
	char *u = "Username : ";
	char *p = "Password : ";

	auth = newwin(6,45,user->row/2, (user->col - 45)/2);
	box(auth,0,0);
	mvwprintw(auth,2,5,"%s",u);
	mvwprintw(auth,3,5,"%s",p);

	mvwscanw(auth,2,(5 + strlen(u)),"%s",user->username);
	noecho();
	mvwscanw(auth,3,(5 + strlen(p)),"%s",user->password);
	wmove(auth,3,5 +strlen(p));

	wrefresh(auth);
	delwin(auth);
	return;
}


