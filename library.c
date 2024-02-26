#include "library.h"
#include "string_tools.h"
#define NUM_CHAR 256



Artist* artist_directory[NUM_CHAR];
int num_index = 0;

void init() {
	for (int i = 0; i < NUM_CHAR; i++)
		artist_directory[i] = NULL;
}

void load(FILE* fp)
{
	char buf[BUF_SIZE];
	char* _name, *_title, *_path;

	while (1) {
		if (read_line(fp, buf, BUF_SIZE) <= 0)
			break;

		_name = strtok(buf, "#");
		if (!strcmp(_name, " ")) {
			_name = NULL;
		}
		/*else {
			_name = _strdup(_name);
		}*/

		_title = strtok(NULL, "#");
		if (!strcmp(_title, " ")) {
			_title = NULL;
		}
		/*else {
			_title = _strdup(_title);
		}*/

		_path = strtok(NULL, "#");
		if (!strcmp(_path, " ")) {
			_path = NULL;
		}
		/*else {
			_path = _strdup(_path);
		}*/

		//printf("%s    %s    %s\n", _name, _title, _path);
		add_song(_name, _title, _path);

	}
}

Artist* create_artist_instance(char* name) {
	Artist* ptr_artist = (Artist*)malloc(sizeof(Artist));
	if (ptr_artist != NULL) {
		ptr_artist->name = _strdup(name);
		ptr_artist->head = NULL;
		ptr_artist->tail = NULL;
		ptr_artist->next = NULL;

		return ptr_artist;
	}
	printf("failed create artist instance.\n");
	return NULL;
}

Artist* add_artist(char* name) {
	Artist* ptr_artist = create_artist_instance(name);
	Artist* p = artist_directory[(unsigned char)name[0]]; //head     
	Artist* q = NULL;

	while (p!= NULL && strcmp(p->name, name) < 0) {
		q = p;
		p = p->next;
	}
	if (p == NULL && q == NULL) { //unique node
		artist_directory[(unsigned char)name[0]] = ptr_artist;
	}
	else if (q == NULL) { //first node
		ptr_artist->next = p;
		artist_directory[(unsigned char)name[0]] = ptr_artist;
	}
	else { //add ptr_artist before p
		ptr_artist->next = p;
		q->next = ptr_artist;
	}
	return ptr_artist;
}

Artist* find_artist(char* name) {
	Artist* p = artist_directory[(unsigned char)name[0]]; //first artist with initial name[0]
	while (p != NULL && strcmp(p->name, name) < 0) {
		p = p->next;
	}
	if (p != NULL && !strcmp(p->name, name))
		return p;
	else
		return NULL;
}

void add_song(char* artist, char* title, char* path) {
		//find if the artist already exist
	Artist* ptr_artist = find_artist(artist); //return NULL is not exist the artist
	if (ptr_artist == NULL) {
		ptr_artist = add_artist(artist); //create artist and input arr
	}
	Song* ptr_song = create_song_instance(ptr_artist, title, path);
	Snode* ptr_snode = create_snode_instance(ptr_song);
	insert_node(ptr_snode, ptr_artist);
	
}

void insert_node(Snode* ptr_snode, Artist* ptr_artist){
	Snode* p = ptr_artist->head;
	while (p != NULL && strcmp(p->song->title , ptr_snode->song->title ) < 0 ) {
		p = p->next;
	}
	
	if (ptr_artist->head == NULL) { //empty node
		ptr_artist->head = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else if (p == ptr_artist->head) { //first_node 
		ptr_snode->next = ptr_artist->head;
		ptr_artist->head->prev = ptr_snode;
		ptr_artist->head = ptr_snode;
	}
	else if (p == NULL) { //last_node
		ptr_snode->prev = ptr_artist->tail;
		ptr_artist->tail->next = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else { //inset before p
		ptr_snode->next = p;
		ptr_snode->prev = p->prev;
		p->prev->next = ptr_snode;
		p->prev = ptr_snode;
	}

}

Snode* create_snode_instance(Song* song) 
{
	Snode* ptr_snode = (Snode*)malloc(sizeof(Snode));
	if (ptr_snode != NULL) {
		ptr_snode->song = song;
		ptr_snode->next = NULL;
		ptr_snode->prev = NULL;
		return ptr_snode;
	}
	printf("failed create sonde instance.\n");
	return NULL;
	
}

Song* create_song_instance(Artist* artist, char* title, char* path) {
	Song* ptr_song = (Song*)malloc(sizeof(Song));
	if (ptr_song != NULL) {
		ptr_song->artist = artist;
		ptr_song->title = _strdup(title);
		ptr_song->path = _strdup(path);
		ptr_song->index = num_index;
		num_index++;

		return ptr_song;
	}
	printf("failed create song instance.\n");
	return NULL;
}

void search_song(char * name, char * title) 
{
	Artist* artist = find_artist(name);
	if (artist == NULL) {
		printf("No such artist exists. \n");
		return;
	}
	Snode* p = artist->head;
	while (p != NULL && strcmp(p->song->title, title) < 0) {
		p = p->next;
	}

	if (p != NULL && !strcmp(p->song->title, title)) {
		printf("Found:\n");
		print_snode(p);
	}
	else
		printf("No such title exists. \n");
}

void search_song_by_only_name(char * name)
{
	Artist* artist = find_artist(name);
	if (artist == NULL) {
		printf("No such artist exists. \n");
		return;
	}
	Snode* p = artist->head;
	printf("Found:\n");
	while (p != NULL) {
		print_snode(p);
		p = p->next;
	}
}

void clear_memory()
{
	for (int i = 0; i < NUM_CHAR; i++) {
		if (artist_directory[i] != NULL) {
			Artist* p = artist_directory[i];
			while (p != NULL) {
				Artist* tmp = p->next;
				clear_snode(p->head);
				free(p->name);	p->name = NULL;
				free(p);	p = NULL;
				p = tmp;
			}
		}
	}
}

void clear_snode(Snode * head) 
{
	while (head != NULL) {
		Snode* tmp = head->next;
		clear_song(head->song);
		free(head);
		head = NULL;
		head = tmp;
	}
}

void clear_song(Song* song) 
{
	free(song->path);
	song->path = NULL;
	free(song->title);
	song->title = NULL;

	free(song);
}

void status()
{
	for (int i = 0; i < NUM_CHAR; i++) {
		Artist* ptr_artist = artist_directory[i];
		while (ptr_artist != NULL) {
			print_artist(ptr_artist);
			ptr_artist = ptr_artist->next;
		}
	}
}

void print_artist(Artist* artist)
{
	printf("%s\n", artist->name);
	Snode* snode = artist->head;
	while (snode != NULL) {
		print_snode(snode);
		snode = snode->next;
	}
}

void print_snode(Snode* snode) {
	printf("     %d : %s, %s\n", snode->song->index, snode->song->title, snode->song->path);
}