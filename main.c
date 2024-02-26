#define _CRT_SECURE_NO_WARNINGS
#include "library.h"
#include "string_tools.h"
#include <vld.h>

//_strdup() free() !!
void process_command();
void handle_add();
void handle_load();
void handle_search();

int main() 
{	
	init();
	handle_load();
	process_command();
	return 0;
}

void handle_load()
{
	char data_file_name[BUF_SIZE];
	printf("Data file name ? ");
	if (read_line(stdin, data_file_name, BUF_SIZE) <= 0)
		return;
	FILE* fp = fopen(data_file_name, "r");
	if (fp == NULL) {
		printf("No such file exist.\n");
		return;
	}
	load(fp);
	fclose(fp);
}



void process_command() 
{
	char command_buf[BUF_SIZE];
	char* command;
	while (1) {
		printf("$ ");
		if (read_line(stdin, command_buf, BUF_SIZE) <= 0)
			continue;
		
		command = strtok(command_buf, " ");

		if (!strcmp(command, "add")) {
			handle_add();
		}
		else if (!strcmp(command, "search")) {
			handle_search();
		}
		/*else if (!strcmp(command, "remove")) {
			handle_search();
		}
		else if (!strcmp(command, "play")) {
			handle_search();
		}
		else if (!strcmp(command, "save as")) {
			handle_search();
		}*/
		else if (!strcmp(command, "status")) {
			status();
		}
		else if (!strcmp(command, "exit")) {
			clear_memory();
			break;
		}

	}
}

void handle_search() 
{
	char artist[BUF_SIZE];
	char title[BUF_SIZE];
	printf("    Artist: ");
	if (read_line(stdin, artist, BUF_SIZE) <= 0) {
		printf("Artist name required.\n");
		return;
	}
	printf("    Title: ");
	int title_len = read_line(stdin, title, BUF_SIZE);

	if (title_len <= 0)
		search_song_by_only_name(artist);
	else
		search_song(artist, title);
}

void handle_add() {

	char buf[BUF_SIZE];
	char* artist = NULL, * title = NULL, * path = NULL;
	printf("    Artist: ");
	if (read_line(stdin, buf, BUF_SIZE) > 0) {
		artist = _strdup(buf);
	}
	printf("    Title: ");
	if (read_line(stdin, buf, BUF_SIZE) > 0) {
		title = _strdup(buf);
	}
	printf("    Path: ");
	if (read_line(stdin, buf, BUF_SIZE) > 0) {
		path = _strdup(buf);
	}
	//printf("[debug]=%s %s %s\n", artist, title, path); 

	add_song(artist, title, path);
}

