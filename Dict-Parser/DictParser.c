#include <stdio.h>
#include <stdlib.h>
#include "../DSA/Utils/hashMap.h"
#include <string.h>
#include <ctype.h>

void stripSpaces(char* str) {
    int write = 0;
    bool instring = false;
    for (int read = 0; str[read] != '\0'; read++) {
        if (str[read] == '\''){
        	instring = !instring;
        	
        }
        if (!isspace((unsigned char)str[read]) || instring){
            str[write] = str[read];
            write++;
        }
    }
    str[write] = '\0';
}


Hashmap* createHashmap(char dictString[]){
	// These are purely to make code more
	// understandable
	bool KEY = 1;
	bool VALUE = 0;
	//-----------------------
	
	// stripping all white space
	stripSpaces(dictString);
	
	// finding length after stripped
	int length = strlen(dictString);
	
	// atleast needs to have '{}'
	if (length <2){
		printf("Invalid dict structure\n");
		printf("Valid Syntax: {key : value}\n");
		return NULL;
	}
	
	// first charecter should be '{' and last
	// '}'
	if (dictString[0] != '{' || dictString[length-1] != '}'){
		printf("Invalid structure");
		printf("Dict syntax should start with '{' and end with '}'\n");
		return NULL;
	}
	
	
	bool currentType = KEY;
	
	// initializing a new hashmap
	Hashmap* hashmap = new_HashMap(30);
	
	char key[50];
	char value[50];
	
	int keyIndex = 0;
	int valueIndex = 0;
	
	bool first_string_quote_seen = false;
	types valueType = INT; // default type
	
	//Starting from 1 to skip the obvious
	//first charecter '{'
	for (int i=1; i<length; i++){
		char c = dictString[i];
		
		if (c == ':'){
			currentType = VALUE;
			continue;
		}
		
		//Indicates end of a key-value pair
		if (c == ',' || c == '}'){
			// if key or value is empty
			if (keyIndex == 0 || valueIndex == 0){
				return hashmap;
				
			}
			// if there is only one quote
			if (first_string_quote_seen && valueType != STRING){
				printf("Invalid value. perhaps you forgot a (') \n");
				return hashmap;
			}
			
			
			// Adding the pair to map
			if (valueType != STRING){
				if (strcmp(value, "false") == 0){
					AddToMap(hashmap, key, false, BOOL);
				}
				else if (strcmp(value, "true") == 0){
					AddToMap(hashmap, key, true, BOOL);
				}
				else{
					AddToMap(hashmap, key,atoi(value), INT);
				}
			} else{
				AddToMap(hashmap, key, value, STRING);
			}
			
	    	// resetting the key, value and
		    // type fields for next pair
			keyIndex = 0;
			valueIndex = 0;
			key[0] = '\0';
			value[0] = '\0';
			currentType = KEY;
			first_string_quote_seen = false;
			continue;
		}
		
		// detecting STRING type values
		if (c == '\''){
			if (currentType != VALUE){
				printf("Invalid syntax: Keys cant have quotes \n");
				
				return hashmap;
			}
			if (first_string_quote_seen){
				valueType = STRING;
			}
			else{
				first_string_quote_seen = true;
			}
			continue;
		}
		
		if (currentType == KEY){
			key[keyIndex] = c;
			key[keyIndex + 1] = '\0'; 
			keyIndex += 1;
		}
		else if (currentType == VALUE){
			value[valueIndex] = c;
			value[valueIndex + 1] = '\0';
			valueIndex += 1;
		}
		
	}
	return hashmap;
}

//example usage
int main(int argc, char *argv[])
{
	char dict[] = "{"
	
             "name: 'Carla',"
             "age: 19,"
             "occupation: 'teacher',"
             "salary: 75000,"
             "isMarried: false,"
             "height: '5ft 7',"
             "city: 'Lahore',"
             "languages: 'English',"
             
             "}";
             
	Hashmap* hashmap=createHashmap(dict);
	
	printf("");
	printFullMap(hashmap);
	free(hashmap->items);
	free(hashmap);
	return 0;
}