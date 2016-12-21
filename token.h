/*
 * token.h
 */

#pragma once
#define MAX_TOKEN_LENGTH 200
#include <stdio.h>

// Token Type:
// 0: Not Set!
// 1: Hex
// 2: Decimal
// 3: Add
// 4: Minus
// 5: Multiplication
// 6: Divide
// 7: Mod
// 8: Less Than
// 9: Less or equal
// 10: Equal
// 11: Greater or equal
// 12: Greater than
// 13: And
// 14: Drop
// 15: Duplicate
// 16: Swap
// 17: Max
// 18: Low8
// 19: ArgN
// 20: If
// 21: Else
// 22: EndIf
// 23: Defun
// 24: Return
// 25: function name
// 26: Or
// 27: Not

typedef struct {
  int type;             // token type
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];
} token;

//Returns 0 on success
//Returns 1 on success and final token
//Returns 2 on broken token
int read_token (FILE *theFile);

