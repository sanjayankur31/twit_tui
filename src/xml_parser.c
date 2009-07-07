/* implement linked list again.. uve fucked up somewhere in it. */

#include "xml_parser.h"

#include <curses.h>
#include <unistd.h>


int xml_parse (struct MemoryStruct chunk, char *error){
	xmlNode *cur_node, *child_node, *g_child_node;
	xmlChar *created_at,*id,*text,*source,*truncated,*in_reply_to_status_id,*in_reply_to_user_id,*favorited,*in_reply_to_screen_name;
	xmlChar *user_id,*user_name,*user_screen_name,*user_location,*user_description,*user_profile_image_url,*user_url,*user_protected,*user_followers_count;
	xmlDocPtr doc;
	tweet_list temp_node;
	int ret_code;	
	xmlNode *root = NULL;
	
	/*doc = xmlReadFile(file_name,"UTF-8",XML_PARSE_PEDANTIC);*/
	doc = xmlRecoverMemory(chunk.memory,chunk.size);

	if (doc == NULL){
		printw("Could not parse");
		return 0;
	}

	root = xmlDocGetRootElement(doc);

	if( !root || !root->name || xmlStrcmp(root->name,(const xmlChar *)"statuses")){
		xmlFreeDoc(doc);
		return 0;
	}

	

	for(cur_node = root->children; cur_node != NULL; cur_node = cur_node->next){
		/* entered one "status" node */
		
		if (cur_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(cur_node->name, (const xmlChar *) "status")){
			/* entering "status"'s children */
			for(child_node = cur_node->children; child_node != NULL; child_node = child_node->next){
				/* "created_at" */
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"created_at")){
					/*printf("   Child=%s\n", child_node->name);*/
					created_at = xmlNodeGetContent(child_node);
					if(created_at) asprintf(&(temp_node.t_created_at),"%s",created_at);
					/* error checking 
					printw("%s",temp_node.t_created_at);
					refresh();
					sleep(2);
					*/
					xmlFree(created_at);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"id")){
					/*printf("   Child=%s\n", child_node->name);*/
					id = xmlNodeGetContent(child_node);
					if(id) asprintf(&(temp_node.t_id),"%s",id);
					xmlFree(id);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"text")){
					/*printf("   Child=%s\n", child_node->name);*/
					text = xmlNodeGetContent(child_node);
					if(text) asprintf(&(temp_node.t_text),"%s",text);
					xmlFree(text);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"source")){
					/*printf("   Child=%s\n", child_node->name);*/
					source = xmlNodeGetContent(child_node);
					if(source) asprintf(&(temp_node.t_source),"%s",source);
					xmlFree(source);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"truncated")){
					/*printf("   Child=%s\n", child_node->name);*/
					truncated = xmlNodeGetContent(child_node);
					if(truncated) asprintf(&(temp_node.t_truncated),"%s",truncated);
					xmlFree(truncated);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"in_reply_to_status_id")){
					/*printf("   Child=%s\n", child_node->name);*/
					in_reply_to_status_id = xmlNodeGetContent(child_node);
					if(in_reply_to_status_id) asprintf(&(temp_node.t_reply_to_status_id)," %s",in_reply_to_status_id);
					if(!(strcmp(temp_node.t_reply_to_status_id," "))) {
						asprintf(&(temp_node.t_reply_to_status_id),"1");
					}
					xmlFree(in_reply_to_status_id);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"in_reply_to_user_id")){
					/*printf("   Child=%s\n", child_node->name);*/
					in_reply_to_user_id = xmlNodeGetContent(child_node);
					if(in_reply_to_user_id) asprintf(&(temp_node.t_reply_to_user_id)," %s",in_reply_to_user_id);
					if(!(strcmp(temp_node.t_reply_to_user_id," "))){
						asprintf(&(temp_node.t_reply_to_user_id),"1");
					}
					/*printw("\n%scrap",in_reply_to_user_id);
					refresh();*/
					xmlFree(in_reply_to_user_id);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"favorited")){
					/*printf("   Child=%s\n", child_node->name);*/
					favorited = xmlNodeGetContent(child_node);
					if(favorited) asprintf(&(temp_node.t_favorited),"%s",favorited);
					xmlFree(favorited);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"in_reply_to_screen_name")){
					/*printf("   Child=%s\n", child_node->name);*/
					in_reply_to_screen_name = xmlNodeGetContent(child_node);
					/*if(in_reply_to_screen_name) printf("in reply to screen name: %s\n", in_reply_to_screen_name);*/
					xmlFree(in_reply_to_screen_name);
				}
				if (child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(child_node->name, (const xmlChar *)"user")){
					/*printf("User Info :\n");*/
					for(g_child_node = child_node->children; g_child_node != NULL; g_child_node = g_child_node->next){

						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"id")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_id = xmlNodeGetContent(g_child_node);
							if(user_id) asprintf(&(temp_node.u_id),"%s",user_id);
							xmlFree(user_id);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"name")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_name = xmlNodeGetContent(g_child_node);
							/*if(user_name) printf("\tname: %s\n", user_name);*/
							xmlFree(user_name);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"screen_name")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_screen_name = xmlNodeGetContent(g_child_node);
							if(user_screen_name) asprintf(&(temp_node.u_screen_name),"%s",user_screen_name);
							xmlFree(user_screen_name);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"location")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_location = xmlNodeGetContent(g_child_node);
							/*if(user_location) printf("\tlocation: %s\n", user_location);*/
							xmlFree(user_location);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"description")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_description = xmlNodeGetContent(g_child_node);
							/*if(user_description) printf("\tdescription: %s\n", user_description);*/
							xmlFree(user_description);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"profile_image_url")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_profile_image_url = xmlNodeGetContent(g_child_node);
							/*if(user_profile_image_url) printf("\tprofile url: %s\n", user_profile_image_url);*/
							xmlFree(user_profile_image_url);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"url")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_url = xmlNodeGetContent(g_child_node);
							/*if(user_url) printf("\turl: %s\n", user_url);*/
							xmlFree(user_url);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"protected")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_protected = xmlNodeGetContent(g_child_node);
							/*if(user_protected) printf("\tprotected: %s\n", user_protected);*/
							xmlFree(user_protected);
						}
						if (g_child_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(g_child_node->name, (const xmlChar *)"followers_count")){
							/*printf("   Child=%s\n", g_child_node->name);*/
							user_followers_count = xmlNodeGetContent(g_child_node);
							/*if(user_followers_count) printf("\tfollowers count: %s\n", user_followers_count);*/
							xmlFree(user_followers_count);
						}
					}
				}
			}
			if((ret_code = db_put_data(temp_node))) {
				strcpy(error,"Error putting data to db");
				return ret_code;
			}
		}
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
	
	return 0;
}
