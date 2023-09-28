/*
 * lexer.h
 *
 *  Created on: 25-Feb-2023
 *      Author: user
 */

#ifndef LEXER_LEXER_H_
#define LEXER_LEXER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../Token/token.h"
#include "../Token/tag.h"

#define RESERVED_KEYWORD_COUNT 5

typedef struct lexer_t {
	int line;
	FILE *input;
	char peek;
	Token **reserved;
	int len;
	int top;
	void (*LexerInit)(struct lexer_t *lexer, FILE *file);
	void (*reserve)(struct lexer_t *lexer, Tag tag, char *lexeme);
	void (*readch)(struct lexer_t *lexer);
	int (*match)(struct lexer_t *lexer, char c);
	Token* (*scan)(struct lexer_t *lexer);
} Lexer;

typedef struct lexer_t Lexer;

void LexerInit(Lexer *lexer, FILE *file);
void reserve(Lexer *lexer, Tag tag, char *lexeme);
void readch(Lexer *lexer);
int match(Lexer *lexer, char c);
Token* scan(Lexer *lexer);

Token** create_reserved() {
	Token **temp = (Token**) calloc(RESERVED_KEYWORD_COUNT, sizeof(Token*));
	for (int i = 0; i < RESERVED_KEYWORD_COUNT; i++) {
		*(temp + i) = CreateToken();
	}
	return temp;
}

Lexer* CreateLexer() {
	Lexer *temp = (Lexer*) malloc(sizeof(Lexer));
	if (temp == NULL) {
		printf("Error Creating Lexer\n");
		return NULL;
	}
	temp->line = 1;
	temp->peek = ' ';
	temp->reserved = create_reserved();
	temp->len = RESERVED_KEYWORD_COUNT;
	temp->top = 0;
	temp->LexerInit = &LexerInit;
	temp->match = &match;
	temp->readch = &readch;
	temp->reserve = &reserve;
	temp->scan = &scan;
	temp->reserve(temp, IF, "if");
	temp->reserve(temp, ELSE, "else");
	temp->reserve(temp, WHILE, "while");
	temp->reserve(temp, DO, "do");
	temp->reserve(temp, BREAK, "break");
	return temp;
}

