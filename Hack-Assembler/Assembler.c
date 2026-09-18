#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../Dict-Parser/DictParser.h"
#define MAX_SYMBOL_LENGTH 20
#define MAX_VAR_LENGTH 20

typedef enum{
	A,
	C
}Instruction_type;


typedef struct {
	Instruction_type type;
	void* instruction;
}Token;


typedef struct {
	intptr_t value;
}Instruction_A;


typedef struct {
	char* dest;
	char* comp;
	char* jdir;
}Instruction_C;


typedef struct {
	Hashmap* symbols;
	Hashmap* comps;
	Hashmap* dests;
	Hashmap* jdirs;
}SyntaxMap;



//-----------------------------------
int preprocess(unsigned char* assembly_code, SyntaxMap* syntax);
void removeComments(unsigned char* str);
unsigned char* readFile(FILE* file);
void stripSpaces(unsigned char* str);
Token* parse(unsigned char* str, int n, SyntaxMap* syntax);
int resolveSymbols(Hashmap* symbols, unsigned char* str);
SyntaxMap createSyntaxMap();
void clearSyntaxMap(SyntaxMap* syntax);
bool split_C_IntoFields(char* dest, char* comp, char* jdir, char* instruction, int insNum);
void freetokens(Token* tokens, int n);
char* BinaryEncoder(Token* tokens, int n, SyntaxMap* syntax);
void cleanUp(Token* tokens, int n, SyntaxMap* syntax);
//-----------------------------------


int main(int argc, char *argv[])
{
	if (argc != 3){
		fprintf(stderr, "Assembler expects exactly two arguments\nCorrect usage: ./executable_name Source_file Out_file\n");
		return 1;
	}
	// +--NOTE--+
	// opening file in rb mode to prevent
	// discrepencies in file length between
	// windows and UNIX based systems due to
	// difference of newline charecter
	FILE* asmfile = fopen(argv[1], "rb");
	if (asmfile == NULL){
		perror("Error opening source file");
		return 1;
	}
	
	unsigned char* assembly_code = readFile(asmfile);
	if (assembly_code == NULL){
		fclose(asmfile);
		return 1;
	}
	fclose(asmfile);
	//----------------
	
	// Creating all required tables
	SyntaxMap syntax = createSyntaxMap();
	//_____________________________________
	
	
//           ( Assembler Pipeline )

//    Pre-processing
//	|--- removeComments
//	|--- stripSpaces
//	|--- resolveSymbols
//	    |
//	    √
//	Parsing
//	    |
//	    √
//	Binary-Encoding
//	    |
//	    √
//	Save-to-file
//_________________________________________
	
	int n = preprocess(assembly_code,&syntax);
	printf("Number of instructions = %i\n", n);
	if (n == -1){
		return 1;
	}
	
	Token* tokens = parse(assembly_code, n, &syntax);
	if (tokens == NULL){
		return 1;
	}
	free(assembly_code);
	
	char* binaryString = BinaryEncoder(tokens, n, &syntax);
	if (binaryString == NULL){
		return 1;
	}
	cleanUp(tokens, n, &syntax);
	
	FILE* outFile = fopen(argv[2], "wb");
	if (outFile == NULL){
		perror("Error opening output file");
		return 1;
	}
	fprintf(outFile, "%s", binaryString);
	fclose(outFile);
	free(binaryString);
	//____________________________________
	
	return 0;
}


int preprocess(unsigned char* assembly_code, SyntaxMap* syntax){
	removeComments(assembly_code);
	stripSpaces(assembly_code);
	int n = resolveSymbols(syntax->symbols, assembly_code);
	return n;
}


unsigned char* readFile(FILE* file){
    if (fseek(file, 0, SEEK_END) != 0){
    	perror("error seeking to the end of file");
    	return NULL;
    } 
    
    // finding length
    long length = ftell(file);
    if (length < 0){
		perror("error telling file cursor position");
		return NULL;
	} else if (length == 0){
		fprintf(stderr, "provided file is empty");
		return NULL;
	}
	
    if (fseek(file, 0, SEEK_SET)){
    	perror("error resetting file cursor position");
    	return NULL;
    }
    
    // Allocating 1 more byte than length
    // for '\0'
	unsigned char* buffer = malloc(length + 1);
	if (buffer == NULL){
		perror("error allocating memory");
		return NULL;
	}
	
	// reading everything from file at once
	// into buffer
	size_t bytes_read = fread(buffer, 1, length, file);
	if (bytes_read != (size_t)length){
		fprintf(stderr, "Fatal Error: Expected %ld bytes, but only read %zu.\n", length, bytes_read);
		
		if (ferror(file)){
			perror("system error");
			return NULL;
		} else if(feof(file)){
			fprintf(stderr, "File length expectedly changed during execution");
		}
		
		return NULL;
	}
	buffer[length] = '\0';
	return buffer;
}


