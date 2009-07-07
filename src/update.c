#include "update.h"
/* the downloding and storing in a chunk is taken from one of the examples.. fit in so perfectly that i dont need to change a thing */

static void *myrealloc(void *ptr, size_t size)
{
  /* There might be a realloc() out there that doesn't like reallocing
     NULL pointers, so we take care of it here */
  if(ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

static size_t
WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->memory = myrealloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory) {
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
  }
  return realsize;
}

int update(infostruct user,char *xml_to_download,char *error){
	/* just does the stuff :D */
	char *user_name;
	CURL *curl_handle;
	int ret_code;

	struct MemoryStruct chunk;

	chunk.memory=NULL; /* we expect realloc(NULL, size) to work */
	chunk.size = 0;    /* no data at this point */

	user_name = user.username;

	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, xml_to_download);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	/* other stuff needed */
	curl_easy_setopt(curl_handle,CURLOPT_ERRORBUFFER,error);
	curl_easy_setopt(curl_handle,CURLOPT_FAILONERROR,1);
	curl_easy_setopt(curl_handle,CURLOPT_HTTPAUTH,CURLAUTH_ANY);
	curl_easy_setopt(curl_handle,CURLOPT_USERNAME,user.username);
	curl_easy_setopt(curl_handle,CURLOPT_PASSWORD,user.password);
	if(strcmp(user.proxy,"0")){
		curl_easy_setopt(curl_handle,CURLOPT_PROXY,user.proxy);

		if(strcmp(user.proxy_port,"0"))
			curl_easy_setopt(curl_handle,CURLOPT_PROXYPORT,user.proxy_port);
		if(strcmp(user.proxy_username,"0"))
			curl_easy_setopt(curl_handle,CURLOPT_PROXYUSERNAME,user.proxy_username);
		if(strcmp(user.proxy_password,"0"))
			curl_easy_setopt(curl_handle,CURLOPT_PROXYPASSWORD,user.proxy_password);
	}



	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	/* some servers don't like requests that are made without a user-agent
	field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* get it! CURLE_OK is 0 anyway..*/
	if((ret_code = curl_easy_perform(curl_handle))){
		curl_easy_cleanup(curl_handle);
		return ret_code;
	}

	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
	
	if((ret_code = xml_parse(chunk,error)))return ret_code;


	if(chunk.memory)
		free(chunk.memory);

		

	return 0;
}