void LexerInit(Lexer *lexer, FILE *file) {
	lexer->input = file;
}
void reserve(Lexer *lexer, Tag tag, char *lexeme) {
	lexer->reserved[lexer->top]->WordInit(lexer->reserved[lexer->top], tag,
			lexeme);
	lexer->top = lexer->top + 1;
}
void readch(Lexer *lexer) {
	lexer->peek = fgetc(lexer->input);
}
int match(Lexer *lexer, char c) {
	lexer->readch(lexer);
	if (lexer->peek != c)
		return 0;
	return 1;
}
Token* scan(Lexer *lexer) {
	Token *t = CreateToken();
	char *str = malloc(sizeof(char));
	int j = 1;
	if (feof(lexer->input)) {
		t->TokenInit(t, eof);
		fclose(lexer->input);
		return t;
	}
	for (; !feof(lexer->input) || lexer->peek == '\0'; lexer->readch(lexer)) {
		if (lexer->peek == ' ' || lexer->peek == '\t') {
			continue;
		} else if (lexer->peek == '\n') {
			lexer->line = lexer->line + 1;
		} else {
			break;
		}
	}
	printf("peek : %c\n",lexer->peek);
	if (lexer->peek == '\0' || lexer->peek < 0) {
		t->TokenInit(t, eof);
		fclose(lexer->input);
		return t;
	}
	switch (lexer->peek) {
	case '&': {
		if (lexer->match(lexer, '&')) {
			t->WordInit(t, AND, "&&");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, BIT_AND, "&");
			return t;
		}
	}
	case '|': {
		if (lexer->match(lexer, '|')) {
			t->WordInit(t, OR, "||");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, BIT_OR, "|");
			return t;
		}
	}
	case '=': {
		if (lexer->match(lexer, '=')) {
			t->WordInit(t, EQ, "==");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, ASSIGN, "=");
			return t;
		}
	}
	case '!': {
		if (lexer->match(lexer, '=')) {
			t->WordInit(t, NE, "!=");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, NOT, "!");
			return t;
		}
	}
	case '<': {
		if (lexer->match(lexer, '=')) {
			t->WordInit(t, LE, "<=");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, LT, "<");
			return t;
		}
	}
	case '>': {
		if (lexer->match(lexer, '=')) {
			t->WordInit(t, GE, ">=");
			lexer->readch(lexer);
			return t;
		} else {
			t->WordInit(t, GT, ">");
			return t;
		}
	}
	case '+': {
		t->WordInit(t, PLUS, "+");
		lexer->readch(lexer);
		return t;
	}
	case '-': {
		t->WordInit(t, MINUS, "-");
		lexer->readch(lexer);
		return t;
	}
	case '*': {
		t->WordInit(t, MULTIPLY, "*");
		lexer->readch(lexer);
		return t;
	}
	case '/': {
		if (lexer->match(lexer, '/')) {
			while (lexer->peek != '\n')
				lexer->readch(lexer);
			return lexer->scan(lexer);
		}
		t->WordInit(t, DIVIDE, "/");
		return t;
	}
	case '(': {
		t->WordInit(t, LPARAN, "(");
		lexer->readch(lexer);
		return t;
	}
	case ')': {
		t->WordInit(t, RPARAN, ")");
		lexer->readch(lexer);
		return t;
	}
	case '[': {
		t->WordInit(t, RSBRAC, "[");
		lexer->readch(lexer);
		return t;
	}
	case ']': {
		t->WordInit(t, LSBRAC, "]");
		lexer->readch(lexer);
		return t;
	}
	case '{': {
		t->WordInit(t, RCBRAC, "{");
		lexer->readch(lexer);
		return t;
	}
	case '}': {
		t->WordInit(t, LCBRAC, "}");
		lexer->readch(lexer);
		return t;
	}
	case '\'': {
		char charac[2];
		charac[1] = '\0';
		lexer->readch(lexer);
		charac[0]=lexer->peek;
		if(!lexer->match(lexer,'\'')) {
			//error handling
			return lexer->scan(lexer);
		}
		t->WordInit(t, CHARA, charac);
		lexer->readch(lexer);
		return t;
	}
	case '\"': {
		lexer->readch(lexer);
		while (lexer->peek!='\"' && lexer->peek!='\n') {
			str = realloc(str, j * sizeof(char));
			str[j - 1] = lexer->peek;
			j += 1;
			lexer->readch(lexer);
		}
		str[j - 1] = '\0';
		if(lexer->match(lexer,'\"')!=0) {
			//error handling
			return lexer->scan(lexer);
		}
		t->WordInit(t, STRING, str);
		lexer->readch(lexer);
		return t;
	}
	}

	if (isdigit(lexer->peek)) {
		int v = 0;
		do {
			v = v * 10 + (lexer->peek - '0');
			lexer->readch(lexer);
		} while (isdigit(lexer->peek));
		if (lexer->peek != '.') {
			t->NumInit(t, v);
			return t;
		}
		float x = v;
		float d = 10;
		for (;;) {
			lexer->readch(lexer);
			if (!(isdigit(lexer->peek)))
				break;
			x = x + (int) strtol(&(lexer->peek), NULL, 10) / d;
			d = d * 10;
		}
		t->RealInit(t, x);
		return t;
	}
	if (isalpha(lexer->peek) || lexer->peek == '_') {
		do {
			str = realloc(str, j * sizeof(char));
			str[j - 1] = lexer->peek;
			j += 1;
			lexer->readch(lexer);
		} while (isalnum(lexer->peek) || lexer->peek == '_');
		str[j - 1] = '\0';
		for (int i = 0; i < lexer->top; i++) {
			if (strcmp(lexer->reserved[i]->lexeme, str) == 0) {
				free(t);
				return lexer->reserved[i];
			}
		}
		if (!strcmp(str,"int") || !strcmp(str,"float") || !strcmp(str,"char") || !strcmp(str,"bool")) {

			t->WordInit(t,DATATYPE,str);
			return t;
		}
		if (!strcmp(str,"true")) {
			t->WordInit(t,TRUE,str);
			return t;
		}
		if (!strcmp(str,"false")) {
			t->WordInit(t,FALSE,str);
			return t;
		}
		t->WordInit(t, ID, str);
		return t;
	}
	if (lexer->peek == ';') {
		t->WordInit(t, SEMI, ";");
		lexer->readch(lexer);
		return t;
	}
	if (lexer->peek == ',') {
		t->WordInit(t, COMA, ",");
		lexer->readch(lexer);
		return t;
	}
	printf("Error Unidentified Token at line %d\n", lexer->line);
	t->TokenInit(t, UNIDENTIFIED);
	lexer->readch(lexer);
	return t;
}

#endif /* LEXER_LEXER_H_ */
