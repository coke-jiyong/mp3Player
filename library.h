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
Artist* find_artist(char* name);
Artist* add_artist(char* name);
Artist* create_artist_instance(char* name);
Song * create_song_instance(Artist* ptr_artist, char *  title,char * path);
Snode* create_snode_instance(Song* song);
void insert_node(Snode* snode, Artist* artist);
void status();
void clear_memory();
void clear_snode(Snode* snode);
void clear_song(Song* song);
void print_artist(Artist* artist);
void print_snode(Snode* snode);
void load(FILE* fp);
void search_song(char * name, char * title);
void search_song_by_only_name(char* name);
#endif



