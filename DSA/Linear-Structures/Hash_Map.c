#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Utils/hashMap.h"

/*

This hashmap implementation only allows strings as keys. But it allows these data types as values: (INT,STRING,BOOL)
 
 */
 
 // USAGE
//int main(int argc, char *argv[])
//{
//	Hashmap* Scoremap = new_HashMap(20);

//	char *students[10] = {
//		"Liam Smith",
//		"Noah Johnson",
//		"Oliver Williams",
//		"Elijah Brown",
//		"James Jones",
//		"William Garcia",
//		"Benjamin Miller",
//		"Lucas Davis",
//		"Henry Rodriguez",
//		"Alexander Martinez"};

//	int scores[10] = {
//		85, 92,
//		78, 95,
//		88, 73,
//		90, 64,
//		81, 89};
//		
//	int numScores = sizeof(scores) / sizeof(int);
//	char *results[numScores];
//	for (int i = 0; i < numScores; i++)
//	{
//		if (scores[i] >= 95)
//		{
//			results[i] = "A+";
//		}
//		else if (scores[i] >= 90)
//		{
//			results[i] = "A";
//		}
//		else if (scores[i] >= 85)
//		{
//			results[i] = "B+";
//		}
//		else if (scores[i] >= 80)
//		{
//			results[i] = "B";
//		}
//		else if (scores[i] >= 75)
//		{
//			results[i] = "C+";
//		}
//		else if (scores[i] >= 70)
//		{
//			results[i] = "C";
//		}
//		else
//		{
//			results[i] = "F";
//		}
//	}
//	
//	// Adding key value pairs to hashmap
//	printf("items added to hashmap:- \n");
//	for (int i = 0; i < 10; i++)
//	{
//		AddToMap(Scoremap, students[i], scores[i], INT);
//	}
//	printFullMap(Scoremap);


//	// Changing values to a different type
//	printf("items changed in hashmap:- \n");
//	for (int i = 0; i < 10; i++)
//	{
//		AddToMap(Scoremap, students[i], results[i], STRING);
//	}
//	printFullMap(Scoremap);
//	
//	
//	// Looking up a value
//	printf("looking up by value:- \n");
//	printItem(lookup(Scoremap,"Elijah Brown"));


//	//Clearing the map
//	printf("\nhasmap cleared:- \n");
//	clearMap(Scoremap);
//	printFullMap(Scoremap);

//		
//	// Freeing the memory allocated for items*
//	free(Scoremap->items);
//    free(Scoremap);
//	return 0;
//}

//    +---------Implementation---------+

unsigned int Hash(char *key)
{
	int result = 0;
	for (int i = 0; i < strlen(key); i++)
	{
		result += key[i];
		result *= (i + 1);
	}
	return result;
}

// frees the Item properly by first freeing
// pointers stored in the item
void freeItem(Item *item)
{
	free(item->key);
	if (item->type == STRING)
	{
		free(item->value);
	}
	free(item);
}

// Adds an item to the map. if key already
// exists then it updates the items value
bool AddToMap(Hashmap *hashmap, char *key, void *value, types type)
{
	if (type < 0 || type >= TYPE_COUNT)
	{
		return false;
	}

	// Creating persistent keys and values

	// one extra byte for \0
	char *keyCopy = malloc(strlen(key) + 1);
	if (key == NULL)
	{
		return false;
	}
	key = strcpy(keyCopy, key);

	if (type == STRING)
	{
		void *valueCopy = malloc(strlen(value) + 1);
		if (valueCopy == NULL)
		{
			return false;
		}
		value = strcpy(valueCopy, value);
	}
	//___________________________________

	Item *newItem = malloc(sizeof(Item));
	if (newItem == NULL)
	{
		return false;
	}
	newItem->key = key;
	newItem->value = value;
	newItem->type = type;
	newItem->next = NULL;

	int index = Hash(key) % hashmap->size;
	Item *curr = hashmap->items[index];

	if (curr == NULL)
	{
		hashmap->items[index] = newItem;
		return true;
	}
	Item *prev = curr;
	while (curr != NULL)
	{
		if (strcmp(curr->key, key) == 0)
		{
			if (curr != hashmap->items[index])
			{
				newItem->next = curr->next;
				free(curr);
				prev->next = newItem;
			}
			else
			{
				hashmap->items[index] = newItem;
			}
			return true;
		}
		prev = curr;
		curr = curr->next;
	}
	prev->next = newItem;
	return true;
}

// Looks up an item by its key in the hasmap
Item *lookup(Hashmap *hashmap, char *key)
{
	int index = Hash(key) % hashmap->size;
	Item *curr = hashmap->items[index];
	while (curr != NULL)
	{
		if (strcmp(curr->key, key) == 0)
		{
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

// prints a single items key and value
void printItem(Item *item)
{
	if (item == NULL){
		printf("{ NULL }");
		return;
	}
	printf(" %s: ", item->key);
	switch (item->type)
	{
	case INT:
		printf("%i\n", (int)item->value);
		break;
	case STRING:
		printf("%s\n", (char *)item->value);
		break;
	case BOOL:
		printf("%s\n", (bool)item->value == 1 ? "true" : "false");
		break;
	}
}

// creates hashmap and initializes all elements of the allocated as NULL
Hashmap* new_HashMap(int size)
{
	Hashmap* hashmap=malloc(sizeof(Hashmap));
	hashmap->size = size;
	
	Item **items = malloc(size * sizeof(int));
	// initializing all items as NULL
	for (int i = 0; i < size; i++)
	{
		items[i] = NULL;
	}
	hashmap->items = items;
	return hashmap;
}

// prints full map including all elements
// in each linked list bucket
void printFullMap(Hashmap *hashmap)
{
	bool empty = true;
	for (int i = 0; i < hashmap->size; i++)
	{
		Item *curr = hashmap->items[i];
		while (curr != NULL)
		{
			printItem(curr);
			curr = curr->next;
			empty = false;
		}
	}
	if (empty)
	{
		printf(" | No Items |");
	}
	printf("\n");
}

// frees all allocated Items in map and
// re iniitializes all elements to NULL
void clearMap(Hashmap *map)
{
	for (int i = 0; i < map->size; i++)
	{
		clearMap_r(map->items[i]);
		map->items[i] = NULL;
	}
}
void clearMap_r(Item *item)
{
	if (item == NULL)
	{
		return;
	}
	clearMap_r(item->next);
	freeItem(item);
	return;
}