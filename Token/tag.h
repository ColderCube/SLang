/*
 * tag.h
 *
 *  Created on: 23-Feb-2023
 *      Author: user
 */

#ifndef TOKEN_TAG_H_
#define TOKEN_TAG_H_

enum tag_e {
	eof,
	SEMI,
	AND,
	BASIC,
	BREAK,
	BIT_AND,
	BIT_OR,
	BIT_NOT,
	COMA,
	CHARA,
	STRING,
	RPARAN,
	LPARAN,
	RSBRAC,
	LSBRAC,
	RCBRAC,
	LCBRAC,
	DATATYPE,
	DO,
	ELSE,
	EQ,
	FALSE,
	GE,
	GT,
	ID,
	IF,
	INDEX,
	LE,
	LT,
	ASSIGN,
	MINUS,
	NE,
	NOT,
	PLUS,
	MULTIPLY,
	DIVIDE,
	NUM,
	OR,
	REAL,
	TEMP,
	TRUE,
	WHILE,
	UNIDENTIFIED=LLONG_MAX
};

typedef enum tag_e Tag;



#endif /* TOKEN_TAG_H_ */
