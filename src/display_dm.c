#include "display_dm.h"

int display_dm(int stat,char *user_name,WINDOW *main_pad){
	char *db_name = ".twit_ui_db";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	char *appname = "Twit-Tui";
	int t;
	time_t since_epoch;
	char time_str[30];
	WINDOW *top_pad;
	int sql_errors;
	
	
	since_epoch = time(NULL);
	ctime_r(&since_epoch,time_str);

	top_pad = newpad(3,getmaxx(stdscr));



	if((sql_errors = sqlite3_open(db_name,&db_handle)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_errors;
	}

	/* retrieve data from db */
	if(stat == 1){
		if((sql_errors = sqlite3_prepare_v2(db_handle,"select * from dms order by d_id desc;",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_errors;
		}
	}

	else if(stat == 2){
		if((sql_errors = sqlite3_prepare_v2(db_handle,"select * from dms_sent order by d_id desc;",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_errors;
		}
	}

	wmove(main_pad,0,0);
	/*clear();
	refresh();*/
	wclear(main_pad);
	prefresh(main_pad,0,0,0,0,getmaxy(stdscr) -1 ,getmaxx(stdscr) -1);
	wmove(top_pad,0,0);
	wattron(top_pad,A_BOLD);
	whline(top_pad,ACS_HLINE, 1000);
	if(stat == 1)
	mvwprintw(top_pad,1,(getmaxx(stdscr)- (strlen(appname) + 40 + strlen(user_name) ))/2,"Direct Messages Received ***  %s <<%s>>  *** \t Last updated at :%s",appname,user_name,time_str);
	else if(stat == 2)
	mvwprintw(top_pad,1,(getmaxx(stdscr)- (strlen(appname) + 40 + strlen(user_name) ))/2,"Direct Messages Sent ***  %s <<%s>>  *** \t Last updated at :%s",appname,user_name,time_str);
	wattroff(top_pad,A_BOLD);
	whline(top_pad,ACS_HLINE, 1000);
	prefresh(top_pad,0,0,0,0,3,getmaxx(stdscr) -1);

	/* work on scrolling */

	for(t = sqlite3_step(temp) ; t == SQLITE_ROW; t = sqlite3_step(temp)){

		/*attron(COLOR_PAIR(2));
		printw("< %.0f ",sqlite3_column_double(temp,1));
		attroff(COLOR_PAIR(2));*/
		whline(main_pad,ACS_HLINE, 1000);
		wmove(main_pad,getcury(main_pad) + 1,getcurx(main_pad));
		wattron(main_pad,COLOR_PAIR(3));
		wattron(main_pad,A_BOLD);
		if(stat == 1)
		wprintw(main_pad,"%d <%s>",sqlite3_column_int(temp,0),sqlite3_column_text(temp,5));
		else if (stat == 2)
		wprintw(main_pad,"%d <%s>",sqlite3_column_int(temp,0),sqlite3_column_text(temp,7));
		wattroff(main_pad,COLOR_PAIR(3));
		wprintw(main_pad,"  %s ",sqlite3_column_text(temp,3));
		wattroff(main_pad,A_BOLD);
		wprintw(main_pad,"at %s.\n",sqlite3_column_text(temp,1));

		prefresh(main_pad,0,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);
	}

	wmove(main_pad,1,(getmaxx(stdscr))/2);
	prefresh(main_pad,0,0,3,0,getmaxy(stdscr) -1,getmaxx(stdscr) -1);

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	return 0;
}

