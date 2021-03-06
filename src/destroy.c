#include "destroy.h"

int destroy(int stat,infostruct user, char *error){
	char *url_t = "http://twitter.com/statuses/destroy/";
	char *url_d = "http://twitter.com/direct_messages/destroy/";
	char *header_t = "*** Delete a tweet ***";
	char *header_d = "*** Delete a Direct Message ***";
	char *ask_t = "Sno of tweet to delete: ";
	char *ask_d = "Sno of dm to delete: ";
	CURL *easy_handle;
	CURLcode success;
	int retval = 0;
	char *url_final;
	char sno[4];
	double message_id = 0;
	char *screen_name;
	char *message;
	char *header;
	char *ask;
	char *url;
	FILE *temp_file;
	
	if(stat == 0){
		header = header_t;
		ask = ask_t;
		url = url_t;
	}
	else {
		header = header_d;
		ask = ask_d;
		url = url_d;
	}

	if(get_sno(stat,user,sno,header,ask)){
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

	if((message_id = get_message_id(stat,sno,&screen_name,&message)) == 1)
		return 1;

	if(!(temp_file = fopen("/dev/null","w"))){
		return 1;
	}
	/* debug 
	printw("%.0f",message_id);
	refresh();
	sleep(2);
	*/


	url_final = (char*)malloc(strlen(url) + 20);
	sprintf(url_final,"%s%.0f.xml",url,message_id);
	
	easy_handle = curl_easy_init();
	curl_easy_setopt(easy_handle,CURLOPT_NOPROGRESS,1);
	curl_easy_setopt(easy_handle,CURLOPT_VERBOSE, 0);
	curl_easy_setopt(easy_handle,CURLOPT_URL,url_final);
	curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
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
	if((success = curl_easy_perform(easy_handle)))
		retval = 1;
	
	curl_easy_cleanup(easy_handle);

	free(screen_name);
	/*free(status_send);*/
	fclose(temp_file);
	refresh();
	return retval;

}
