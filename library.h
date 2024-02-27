#define _CRT_SECURE_NO_WARNINGS
#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct artist Artist;
typedef struct snode Snode;
typedef struct song Song;

struct artist{
	char* name;
	struct artist* next;
	struct snode* tail, * head;
};

struct snode {
	struct song* song;
	struct snode* prev, * next;
};

struct song {
	char* title;
	int index;
	char* path;
	struct artist* artist;
};


void add_song(char* artist, char* title, char* path);
void init();
void status();
void clear_memory();
void load(FILE* fp);
void search_song(char * name, char * title);
void search_song_by_only_name(char* name);
void play(int index);
void delete_by_index(int index);
void save(char* file_name);


#endif