void stripSpaces(unsigned char* str){
    // Strips
    int write = 0;
    unsigned char c = 0;
    bool lastwasnewline = false;
    
     // Stripping all white space while
     // preserving only single line breaks 
     // because they serve as instruction
     // seperators
    for (int read = 0; (c = str[read]) != '\0'; read++) {
        if (c == '\n'){
           //write>0 so there is no line
           //break before first instruction
        	if (!lastwasnewline && write > 0){
	        	str[write] = c;
	            lastwasnewline = true;
	            write ++;
	            continue;
        	}
        }
        //removing whitespace
        else if (!isspace(c)){
            str[write] = c;
            lastwasnewline = false;
            write++;
        }
    }
    //adding last newline if not present
    // for instruction detection in parsing
    if (str[write-1] != '\n'){
    	str[write] = '\n';
    	write++;
    }
    str[write] = '\0';
}


void removeComments(unsigned char* str){
	int write = 0;
	int read = 0;
	bool incomment = false;
	unsigned char c = 0;
	while ((c = str[read]) != '\0'){
		
		//in comment
		if (c == '/' && str[read+1] == '/'){
			incomment = true;
			read += 2;
			continue;
		}
		
		//out of comment
		if (c == '\n'){
			incomment = false;
		}
		
		if (!incomment){
			str[write] = c;
			write ++;
		}
		read ++;
	}
	str[write] = '\0';
}


int resolveSymbols(Hashmap* symbols, unsigned char* str){
	int read = 0;
	int write = 0;
	unsigned char c = 0;
	
	// for symbol tracking
	bool inSymbol = false;
	char symbol[MAX_SYMBOL_LENGTH] = {0};
	int symbolIndex = 0;
	
	// tracks address of current instruction
	int insNum = 0;
	
	// First pass (stores symbols in map and
	// removes them from the code string)
	while ((c = str[read]) != '\0'){
		if (c == '('){
			inSymbol = true;
			read ++;
			continue;
		}
		
		if (c == ')'){
			inSymbol = false;
			symbol[symbolIndex] = '\0';
			read ++;
			continue;
		}
		
		if (c == '\n'){
			// if symbol bracket not closed
			if (inSymbol){
				fprintf(stderr, "Invalid symbol definition: Symbol bracket not closed\n--Instruction # %i", insNum);
				return -1;
			}
			else if (symbolIndex > 0){
				if (lookup(symbols, symbol) == NULL){
					AddToMap(symbols, symbol, (void*)(intptr_t)insNum+1, INT);
				}
				symbolIndex = 0;
				read ++;
				continue; 
				// skipping write for
			    // newlines delimiting
			    // symbol declarations
			}
			else{
				insNum++;
			}
		}
		
		if (inSymbol){
			symbol[symbolIndex] = c;
			symbolIndex ++;
			read++;
			continue;
		}
		
		str[write] = c;
		write++;
		read ++;
	}
	str[write] = '\0';
	
	//----------------
	
	// for variable tracking
	read = 0;
	i8 variableStart = 16; 
	//user defined variables are assigned
	//consequitive addresses starting from 16
	bool invariable = false;
	char variable[MAX_VAR_LENGTH] = {0};
	int  varIndex = 0;
	int numVars = 0;
	
	// prevnewline tracks whether previous
	// char was a newline. Used to detect
	// first charecter of instruction
	bool prevnewline = true;
	
	// Second pass (stores variables in map)
	while ((c = str[read]) != '\0'){
		if (prevnewline && c == '@'){
			// if first charecter not numeric
			if (!(str[read+1] >= 48 && str[read+1] <= 57)){
				invariable = true;
				read ++;
				continue;
			}
		}
			
		// Marks the end of each instruction
		if (c == '\n'){
			prevnewline = true;
			if (invariable){
				//closing variable string
				variable[varIndex] = '\0';
				
				// if variable doesnt already
				// exist in symbols
				if (lookup(symbols, variable) == NULL){
					AddToMap(symbols, variable, (void*)(intptr_t)(variableStart+numVars), INT);
				}
				else{
					numVars += 1;
				}
				// Resetting variable
				invariable = false;
				varIndex = 0;
				read ++;
				continue;
			}
		}
		else{
			prevnewline = false;
		}
		
		if (invariable){
			variable[varIndex] = c;
			varIndex ++;
		}
		
		read ++;		
	}
	return insNum;
}


