# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

MaxTokens = 100;
MaxTokenLength = 50;

void substring(char *dest, const char *src, int start, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = src[start + i];
    }

    dest[length] = '\0';
}

// seperates the query string into seperate words and stores them in the provided array
void tokenize(char str[], char tokens[][MaxTokenLength], int* tokenCount){
	int tokenStart = -1;// -1 acts as No Start
	int tokenIndex = 0;
	for (int i = 0; i<strlen(str); i++){
		if (tokenIndex >= MaxTokens){
			break;
		}
		char c = str[i];
		if (!isspace(c) && tokenStart == -1){
			tokenStart = i;
		} else if (isspace(c) && tokenStart != -1){
			char token[MaxTokenLength];
			substring(token, str, tokenStart, i-tokenStart);
			strcpy(tokens[tokenIndex], token);
			tokenStart = -1;
			tokenIndex ++;
		}
		// for last token
		if(!isspace(c) && i == strlen(str)-1){
			char token[MaxTokenLength];
			substring(token, str,tokenStart, i-tokenStart);
			strcpy(tokens[tokenIndex],token);
			tokenIndex++;
		}
	}
	*tokenCount = tokenIndex;
}

int main(){
	char query[256];
	for(;;){
		printf("Enter query: ");
		fgets(query, sizeof(query), stdin);
		char tokens[MaxTokens][MaxTokenLength];
		int tokenCount;
		tokenize(query, tokens, &tokenCount);
		printf("[");
		for (int i = 0; i < tokenCount; i++){
			if (i == tokenCount-1){
				printf("%s",tokens[i]);
				break;
			}
			printf("%s,",tokens[i]);
		}
		printf("]\n\n");
	}
	return 0;
}