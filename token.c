/*
 * token.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"
#include "jc.h"


int read_token(FILE *theFile){
	token current_token;

	//what will be returned, helps avoid a compiler warning
	int returnInt = 0;

	char buffer[MAX_TOKEN_LENGTH];
	char * components;

	//return 1 on success and final token
	if(feof(theFile)){
		returnInt = 1;
	}

	//get line and process into tokens
	if (fgets(buffer, MAX_TOKEN_LENGTH, theFile) != NULL){
		components = strtok(buffer," \n\t");
		while (components != NULL){

			//if comment, stop reading line
			if(components[0] == ';'){
				break;
			}

			//reset token
			current_token.type = 0;
			current_token.literal_value = 0;
			current_token.arg_no = 0;

			// HEX
			if(components[0] == '0' && components[1] == 'x'){
				current_token.type = 1;
				current_token.literal_value = strtol(components, NULL, 0);
				current_token.arg_no = 0;
			}

			// NEGATIVE DECIMAL
			else if(components[0] == '-' && isdigit(components[1])){
				current_token.type = 2;
				current_token.literal_value = strtol(components, (char **)NULL, 10);
				current_token.arg_no = 0;
			}

			// DECIMAL
			else if(isdigit(components[0])){
				current_token.type = 2;
				current_token.literal_value = strtol(components, (char **)NULL, 10);
				current_token.arg_no = 0;
			}

			// ADD
			else if(components[0] == '+'){
				current_token.type = 3;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// SUBTRACT
			else if(components[0] == '-'){
				current_token.type = 4;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// MULTIPLY
			else if(components[0] == '*'){
				current_token.type = 5;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// DIVIDE
			else if(components[0] == '/'){
				current_token.type = 6;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// MOD
			else if(components[0] == '%'){
				current_token.type = 7;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Less than
			else if(components[0] == 'l' && components[1] == 't'){
				current_token.type = 8;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Less or equal
			else if(components[0] == 'l' && components[1] == 'e'){
				current_token.type = 9;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Equal
			else if(components[0] == 'e' && components[1] == 'q'){
				current_token.type = 10;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Greater or equal
			else if(components[0] == 'g' && components[1] == 'e'){
				current_token.type = 11;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Greater than
			else if(components[0] == 'g' && components[1] == 't'){
				current_token.type = 12;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// AND
			else if(components[0] == 'a' && components[1] == 'n' && components[2] == 'd'){
				current_token.type = 13;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// OR
			else if(components[0] == 'o' && components[1] == 'r'){
				current_token.type = 26;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// NOT
			else if(components[0] == 'n' && components[1] == 'o' && components[2] == 't'){
				current_token.type = 27;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// DROP
			else if(components[0] == 'd' && components[1] == 'r' && components[2] == 'o' && components[3] == 'p'){
				current_token.type = 14;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// DUP
			else if(components[0] == 'd' && components[1] == 'u' && components[2] == 'p'){
				current_token.type = 15;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// SWAP
			else if(components[0] == 's' && components[1] == 'w' && components[2] == 'a' && components[3] == 'p'){
				current_token.type = 16;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// MAX
			else if(components[0] == 'm' && components[1] == 'a' && components[2] == 'x'){
				current_token.type = 17;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// LOW8
			else if(components[0] == 'l' && components[1] == 'o' && components[2] == 'w' && components[3] == '8'){
				current_token.type = 18;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// argN
			else if(components[0] == 'a' && components[1] == 'r' && components[2] == 'g' && isdigit(components[3])){
				current_token.type = 19;
				current_token.literal_value = 0;
				current_token.arg_no = strtol(components + 3, (char **)NULL, 10); // +3 to get rid of arg
			}

			// IF
			else if(components[0] == 'i' && components[1] == 'f'){
				current_token.type = 20;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// ELSE
			else if(components[0] == 'e' && components[1] == 'l' && components[2] == 's' && components[3] == 'e'){
				current_token.type = 21;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// ENDIF
			else if(components[0] == 'e' && components[1] == 'n' && components[2] == 'd' && components[3] == 'i' && components[4] == 'f'){
				current_token.type = 22;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Defun
			else if(components[0] == 'd' && components[1] == 'e' && components[2] == 'f' && components[3] == 'u' && components[4] == 'n'){
				current_token.type = 23;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			// Return
			else if(components[0] == 'r' && components[1] == 'e' && components[2] == 't' && components[3] == 'u' && components[4] == 'r' && components[5] == 'n'){
				current_token.type = 24;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			//if none of those, assume it's a function name
			else{
				current_token.type = 25;
				current_token.literal_value = 0;
				current_token.arg_no = 0;
			}

			//set token string
			strcpy(current_token.str,components);

			handleToken(&current_token);

			//reset components
			components = strtok (NULL, " \n\t");
		}
	}

	return returnInt;
}