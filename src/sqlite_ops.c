#include "sqlite_ops.h"
#include <curses.h>
/* the display function is in display_time_line.c */

/* makes a temporary database in the working directory */
int create_table(){
	char *db_name = ".twit_ui_db";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int sql_error;
	


	if((sql_error = sqlite3_open(db_name,&db_handle)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}

	if((sql_error = sqlite3_prepare_v2(db_handle,"create table tweets(sno INTEGER PRIMARY KEY, t_created_at VARCHAR(70),t_id DOUBLE,t_text VARCHAR(200),t_source VARCHAR(15),t_truncated VARCHAR(15),t_reply_to_user_id DOUBLE,t_reply_to_status_id DOUBLE,t_favourited VARCHAR(15),u_id DOUBLE,u_screen_name VARCHAR(20));",-1,&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}


	if((sql_error = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		return sql_error;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	return 0;
}
int drop_table(){
	char *db_name = ".twit_ui_db";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int sql_error;
	


	if((sql_error = sqlite3_open(db_name,&db_handle)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}

	if((sql_error = sqlite3_prepare_v2(db_handle,"drop table tweets;",-1,&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}


	if((sql_error = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		return sql_error;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	return 0;
}

int drop_table_dm(int stat){
	char *db_name = ".twit_ui_db";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int sql_error;
	


	if((sql_error = sqlite3_open(db_name,&db_handle)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}

	if(stat == 1){

		if((sql_error = sqlite3_prepare_v2(db_handle,"drop table dms;",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_error;
		}
	}

	else if(stat == 2){
		
		if((sql_error = sqlite3_prepare_v2(db_handle,"drop table dms_sent;",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_error;
		}
	}

	if((sql_error = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		return sql_error;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	return 0;
}
int create_table_dm(int stat){
	char *db_name = ".twit_ui_db";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int sql_error;
	


	if((sql_error = sqlite3_open(db_name,&db_handle)) != SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_error;
	}

	if(stat == 1){

		if((sql_error = sqlite3_prepare_v2(db_handle,"create table dms(sno INTEGER PRIMARY KEY, d_created_at VARCHAR(70),d_id DOUBLE,d_text VARCHAR(200),ds_id DOUBLE,ds_screen_name VARCHAR(20),dr_id DOUBLE,dr_screen_name VARCHAR(20));",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_error;
		}
	}
	else if (stat == 2){

		if((sql_error = sqlite3_prepare_v2(db_handle,"create table dms_sent(sno INTEGER PRIMARY KEY, d_created_at VARCHAR(70),d_id DOUBLE,d_text VARCHAR(200),ds_id DOUBLE,ds_screen_name VARCHAR(20),dr_id DOUBLE,dr_screen_name VARCHAR(20));",-1,&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			return sql_error;
		}
	}
	if((sql_error = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		return sql_error;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	return 0;
}
/* puts data to the db */
int db_put_data_dm(int stat,dm_list temp_node){
	char *db_name = ".twit_ui_db";
	char *statement;
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int i = 0;
	int sql_errors;

	while(temp_node.d_created_at[++i] != '+');
	temp_node.d_created_at[i-1] = '\0';


	if((sql_errors = sqlite3_open(db_name,&db_handle))!= SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_errors;
	}
	if(stat == 1) /* implies received */
		statement = sqlite3_mprintf("insert into dms values(NULL,'%q',%Q,'%q',%Q,'%q',%Q,'%q');",temp_node.d_created_at,temp_node.d_id,temp_node.d_text,temp_node.ds_id,temp_node.ds_screen_name,temp_node.dr_id,temp_node.dr_screen_name);
	else 
		statement = sqlite3_mprintf("insert into dms_sent values(NULL,'%q',%Q,'%q',%Q,'%q',%Q,'%q');",temp_node.d_created_at,temp_node.d_id,temp_node.d_text,temp_node.ds_id,temp_node.ds_screen_name,temp_node.dr_id,temp_node.dr_screen_name);

	/* error checking */
	/*printw("%s",statement);
	refresh();*/

	if((sql_errors = sqlite3_prepare_v2(db_handle,statement,strlen(statement),&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		sqlite3_free(statement);
		return sql_errors;
	}
	



	if((sql_errors = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		sqlite3_free(statement);
		return sql_errors;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	sqlite3_free(statement);
	return 0;
}
int db_put_data(tweet_list temp_node){
	char *db_name = ".twit_ui_db";
	char *statement;
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	int i = 0;
	char *parse_source;
	int sql_errors;

	while(temp_node.t_created_at[++i] != '+');
	temp_node.t_created_at[i-1] = '\0';

	if(temp_node.t_source[0] == '<'){
		temp_node.t_source[strlen(temp_node.t_source) - 4] = '\0';
		for(i = 0; temp_node.t_source[i] != '>'; i++);

		parse_source = &temp_node.t_source[i+1];
	}
	else 
		parse_source = &temp_node.t_source[0];

	if((sql_errors = sqlite3_open(db_name,&db_handle))!= SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_errors;
	}

	statement = sqlite3_mprintf("insert into tweets values(NULL,'%q',%q,'%q','%q','%q',%Q,%Q,'%q',%q,'%q');",temp_node.t_created_at,temp_node.t_id,temp_node.t_text,parse_source,temp_node.t_truncated,temp_node.t_reply_to_user_id,temp_node.t_reply_to_status_id,temp_node.t_favorited,temp_node.u_id,temp_node.u_screen_name);

	/* error checking */
	/*printw("%s",statement);
	refresh();*/

	if((sql_errors = sqlite3_prepare_v2(db_handle,statement,strlen(statement),&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		sqlite3_free(statement);
		return sql_errors;
	}
	



	if((sql_errors = sqlite3_step(temp)) != SQLITE_DONE){
		sqlite3_close(db_handle);
		sqlite3_free(statement);
		return sql_errors;
	}

	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	sqlite3_free(statement);
	return 0;
}

/* gets the max id from db */
double get_max_id_dm(int stat){
	char *db_name = ".twit_ui_db";
	char *statement = "select max(d_id) from dms;";
	char *statement1 = "select max(d_id) from dms_sent;";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	double max_id = 0; 
	int sql_errors;

	

	if((sql_errors = sqlite3_open(db_name,&db_handle))!= SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_errors;
	}

	if(stat == 1){
		if((sql_errors = sqlite3_prepare_v2(db_handle,statement,strlen(statement),&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			sqlite3_finalize(temp);
			return sql_errors;
		}
	}
	else if(stat == 2){
		if((sql_errors = sqlite3_prepare_v2(db_handle,statement1,strlen(statement1),&temp,NULL)) != SQLITE_OK){
			sqlite3_close(db_handle);
			sqlite3_finalize(temp);
			return sql_errors;
		}
	}
	
	if((sql_errors = sqlite3_step(temp)) != SQLITE_ROW){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors;
	}


	max_id =  sqlite3_column_double(temp,0);
	sqlite3_close(db_handle);
	sqlite3_finalize(temp);

	return max_id;
}
double get_max_id(){
	char *db_name = ".twit_ui_db";
	char *statement = "select max(t_id) from tweets;";
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	double max_id;
	int sql_errors;

	

	if((sql_errors = sqlite3_open(db_name,&db_handle))!= SQLITE_OK){
		sqlite3_close(db_handle);
		return sql_errors;
	}

	if((sql_errors = sqlite3_prepare_v2(db_handle,statement,strlen(statement),&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors;
	}
	



	if((sql_errors = sqlite3_step(temp)) != SQLITE_ROW){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors;
	}


	max_id =  sqlite3_column_double(temp,0);
	sqlite3_close(db_handle);
	sqlite3_finalize(temp);

	return max_id;
}
/* gets the message_id, screen_name and the message */
double get_message_id(int stat,char *sno,char **ptr_screen_name,char **ptr_message){
	char *db_name = ".twit_ui_db";
	char *statement_t = "select t_id,u_screen_name,t_text from tweets where sno=";
	char *statement_d = "select d_id,ds_screen_name,d_text from dms where sno=";
	char *statement_d_sent = "select d_id,dr_screen_name,d_text from dms_sent where sno=";
	char *final_statement;
	sqlite3 *db_handle;
	sqlite3_stmt *temp;
	double max_id;
	int sql_errors;
	char *statement;

	if(stat == 0)
		statement = statement_t;
	else if (stat == 1)
		statement = statement_d;
	else if (stat == 2)
		statement = statement_d_sent;


	final_statement = malloc(strlen(statement) + 20);

	sprintf(final_statement,"%s%s;",statement,sno);
	/* for debugging purposes 
	clear();
	printw("%s",final_statement);
	refresh();
	sleep(2);
	*/

	

	if((sql_errors = sqlite3_open(db_name,&db_handle))!= SQLITE_OK){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors;
	}

	if((sql_errors = sqlite3_prepare_v2(db_handle,final_statement,strlen(final_statement),&temp,NULL)) != SQLITE_OK){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors;
	}
	



	if((sql_errors = sqlite3_step(temp)) != SQLITE_ROW){
		sqlite3_close(db_handle);
		sqlite3_finalize(temp);
		return sql_errors ;
	}


	max_id =  sqlite3_column_double(temp,0);

	sprintf(*ptr_screen_name,"%s",(char *)sqlite3_column_text(temp,1));
	sprintf(*ptr_message,"%s",(char *)sqlite3_column_text(temp,2));
	/* debug debug debug
	printw("%.0f",max_id);
	refresh();
	sleep(2);
	*/
	sqlite3_close(db_handle);
	sqlite3_finalize(temp);
	free(final_statement);

	return max_id;
}
