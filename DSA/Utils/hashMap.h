#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdbool.h>

typedef enum types{
	INT,
	STRING,
	BOOL,
	TYPE_COUNT
}types;

typedef struct mapItem{
	void* value;
	struct mapItem* next;
	types type;
	char* key;
} Item;

typedef struct Hashmap{
	int size;
	Item** items;
} Hashmap;


unsigned int Hash(char *key);

void freeItem(Item *item);

bool AddToMap(Hashmap* hashmap, char* key, void* value, types type);

Item* lookup(Hashmap* hashmap, char* key);
void clearMap(Hashmap* map);

void clearMap_r(Item* item);

Hashmap* new_HashMap(int size);

void printItem(Item* item);

void printFullMap(Hashmap* hashmap);
#endif