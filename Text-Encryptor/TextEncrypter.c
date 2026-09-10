#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Caesar cipher
int ShiftBy(char *text, unsigned int k)
{
	int c;
	for (int i = 0; i < strlen(text); i++)
	{
		c = text[i];
		if (c == ' ')
		{
			continue;
		}
		else if (c >= 'A' && c <= 'Z')
		{
			text[i] = 65 + ((c - 65 + k) % (91 - 65));
		}
		else if (c >= 'a' && c <= 'z')
		{
			text[i] = 97 + ((c - 97 + k) % (123 - 97));
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

//Substitution cipher
int Susbtitute(char *Text, char *Key)
{
	if (strlen(Key) < 26)
	{
		return 1;
	}

	int c;
	for (int i = 0; i < strlen(Text); i++)
	{
		c = Text[i];
		//______________________________

		if (c >= 'A' && c <= 'Z')
		{
			Text[i] = Key[c - 'A'];
		}
		else if (c >= 'a' && c <= 'z')
		{
			// adding 32 to shift to lower case
			Text[i] = Key[c - 'a'] + 32;
		}
		else if (isspace(c))
		{
			continue;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}

void clearStdin()
{
	int c;
	while ((c = getchar()) != EOF && c != '\n')
		;
}

void capitalize(char* text){
	char c;
	for (int i=0; i<strlen(text); i++){
		c = text[i];
		if (c >= 'a' && c <= 'z'){
			text[i] = c - 32;
		}
	}
}

int main(void)
{
	char Text[100];
	int mode;

	while (1)
	{
		printf("\nEnter Text: ");
		fgets(Text, 40, stdin);
		Text[strcspn(Text, "\n")] = '\0';

		printf("\nEnter Encryption Mode:\n");
		printf("1. Ceaser_cipher (c)\n");
		printf("2. Substitution_Cipher (s)\n");

		while (printf("\n--enter: "), (mode = getchar()) != 'c' && mode != 's')
		{
			clearStdin();
			printf("Invalid input: Try again");
		}
		clearStdin();
		if (mode == 'c')
		{
			int key;
			printf("Enter key (int): ");
			scanf("%i", &key);
			// removing the left over \n
			clearStdin();

			if (ShiftBy(Text, key) == 0)
			{
				printf("Encrypted Text: %s\n", Text);
			}
			else
			{
				printf("Text can only contain letters (A-Z) and (a-z)\n");
			}
		}

		else if (mode == 's')
		{
			printf("Enter key (str): ");
			char key[32];
			fgets(key, sizeof(key), stdin);
			key[strcspn(key, "\n")] = '\0';
			
			// correcting length
			while (strlen(key) > 26){
				key[strlen(key)-1] = '\0';
			}
			capitalize(Text);
			int status = Susbtitute(Text, key);
			if (status == 1)
			{
				printf("Key is too short. It should have atleast 26 charecters\n");
			}
			else if (status == 2)
			{
				printf("Invalid input string. It should only have letters (A-Z) and (a-z)\n");
			}
			else
			{
				printf("Encrypted Text: %s\n", Text);
			}
			
		}
	}

	return 0;
}