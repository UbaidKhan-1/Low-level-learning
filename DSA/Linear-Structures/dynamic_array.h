#ifndef VECTOR_H
#define VECTOR_H
#include "../Utils/macros.h"

typedef enum{
	NONE,
	INT,
	CHARECTER,
	STRING,
	DYNAMIC_ARRAY,
	LAST_TYPE // Tracks end of the enum
} types;

typedef struct{
	types type;
	void* value;
}Item;

typedef struct vectN{
    Item* data;
    int size;
    int capacity;
} VectN;

#endif