Token* parse(unsigned char* str, int n, SyntaxMap* syntax){
	// n = # of instructions = # of tokens
	Token* tokens = malloc(sizeof(Token)*n);
	int tokenIndex = 0;
	
	Instruction_type InstructionType;
	int start = 0;
	
	bool isvariable = false;
	
	unsigned char c;
	for (int i=0; (c = str[i]) != '\0'; i++){
		
		//if first charecter of instruction
		if (i-start == 0){
			if (c == '@'){
				InstructionType = A;
				//if value's first char 
				//is not numeric
				if (!(str[start + 1] >= '0' && str[start + 1] <= '9')){
					isvariable = true;
				}
				else{
					isvariable = false;
				}
			}
			else{
				InstructionType = C;
			}
		}
		
		// At the end of each instruction
		if (c == '\n'){
			//allocating more than enough
			//space for longest possible
			//assembly instruction
			char ins[15];
			
			// skipping over @ for A ins
			if (InstructionType == A){
				start ++;
			}
			
			//copying over instruction
			int index = 0;
			while (start < i){
				ins[index] = str[start];
				index++;
				start++;
				
				if (index > 14){
					fprintf(stderr, "Invalid Instruction\n--Instruction # %i\n", tokenIndex + 1);
					cleanUp(tokens, tokenIndex, syntax);
					return NULL;
				}
			}
			ins[index] = '\0';
			start += 1;
			
			
			if (InstructionType == A){
				
				tokens[tokenIndex].type = A;
				Instruction_A* instruction = malloc(sizeof(Instruction_A));
				
				if (!isvariable){
					int num = atoi(ins);
					instruction->value = num;
					tokens[tokenIndex].instruction = instruction;
					
					
				} else{
					Item* sym = lookup(syntax->symbols, ins);
					if (sym != NULL){
						instruction->value = ( intptr_t)sym->value;
						tokens[tokenIndex].instruction = instruction;
						
					}else{
						fprintf(stderr, "Reference to undefined variable\n--Instruction # %i\n", tokenIndex + 1);
						free(instruction);
						cleanUp(tokens, tokenIndex, syntax);
						return NULL;
					}
				}
			}
			else{
				tokens[tokenIndex].type = C;
				Instruction_C* instruction = malloc(sizeof(Instruction_C));
				//Each C instruction feild
				//can have at most 4 chars
				char* dest = malloc(4);
				char* comp = malloc(4);
				char* jdir = malloc(4);
				int error = split_C_IntoFields(dest, comp, jdir, ins, tokenIndex);
				
				if (error){
					cleanUp(tokens, tokenIndex, syntax);
					free(instruction);
					free(dest);
					free(comp);
					free(jdir);
					return NULL;
				}
				
				instruction->dest = dest;
				instruction->comp = comp;
				instruction->jdir = jdir;
				
				tokens[tokenIndex].instruction = instruction;
			}
			
			tokenIndex ++;
			continue;
		}
	}
	printf("parse: expected %d tokens, created %d tokens\n", n, tokenIndex);
	return tokens;
}


void freetokens(Token* tokens, int n){
	for (int i=0; i<n; i++){
		if (tokens[i].type == C){
			Instruction_C* ins = tokens[i].instruction;
			free(ins -> dest);
			free(ins -> comp);
			free(ins -> jdir);
		}
		free(tokens[i].instruction);
	}
	free(tokens);
}


