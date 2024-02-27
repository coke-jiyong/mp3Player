#include "library.h"
#include "string_tools.h"
#define NUM_CHAR 256
#define SIZE_INDEX_TABLE 10

Artist* artist_directory[NUM_CHAR];
Snode* index_directory[SIZE_INDEX_TABLE];
int num_index = 0;

Snode* find_snode(Artist* artist, char* title);
Artist* find_artist(char* name);
void print_snode(Snode* snode);
void print_artist(Artist* artist);
void insert_node(Snode* snode, Artist* artist);
void insert_index_diretory(Song* ptr_song);
Snode* find_song_by_index(int index);
void clear_snode(Snode* snode);
void clear_song(Song* song); void index_directory_clear(Snode*); Artist* find_artist(char* name);
Artist* add_artist(char* name);
Artist* create_artist_instance(char* name);
Song* create_song_instance(Artist* ptr_artist, char* title, char* path);
Snode* create_snode_instance(Song* song);
void save_parser(FILE* fp, Artist* ptr_artist);
void delete_snode(Artist * ptr_artist ,Snode* ptr_snode);
void delete_artist(Artist* ptr_artist, char* name);

void init() {

	for (int i = 0; i < NUM_CHAR; i++) 
		artist_directory[i] = NULL;

	for(int i = 0 ; i < SIZE_INDEX_TABLE ; i ++)
		index_directory[i] = NULL;

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
		_title = strtok(NULL, "#");
		if (!strcmp(_title, " ")) {
			_title = NULL;
		}
		_path = strtok(NULL, "#");
		if (!strcmp(_path, " ")) {
			_path = NULL;
		}
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
	insert_index_diretory(ptr_song);
}

void insert_index_diretory(Song * ptr_song)
{
	Snode* index_snode = create_snode_instance(ptr_song);
	if (index_snode == NULL) {
		printf("Insert index directory failed.\n");
		return;
	}
	int index = ptr_song->index % SIZE_INDEX_TABLE;

	Snode* p = index_directory[index]; 
	Snode* q = NULL;
	while (p != NULL && strcmp(p->song->title, index_snode->song->title) < 0) {
		q = p;
		p = p->next;
	}
	if (q == NULL) {					//index_directory[index] = p 를 하지 않아서 생긴 오류
		index_snode->next = p;
		index_directory[index] = index_snode;
	}
	else {
		index_snode->next = p;
		q->next = index_snode;
	}
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
	Artist* ptr_artist = find_artist(name);
	if (ptr_artist == NULL) {
		printf("No such artist exists. \n");
		return;
	}
	Snode * ptr_snode = find_snode(ptr_artist, title);
	if (ptr_snode == NULL) {
		printf("No such title exists. \n");
		return;
	}
	printf("Found\n");
	print_snode(ptr_snode);
}

Snode* find_snode(Artist* artist, char* title) 
{
	Snode* ptr_snode = artist->head;
	while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0) {
		ptr_snode = ptr_snode->next;
	}

	if (ptr_snode != NULL && !strcmp(ptr_snode->song->title, title)) {
		return ptr_snode;
	}
	else {
		return NULL;
	}
}

void search_song_by_only_name(char * name)
{
	Artist* ptr_artist = find_artist(name);
	if (ptr_artist == NULL) {
		printf("No such artist exists. \n");
		return;
	}
	printf("Found:\n");
	print_artist(ptr_artist);
}

Snode* find_song_by_index(int index)
{
	int _index = index % SIZE_INDEX_TABLE;
	Snode* ptr_snode = index_directory[_index];
	while (ptr_snode != NULL && ptr_snode->song->index !=index ) {
		ptr_snode = ptr_snode->next;
	}
	
	return ptr_snode;
}

void play(int index)
{
	Snode* ptr_snode = find_song_by_index(index);
	if (ptr_snode == NULL) {
		printf("No such song exists.\n");
		return;
	}
	printf("Play %d: %s\n",ptr_snode->song->index, ptr_snode->song->title);
}

void delete_by_index(int index)
{
	int directory_index = index % SIZE_INDEX_TABLE;
	Snode* ptr_snode = index_directory[directory_index];
	Snode* q = NULL;
	Song* s = NULL;
	while (ptr_snode != NULL && ptr_snode->song->index != index) {
		q = ptr_snode;
		ptr_snode = ptr_snode->next;
	}
	if (ptr_snode == NULL) {
		printf("No such this index exists1.\n");
		return;
	}
	s = ptr_snode->song;
	if (q == NULL) {
		index_directory[directory_index] = ptr_snode;
	}
	else {
		q->next = ptr_snode->next;
	}
	free(ptr_snode); ptr_snode = NULL;


	//delete artist
	Artist* ptr_artist = s->artist; //s.index = index
	Snode * found_snode = find_snode(ptr_artist, s->title);//artist에서 title의 snode를 찾아줌

	if (found_snode== NULL) {
		printf("Not found snode - somthing wrong.\n");
		return;
	}
	//delete_snode(ptr_artist, found_snode);

}

