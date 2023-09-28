/*
 * token.h
 *
 *  Created on: 24-Feb-2023
 *      Author: user
 */

#ifndef TOKEN_TOKEN_H_
#define TOKEN_TOKEN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tag.h"

typedef struct token_t {
	Tag tag;

	// Word
	char *lexeme;

	//NUM
	int int_num;

	//REAL
	float float_num;

	char* (*toString)(struct token_t *token);
	void (*TokenInit)(struct token_t *token, Tag tag);
	void (*WordInit)(struct token_t *token, Tag tag, char *lexeme);
	void (*NumInit)(struct token_t *token, int value);
	void (*RealInit)(struct token_t *token, float value);

} Token;

typedef struct token_t Token;

char* toString(Token *token);
void TokenInit(Token *token, Tag tag);
void WordInit(Token *token, Tag tag, char *lexeme);
void NumInit(Token *token, int value);
void RealInit(Token *token, float value);

Token* CreateToken() {
	Token *temp = (Token*) malloc(sizeof(Token));
	if (temp == NULL) {
		printf("Error Creating Token\n");
		return NULL;
	}
	temp->lexeme = NULL;
	temp->NumInit = &NumInit;
	temp->RealInit = &RealInit;
	temp->TokenInit = &TokenInit;
	temp->WordInit = &WordInit;
	temp->toString = &toString;
	return temp;
}

char* toString(Token *token) {
	char str[512];
	if (token->tag == NUM) {
		sprintf(str, "NUM : %d", token->int_num);
		return strdup(str);
	} else if (token->tag == REAL) {
		sprintf(str, "REAL : %f", token->float_num);
		return strdup(str);
	} else if (token->lexeme != NULL) {
		return strdup(token->lexeme);
	} else {
		sprintf(str, "%I64d", token->tag);
		return strdup(str);
	}
}
void TokenInit(Token *token, Tag tag) {
	token->tag = tag;
}
void WordInit(Token *token, Tag tag, char *lexeme) {
	token->TokenInit(token, tag);
	token->lexeme = strdup(lexeme);
}
void NumInit(Token *token, int value) {
	token->TokenInit(token, NUM);
	token->int_num = value;
}
void RealInit(Token *token, float value) {
	token->TokenInit(token, REAL);
	token->float_num = value;
}

void FreeToken(Token* token) {
	if (token->lexeme!=NULL)
		free(token->lexeme);
	free(token);
}

#endif /* TOKEN_TOKEN_H_ */