SyntaxMap createSyntaxMap(){
	char cmp[] = "{"
               
               "0: '0101010',"
               "1: '0111111',"
               "-1: '0111010',"
               "D: '0001100',"
               "A: '0110000',"
               "!D: '0001101',"
               "!A: '0110001',"
               "-D: '0001111',"
               "-A: '0110011',"
               "D+1: '0011111',"
               "A+1: '0110111',"
               "D-1: '0001110',"
               "A-1: '0110010',"
               "D+A: '0000010',"
               "A+D: '0000010',"
               "D-A: '0010011',"
               "A-D: '0000111',"
               "D&A: '0000000',"
               "A&D: '0000000',"
               "D|A: '0010101',"
               "A|D: '0010101',"
               "M: '1110000',"
               "!M: '1110001',"
               "-M: '1110011',"
               "M+1: '1110111',"
               "M-1: '1110010',"
               "D+M: '1000010',"
               "M+D: '1000010',"
               "D-M: '1010011',"
               "M-D: '1000111',"
               "D&M: '1000000',"
               "M&D: '1000000',"
               "D|M: '1010101',"
               "M|D: '1010101',"
               
               "}";
	
	char dst[] = "{"

               "NA: '000',"
               "M: '001',"
               "D: '010',"
               "MD: '011',"
               "DM: '011',"
               "A: '100',"
               "AM: '101',"
               "MA: '101',"
               "AD: '110',"
               "DA: '110',"
               "AMD: '111',"
               "ADM: '111',"
               "MAD: '111',"
               "MDA: '111',"
               "DAM: '111',"
               "DMA: '111',"

               "}";


	char jmp[] = "{"

               "NA: '000',"
               "JGT: '001',"
               "JEQ: '010',"
               "JGE: '011',"
               "JLT: '100',"
               "JNE: '101',"
               "JLE: '110',"
               "JMP: '111',"

               "}";
    
	//BuiltIn symbols
	char sym[] = "{"

                 "R0: 0,"
                 "R1: 1,"
                 "R2: 2,"
                 "R3: 3,"
                 "R4: 4,"
                 "R5: 5,"
                 "R6: 6,"
                 "R7: 7,"
                 "R8: 8,"
                 "R9: 9,"
                 "R10: 10,"
                 "R11: 11,"
                 "R12: 12,"
                 "R13: 13,"
                 "R14: 14,"
                 "R15: 15,"

                 "SCREEN: 16384,"

                 "}";
	
	Hashmap* symbols = createHashmap(sym);
	Hashmap* dests = createHashmap(dst);
	Hashmap* comps = createHashmap(cmp);
	Hashmap* jdirs = createHashmap(jmp);
	
	return (SyntaxMap){
		.symbols = symbols,
		.dests = dests,
		.comps = comps,
		.jdirs = jdirs
	};
}


void clearSyntaxMap(SyntaxMap* syntax){
	clearMap(syntax -> dests);
	clearMap(syntax -> comps);
	clearMap(syntax -> jdirs);
	clearMap(syntax -> symbols);
	
	free(syntax -> dests -> items);
	free(syntax -> comps -> items);
	free(syntax -> jdirs -> items);
	free(syntax -> symbols -> items);
	
	free(syntax -> dests);
	free(syntax -> comps);
	free(syntax -> jdirs);
	free(syntax -> symbols);
}


void cleanUp(Token* tokens, int n, SyntaxMap* syntax){
	clearSyntaxMap(syntax);
	freetokens(tokens, n);
}


bool split_C_IntoFields(char* dest, char* comp, char* jdir, char* instruction, int insNum){
	i8 read = 0;
	i8 write = 0;
	i8 c = 0;
	
	//Indices
	i8 dest_index = 0;
	i8 comp_index = 0;
	i8 jdir_index = 0;
	//______________
		
	while ((c = instruction[read]) != '\0'){
		if (c == '='){
			while (write < read){
				dest[dest_index] = instruction[write];
				dest_index++;
				write++;
				if (dest_index > 3){
					fprintf(stderr, "too many destinations specified\n--instruction # %i\n", insNum);
					return true;
				}
			}
			dest[dest_index] = '\0';
			write++;
		}
		
		else if (c == ';' || (instruction[read+1] == '\0' && comp_index == 0)){
			int end = instruction[read+1] == '\0'? read+1 : read;// when read is at end
			        // write should also reach
			        // that charecter
			while (write < end){
				comp[comp_index] = instruction[write];
				comp_index++;
				write++;
				if (comp_index > 3){
					fprintf(stderr, "too many operands in computation\n--instruction # %i\n", insNum);
					return true;
				}
			}
			comp[comp_index] = '\0';
			write++;
		}
		
		else if (instruction[read+1] == '\0'){
			while (write <= read){
				jdir[jdir_index] = instruction[write];
				jdir_index++;
				write++;
				if (jdir_index > 3){
					fprintf(stderr, "Invalid jump directive\n--instruction # %i\n", insNum);
					return true;
				}
			}
			jdir[jdir_index] = '\0';
			write++;
		}
		read++;
	}
	
	if (comp_index == 0){
		fprintf(stderr, "invalid instruction: comp field cant be empty\n--instruction # %i\n", insNum);
		return true;
	}
	
	if (jdir_index == 0){
		jdir[0] = 'N'; // NA represents no
		jdir[1] = 'A'; // jump in jump table
	}
	
	if (dest_index == 0){
		dest[0] = 'N'; // NA represents no
		dest[1] = 'A'; // dest in dest table
	}
	return 0;
}


