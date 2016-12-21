/*
 * token.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

FILE* file_pointer_output;
char const *filename;
char filename_no_dot_j_global[200];
int count = 0;
int max_count = 0;

int lastOne = -1;
int if_max = -1;

int previousNesting[200];

//1 if previous token was defun, 0 otherwise
int previousWasDefun = 0;

//if 1, will stop processing tokens
int breakWhileLoop = 0;

void initialize_file_pointer(const char *file) {
	file_pointer_output = fopen(file, "w");
}

void close_file_pointer() {
	fclose(file_pointer_output);
}

int main(int argc, char const *argv[])
{
	filename = argv[1];

	FILE * file_pointer = fopen (filename, "rb");
	int token_response;


	if(argc < 2){
		printf("Trace takes at least one argument.\n");
		exit(0);
	}

	if (file_pointer == NULL){
		printf("File not found\n");
		exit(0);
	}

	//filename with .asm extension
	int len = strlen(filename);
	char filename_output[len + 2];
	for (int i = 0 ; i <= len-2 ; i++) {
		filename_output[i] = filename[i] ;
	}
	filename_output[len-1] = 'a' ;
	filename_output[len] = 's' ;
	filename_output[len+1] = 'm' ;
	filename_output[len+2] = '\0' ;

	//filename without extension
	//int len = strlen(filename);
	char filename_no_dot_j[len - 2];
	for (int i = 0 ; i <= len-3 ; i++) {
		filename_no_dot_j[i] = filename[i] ;
	}
	//null terminate to be safe
	filename_no_dot_j[len - 2] = '\0';
	strcpy(filename_no_dot_j_global, filename_no_dot_j);

	initialize_file_pointer(filename_output);

	fprintf(file_pointer_output, "%s\n", ".CODE");

	while(1){

		token_response = read_token(file_pointer);

		//break while loop, used by argN command
		if(breakWhileLoop){
			break;
		}

		//check if last token
		if(token_response == 1){
			break;
		}
	}
	close_file_pointer();
	return 0;
}

void handleToken(token *current_token){

	//CONSTANTS (literals)
	if(current_token->type == 1 || current_token->type == 2){
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #-1");
		int literal_value, imm9, uimm8 ; 
		literal_value = current_token->literal_value ;
		imm9 = (short)literal_value & 0xff ; 
		uimm8 = ((short)literal_value >> 8) & 0xff; 
		fprintf(file_pointer_output,"	%s%d\n","CONST R0, #", imm9);
		fprintf(file_pointer_output,"	%s%d\n","HICONST R0, #", uimm8); 
		fprintf(file_pointer_output, "	%s\n", "STR R0, R6, #0" );
		previousWasDefun = 0;
	}

	//ADD
	if(current_token->type == 3){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","ADD R0, R0, R1");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//SUB
	if(current_token->type == 4){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","SUB R0, R0, R1");
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
	}

	//MUL
	if(current_token->type == 5){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0"); 
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","MUL R0, R0, R1"); 
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");  
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//DIV
	if(current_token->type == 6){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","DIV R0, R0, R1");
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
	}

	//MOD
	if(current_token->type == 7){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","MOD R0, R0, R1");
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
	}

	//LT
	if(current_token->type == 8){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
        fprintf(file_pointer_output,"	%s%s%s%d\n","BRn ", filename_no_dot_j_global, "_COMP_GOOD_", count);
        fprintf(file_pointer_output,"	%s\n","CONST R0, #0");
        fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_GOOD_", count);
		fprintf(file_pointer_output,"	%s\n","CONST R0, #1");
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
        count++;
	}

	//LE
	if(current_token->type == 9){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
        fprintf(file_pointer_output,"	%s%s%s%d\n","BRnz ", filename_no_dot_j_global, "_COMP_GOOD_", count);
        fprintf(file_pointer_output,"	%s\n","CONST R0, #0");
        fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_GOOD_", count);
		fprintf(file_pointer_output,"	%s\n","CONST R0, #1");
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
        count++;
	}

	//EQ
	if(current_token->type == 10){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
        fprintf(file_pointer_output,"	%s%s%s%d\n","BRz ", filename_no_dot_j_global, "_COMP_GOOD_", count);
        fprintf(file_pointer_output,"	%s\n","CONST R0, #0");
        fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_GOOD_", count);
		fprintf(file_pointer_output,"	%s\n","CONST R0, #1");
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
        count++;
	}

	//GE
	if(current_token->type == 11){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
        fprintf(file_pointer_output,"	%s%s%s%d\n","BRzp ", filename_no_dot_j_global, "_COMP_GOOD_", count);
        fprintf(file_pointer_output,"	%s\n","CONST R0, #0");
        fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_GOOD_", count);
		fprintf(file_pointer_output,"	%s\n","CONST R0, #1");
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
        count++;
	}

	//GT
	if(current_token->type == 12){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
        fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
        fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
        fprintf(file_pointer_output,"	%s%s%s%d\n","BRp ", filename_no_dot_j_global, "_COMP_GOOD_", count);
        fprintf(file_pointer_output,"	%s\n","CONST R0, #0");
        fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_GOOD_", count);
		fprintf(file_pointer_output,"	%s\n","CONST R0, #1");
        fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_COMP_END_", count);
        fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
        fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
        previousWasDefun = 0;
        count++;
	}

	//AND
	if(current_token->type == 13){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","AND R0, R0, R1");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//OR
	if(current_token->type == 26){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","OR R0, R0, R1");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//NOT
	if(current_token->type == 27){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","NOT R0, R0");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//DROP
	if(current_token->type == 14){
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		previousWasDefun = 0;
	}

	//DUP
	if(current_token->type == 15){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #-1");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//SWAP
	if(current_token->type == 16){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","STR R1, R6, #0");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #1");
		previousWasDefun = 0;
	}

	//MAX
	if(current_token->type == 17){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","LDR R1, R6, #1");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		fprintf(file_pointer_output,"	%s\n","CMP R0, R1");
		fprintf(file_pointer_output,"	%s%s%s%d\n","BRn ", filename_no_dot_j_global, "_MAX_LT_", max_count);
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_MAX_END_", max_count);
		fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global,"_MAX_LT_", max_count);
		fprintf(file_pointer_output,"	%s\n","STR R1, R6, #0");
		fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global,"_MAX_END_", max_count);
		max_count++;
		previousWasDefun = 0;
	}

	//LOW8
	if(current_token->type == 18){
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","AND R1, R0, #15");
		fprintf(file_pointer_output,"	%s\n","SRL R0, R0, #4");
		fprintf(file_pointer_output,"	%s\n","AND R0, R0, #15");
		fprintf(file_pointer_output,"	%s\n","SLL R0, R0, #4");
		fprintf(file_pointer_output,"	%s\n","OR R0, R0, R1");
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//ARGn
	if(current_token->type == 19){
		int token_response = 0; 
		int number_arg;
		int offset; 
		number_arg = current_token->arg_no; 

		if(number_arg > 20){
			printf("ARGn takes n no greater than 20.\n"); 
			breakWhileLoop = 1;
		} 

		offset = 2 + number_arg ; 
		fprintf(file_pointer_output,"	%s%d\n","LDR R0, R5, #",offset);
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #-1"); 
		fprintf(file_pointer_output,"	%s\n","STR R0, R6, #0");
		previousWasDefun = 0;
	}

	//IF
	if(current_token->type == 20){
		previousNesting[if_max + 1] = lastOne; 
		lastOne = if_max + 1;
		if_max++;
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #1");
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #-1");
		fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_IF_", lastOne);
		fprintf(file_pointer_output,"	%s%s%s%d\n","BRz ", filename_no_dot_j_global, "_ELSE_", lastOne);
		previousWasDefun = 0;
	}

	//ELSE
	if(current_token->type == 21){
		fprintf(file_pointer_output,"	%s%s%s%d\n","JMP ", filename_no_dot_j_global, "_ENDIF_", lastOne);
		fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_ELSE_", lastOne);
		previousWasDefun = 0;
	}

	//ENDIF
	if(current_token->type == 22){
		fprintf(file_pointer_output,"%s%s%d\n",filename_no_dot_j_global, "_ENDIF_", lastOne);
		previousWasDefun = 0;
		lastOne = previousNesting[lastOne];
	}

	//DEFUN
	if(current_token->type == 23){
		fprintf(file_pointer_output, "%s\n", ".FALIGN");
		previousWasDefun = 1;
	}

	//FUNCTION NAME
	if(current_token->type == 25){
		if(previousWasDefun){
		fprintf(file_pointer_output, "%s\n", current_token->str);
		fprintf(file_pointer_output,"	%s\n",";; prologue");
		fprintf(file_pointer_output,"	%s\n","STR R7, R6, #-2");
		fprintf(file_pointer_output,"	%s\n","STR R5, R6, #-3");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #-3");
		fprintf(file_pointer_output,"	%s\n","ADD R5, R6, #0");
		fprintf(file_pointer_output,"	%s\n",";; function body");
		}
		else{
			fprintf(file_pointer_output,"	%s%s\n","LEA R7 ",current_token->str);
			fprintf(file_pointer_output,"	%s\n","JSRR R7");
		}
		previousWasDefun = 0;
	}

	//RETURN
	if(current_token->type == 24){
		fprintf(file_pointer_output,"	%s\n",";; function epilogue");
		fprintf(file_pointer_output,"	%s\n","LDR R0, R6, #0");
		fprintf(file_pointer_output,"	%s\n","STR R0, R5, #2");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R5, #0");
		fprintf(file_pointer_output,"	%s\n","ADD R6, R6, #2");
		fprintf(file_pointer_output,"	%s\n","LDR R7, R5, #1");
		fprintf(file_pointer_output,"	%s\n","LDR R5, R5, #0");
		fprintf(file_pointer_output,"	%s\n","RET");
		previousWasDefun = 0;
	}

	//DEBUG PURPOSES
	//printf("%s       type %d literal_value: %d arg_no: %d\n", current_token->str, current_token->type, current_token->literal_value, current_token->arg_no);
}