void delete_snode(Artist * ptr_artist,Snode* ptr_snode)
{
	Snode* first = ptr_artist->head;
	Snode* last = ptr_artist->tail;
	//unique node
	if (ptr_snode == first && ptr_snode == last) {
		//delete_artist(artist_directory[(unsigned char)ptr_artist->name[0]], ptr_artist->name);
		clear_song(ptr_snode->song);
		free(ptr_snode);	ptr_snode = NULL;
		return;
	}
	//first node
	else if (ptr_snode == first) {
		ptr_artist->head = ptr_snode->next;
		clear_song(ptr_snode->song);
		free(ptr_snode);	ptr_snode = NULL;
		return;
	}
	//last node
	else if (ptr_snode == last) {
		ptr_artist->tail = ptr_snode->prev;
		clear_song(ptr_snode->song);
		free(ptr_snode);	ptr_snode = NULL;
	}
	//in the middle
	else {
		ptr_snode->prev->next = ptr_snode->next;
		ptr_snode->next->prev = ptr_snode->prev;
		clear_song(ptr_snode->song);
		free(ptr_snode);	ptr_snode = NULL;
	}
}



void save(char* file_name)
{
	FILE* fp = fopen(file_name, "w");
	if (fp == NULL) {
		printf("File open failed.\n");
		return;
	}
	for (int i = 0; i < NUM_CHAR; i++) {
		Artist* ptr_artist = artist_directory[i];
		while (ptr_artist != NULL) {
			save_parser(fp ,ptr_artist);
			ptr_artist = ptr_artist->next;
		}
	}

	fclose(fp);
}

void save_parser(FILE* fp, Artist* ptr_artist)
{
	Snode* p = ptr_artist->head;
	while (p != NULL)
	{
		fprintf(fp, "%s#%s#%s#\n", ptr_artist->name, p->song->title, p->song->path == NULL ? " " : p->song->path);
		p = p->next;
	}
}

void clear_memory()
{
	for (int i = 0; i < NUM_CHAR; i++) {
		Artist* p = artist_directory[i];
		while (p != NULL) {
			Artist* tmp = p->next;
			clear_snode(p->head);
			free(p->name);	p->name = NULL;
			free(p);	p = NULL;
			p = tmp;
		}
	}

	for (int i = 0; i < SIZE_INDEX_TABLE; i++) {
		if (index_directory[i] != NULL) {
			index_directory_clear(index_directory[i]);
		}
	}
}

void clear_snode(Snode * head) 
{
	while (head != NULL) {
		Snode* tmp = head->next;
		if(head->song != NULL) 
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
	song = NULL;
}

void index_directory_clear(Snode* p) {
	while (p != NULL) {
		Snode* cp = p->next;
		free(p);
		p = NULL;
		p = cp;
	}
}

void status()
{
	printf("----------[ Artist Directory ]----------\n");
	for (int i = 0; i < NUM_CHAR; i++) {
		Artist* ptr_artist = artist_directory[i];
		while (ptr_artist != NULL) {
			print_artist(ptr_artist);
			ptr_artist = ptr_artist->next;
		}
	}
	printf("----------------------------------------\n");

	printf("----------[ Index Directory ]----------\n");
	for (int i = 0; i < SIZE_INDEX_TABLE; i++) {
		Snode* p = index_directory[i];
		while (p != NULL) {
			print_snode(p);
			p = p->next;
		}
	}
	printf("----------------------------------------\n");
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


void delete_artist(Artist* ptr_artist, char* name)
{
	Artist* head = artist_directory[(unsigned char)name[0]];
	//unique artist
	if (ptr_artist == head && ptr_artist->next == NULL) {
		free(ptr_artist->name);	ptr_artist->name = NULL;
		free(ptr_artist);	ptr_artist = NULL;
		artist_directory[(unsigned char)name[0]] = ptr_artist;
		return;
	}
	//first artist
	if (ptr_artist == head) {
		artist_directory[(unsigned char)name[0]] = ptr_artist->next;
		free(ptr_artist->name);	ptr_artist->name = NULL;
		free(ptr_artist);	ptr_artist = NULL;
		return;
	}
	//last artist
	if (ptr_artist->next == NULL) {
		free(ptr_artist->name);	ptr_artist->name = NULL;
		free(ptr_artist);	ptr_artist = NULL;
		return;
	}

	Artist* p = ptr_artist;
	Artist* q = NULL;
	while (p != NULL && strcmp(p->name, name) < 0) {
		q = p;
		p = p->next;
	}
	q->next = p->next;
	free(p->name);	p->name = NULL;
	free(p);
}