char* decimalToBinary(int n, bool* overflow) {
    // Hack architecture has a 15 bit address
    // space. 1 extra bit for null charecter
    char* binaryNum = malloc(16);
    binaryNum[15] = '\0';

    // Edge case for 0
    if (n == 0) {
        for (int i=0; i<15; i++){
        	binaryNum[i] = '0';
        }
        return binaryNum;
    }
    
    int i = 14;
    while (n > 0 && i>=0) {
        binaryNum[i] = (n%2==1)? '1':'0';
        n = n / 2;
        i--;
    }
    
    if (n > 0){
    	*overflow = true;
    }
    else{
    	*overflow = false;
    }
    
    while (i>=0){
    	binaryNum[i] = '0';
    	i--;
    }
    return binaryNum;
}


char* BinaryEncoder(Token* tokens, int n, SyntaxMap* syntax){
	char* machineCode = malloc(17*n + 1);
	int codeIndex = 0;
	//codeIndex points at the first
	//charecter index of
	//current instruction
	int insNum = 0;
	
	for (int i=0; i<n; i++){
		Token token = tokens[i];
		if (token.type == A){
			bool overflow = false;
			Instruction_A* ins = token.instruction;
			int val = ins->value;
			char* value = decimalToBinary(val, &overflow);		
			if (overflow){
				fprintf(stderr, "Invalid A Instruction: A value overflowed\n--Instruction # %i\n", insNum);
				return NULL;
			}
			// A instruction
			// opcode = 0
			machineCode[codeIndex] = '0';
			codeIndex++;
			
			// copying over the value
			i8 valLen = strlen(value);
			memcpy(&machineCode[codeIndex], value, valLen);
			codeIndex += valLen;
			
			free(value);
			insNum++;
		}
		else{
			
			Instruction_C* ins = token.instruction;
			Item* destcode = lookup(syntax->dests, ins->dest);
			Item* compcode = lookup(syntax->comps, ins->comp);
			Item* jdircode = lookup(syntax->jdirs, ins->jdir);
			
			if (destcode == NULL ){
				fprintf(stderr, "Invalid C Instruction: destination not supported\n--Instruction # %i --> %s\n", insNum, ins->dest);
				return NULL;
			}
			
			if (compcode == NULL){
				fprintf(stderr, "Invalid C Instruction: computation not supported\n--Instruction # %i -> %s\n", insNum, ins->comp);
				return NULL;
			}
			if (jdircode == NULL){
				fprintf(stderr, "Invalid C Instruction: jump directive not supported\n--Instruction # %i -> %s\n", insNum, ins->jdir);
				return NULL;
			}
			
			// C Instruction opcode and
			// padding
			memcpy(&machineCode[codeIndex], "111", 3);
			codeIndex+=3;
			
			// Copy dest bitstring
			int destLen = strlen(destcode->value);
			memcpy(&machineCode[codeIndex], destcode->value, destLen);
			codeIndex += destLen;
			
			// Copy comp bitstring
			int compLen = strlen(compcode->value);
			memcpy(&machineCode[codeIndex], compcode->value, compLen);
			codeIndex += compLen;
			
			// Copy jdir bitstring
			int jdirLen = strlen(jdircode->value);
			memcpy(&machineCode[codeIndex], jdircode->value, jdirLen);
			codeIndex += jdirLen;
			insNum++;
		}
		machineCode[codeIndex] = '\n';
		codeIndex++;
	}
	machineCode[codeIndex++] = '\0';
	printf("BinaryEncoder encoded %d Instructions\n", n);
	return machineCode;
}