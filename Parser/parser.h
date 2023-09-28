/*
 * parser.h
 *
 *  Created on: 28-Feb-2023
 *      Author: user
 */

#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Token/token.h"
#include "../Lexer/lexer.h"
#include "../Symbol/symbol_table.h"
#include "../Ast/ast.h"
#include "../util/ast_helper.h"

typedef struct parser_t {
	Lexer *lexer;
	Token *look;
	Token *prev;
	SymbolTable *symtab;
	Ast *root;

	int label;

	void (*ParserInit)(struct parser_t *parser, Lexer *lexer);
	void (*readToken)(struct parser_t *parser);
	int (*match)(struct parser_t *parser, Tag tag);
	char* (*genLabel)(struct parser_t *parser);
	Ast* (*parseProgram)(struct parser_t *parser);
	Ast* (*parseStatements)(struct parser_t *parser);
	Ast* (*parseStatement)(struct parser_t *parser);
	Ast* (*parseBlockStatement)(struct parser_t *parser);
	Ast* (*parseDeclStatement)(struct parser_t *parser, Token *dtype);
	Ast* (*parseAssignmentStatement)(struct parser_t *parser);
	Ast* (*parseBool)(struct parser_t *parser);
	Ast* (*parseJoin)(struct parser_t *parser);
	Ast* (*parseEquality)(struct parser_t *parser);
	Ast* (*parseRel)(struct parser_t *parser);
	Ast* (*parseExpression)(struct parser_t *parser);
	Ast* (*parseTerm)(struct parser_t *parser);
	Ast* (*parseUnary)(struct parser_t *parser);
	Ast* (*parseFactor)(struct parser_t *parser);
} Parser;

typedef struct parser_t Parser;

void ParserInit(Parser *parser, Lexer *lexer);
void readToken(Parser *parser);
int pmatch(Parser *parser, Tag tag);
char* genLabel(Parser *parser);
Ast* parseProgram(Parser *parser);
Ast* parseStatements(Parser *parser);
Ast* parseStatement(Parser *parser);
Ast* parseBlockStatement(Parser *parser);
Ast* parseDeclStatement(Parser *parser, Token *dtype);
Ast* parseAssignmentStatement(Parser *parser);
Ast* parseBool(Parser *parser);
Ast* parseJoin(Parser *parser);
Ast* parseEquality(Parser *parser);
Ast* parseRel(Parser *parser);
Ast* parseExpression(Parser *parser);
Ast* parseTerm(Parser *parser);
Ast* parseUnary(Parser *parser);
Ast* parseFactor(Parser *parser);

int getsize(char *type);
SymbolType getsymboltype(char *type);
char getprefix(SymbolType s);

Parser* CreateParser() {
	Parser *temp = (Parser*) malloc(sizeof(Parser));
	if (temp == NULL) {
		printf("Error Creating Parser\n");
		return NULL;
	}
	temp->symtab = CreateSymbolTable();
	temp->symtab->SymbolTableInit(temp->symtab, NULL);
	temp->ParserInit = ParserInit;
	temp->readToken = readToken;
	temp->match = pmatch;
	temp->genLabel = genLabel;
	temp->parseProgram = parseProgram;
	temp->parseStatements = parseStatements;
	temp->parseStatement = parseStatement;
	temp->parseBlockStatement = parseBlockStatement;
	temp->parseAssignmentStatement = parseAssignmentStatement;
	temp->parseDeclStatement = parseDeclStatement;
	temp->parseBool = parseBool;
	temp->parseJoin = parseJoin;
	temp->parseEquality = parseEquality;
	temp->parseRel = parseRel;
	temp->parseExpression = parseExpression;
	temp->parseTerm = parseTerm;
	temp->parseUnary = parseUnary;
	temp->parseFactor = parseFactor;

	temp->label = 0;
	return temp;
}

void ParserInit(Parser *parser, Lexer *lexer) {
	parser->lexer = lexer;
	parser->readToken(parser);
}

void readToken(Parser *parser) {
	parser->prev = parser->look;
	parser->look = parser->lexer->scan(parser->lexer);
}

int pmatch(Parser *parser, Tag tag) {
	parser->readToken(parser);
	if (parser->look->tag == tag)
		return 1;
	return 0;
}

char* genLabel(Parser *parser) {
	char label[20] = "L";
	sprintf(label, "L%d", parser->label);
	parser->label += 1;
	return strdup(label);
}

Ast* parseProgram(Parser *parser) {
	printf("Start:\n");
	parser->root = parser->parseStatements(parser);
	printf("End:\n");
	return parser->root;
}

Ast* parseStatements(Parser *parser) {
	Ast *x = NULL, *y = NULL, *z = NULL;
	if (parser->look->tag != eof) {
		x = CreateAst();
		y = parser->parseStatement(parser);
		z = parser->parseStatements(parser);
		x->SeqInit(x, y, z);
	}
	return x;
}

Ast* parseBlockStatement(Parser *parser) {
	SymbolTable *prev = parser->symtab;
	SymbolTable *new = CreateSymbolTable();
	new->SymbolTableInit(new, prev);
	parser->symtab = new;
	Ast *x = NULL, *y = NULL, *z = NULL;
	while (parser->look->tag != eof && parser->look->tag != LCBRAC) {
		z = parser->parseStatement(parser);
//		print_ast(z,0);
		x = CreateAst();
		x->SeqInit(x, y, z);
		y = x;
	}
	FreeSymbolTable(new);
	parser->symtab = prev;
	return x;
}

Ast* parseStatement(Parser *parser) {
	Ast *x = CreateAst();
	if (parser->look->tag == RCBRAC) {
		parser->readToken(parser);
		x = parser->parseBlockStatement(parser);
		parser->readToken(parser);
		return x;
	} else if (parser->look->tag == WHILE) {
		parser->match(parser, LPARAN);
		parser->readToken(parser);
		Ast *y = parser->parseBool(parser);
		parser->readToken(parser);
//		printf("JNE %s\n", endloop);
		Ast *z = parser->parseStatement(parser);
//		x->
//		printf("%s:\n", endloop);

	} else if (parser->look->tag == DO) {
		parser->readToken(parser);
//		char *startloop = parser->genLabel(parser);
//		printf("%s:\n", startloop);
		Ast * body = parser->parseStatement(parser);
		parser->readToken(parser);
		parser->readToken(parser);
		Ast* cond = parser->parseBool(parser);
		parser->readToken(parser);
		parser->readToken(parser);
//		printf("JE %s\n", startloop);
	} else if (parser->look->tag == IF) {
		parser->match(parser, LPARAN);
		parser->readToken(parser);
		Ast *cond = parser->parseBool(parser);
//		y->emit(y);
		parser->readToken(parser);
//		char *endif = parser->genLabel(parser);
//		printf("JNE %s\n", endif);
		Ast *body = parser->parseStatement(parser);
		if (parser->look->tag == ELSE) {
//			char *else_label = endif;
//			endif = parser->genLabel(parser);
//			printf("GOTO %s\n", endif);
//			printf("%s:\n", else_label);
			parser->readToken(parser);
			Ast *ebody = parser->parseStatement(parser);
			x->IfElseStatementInit(x, cond, body, ebody);
		} else {
			x->IfStatementInit(x, cond, body);
		}
		return x;
//		printf("%s:\n", endif);
	} else if (parser->look->tag == DATATYPE) {
		Token *t1 = parser->look;
		if (!parser->match(parser, ID)) {
			// error handling
		} else {
			if (parser->match(parser, LPARAN)) {

			} else {
				x = parser->parseDeclStatement(parser, t1);
				parser->readToken(parser);
				return x;
			}
		}
	} else if (parser->look->tag == ID) {
		Symbol *s = parser->symtab->get(parser->symtab, parser->look->lexeme);
		if (s == NULL) {
			printf("\nUndefined\n");
			parser->readToken(parser);
			return NULL;
		}
		parser->match(parser, ASSIGN);
		x = parser->parseAssignmentStatement(parser);
		parser->readToken(parser);
		return x;
	}
	return NULL;
}

Ast* parseAssignmentStatement(Parser *parser) {
	Ast *x = CreateAst(), *y = NULL;
	Symbol *s = parser->symtab->get(parser->symtab,
			strdup(parser->prev->lexeme));
	parser->readToken(parser);
	y = parser->parseBool(parser);
	x->AssignmentInit(x, s, y);
//	printf("V%cSTORE %d\n", getprefix(s->type), s->offset);
	return x;
}

Ast* parseDeclStatement(Parser *parser, Token *dtype) {
	Ast *x = NULL, *y = NULL, *z = NULL;
	Symbol *s = parser->symtab->getScope(parser->symtab, parser->prev->lexeme);
	if (s != NULL) {
		// error handling
	} else {
		s = CreateSymbol();
		if (parser->look->tag != RSBRAC) {

		}
		s->LocalVariableInit(s, parser->prev->lexeme,
				getsymboltype(dtype->lexeme), 0, parser->symtab->pos);
		s->size = getsize(dtype->lexeme);
		parser->symtab->put(parser->symtab, s);
		printf("%s %d\n", s->name, s->offset);
		if (parser->look->tag == ASSIGN) {
			parser->readToken(parser);
			y = parser->parseBool(parser);
			x = CreateAst();
			x->AssignmentInit(x, s, y);
//			printf("V%cSTORE %d\n", getprefix(s->type), s->offset);
		}
		if (parser->look->tag == COMA) {
			parser->readToken(parser);
			parser->readToken(parser);
			y = parser->parseDeclStatement(parser, dtype);
			z = CreateAst();
			z->SeqInit(z, x, y);
			x = z;
		}
	}
	return x;
}

Ast* parseBool(Parser *parser) {
	Ast *x = parser->parseJoin(parser);
	Ast *y = NULL, *z = NULL;
	while (parser->look->tag == OR) {
		parser->readToken(parser);
		y = parser->parseJoin(parser);
		z = CreateAst();
		z->BoolInit(z, x, y);
		x = z;
//		printf("OR\n");
	}
	return x;
}

Ast* parseJoin(Parser *parser) {
	Ast *x = parser->parseEquality(parser);
	Ast *y = NULL, *z = NULL;
	while (parser->look->tag == AND) {
		parser->readToken(parser);
		y = parser->parseEquality(parser);
		;
		z = CreateAst();
		z->JoinInit(z, x, y);
		x = z;
//		printf("AND\n");
	}
	return x;
}

Ast* parseEquality(Parser *parser) {
	Ast *x = parser->parseRel(parser);
	Ast *y = NULL, *z = NULL;
	while (parser->look->tag == EQ || parser->look->tag == NE) {
		Token *t = parser->look;
		parser->readToken(parser);
		y = parser->parseRel(parser);
		z = CreateAst();
		z->EqualityInit(z, x, t, y);
		x = z;
//		if (parser->look->tag == EQ) {
//			parser->readToken(parser);
//			y=parser->parseRel(parser);
//			z=CreateAst();
//			z->EqualityInit(z,x,parser->look,y);
//			x=z;
////			printf("EQ\n");
//		}
//		if (parser->look->tag == NE) {
//			parser->readToken(parser);
//			parser->parseRel(parser);
////			printf("NE\n");
//		}
	}
	return x;
}

Ast* parseRel(Parser *parser) {
	Ast *x = parser->parseExpression(parser);
	Ast *y = NULL, *z = NULL;
	switch (parser->look->tag) {
	case LT: // {
//		parser->readToken(parser);
//		parser->parseExpression(parser);
//		printf("LT\n");
//		break;
//	}
	case LE: // {
//		parser->readToken(parser);
//		parser->parseExpression(parser);
//		printf("LE\n");
//		break;
//	}
	case GT: // {
//		parser->readToken(parser);
//		parser->parseExpression(parser);
//		printf("GT\n");
//		break;
//	}
	case GE: { // {
//		parser->readToken(parser);
//		parser->parseExpression(parser);
//		printf("GE\n");
//		break;
//	}

		Token *t = parser->look;
		parser->readToken(parser);
		y = parser->parseExpression(parser);
		z = CreateAst();
		z->RelInit(z, x, t, y);
		return z;
		break;
	}
	default: {
		break;
	}
	}
	return x;
}

Ast* parseExpression(Parser *parser) {
	Ast *x = parser->parseTerm(parser);
	Ast *y = NULL, *z = NULL;
	while (parser->look->tag == PLUS || parser->look->tag == MINUS) {
		Token *t = parser->look;
		parser->readToken(parser);
		y = parser->parseTerm(parser);
		z = CreateAst();
		z->ExprInit(z, x, t, y);
		x = z;
//		if (parser->look->tag == PLUS) {
//			parser->readToken(parser);
//			parser->parseTerm(parser);
//			printf("ADD\n");
//		} else {
//			parser->readToken(parser);
//			parser->parseTerm(parser);
//			printf("SUB\n");
//		}
	}
	return x;
}

Ast* parseTerm(Parser *parser) {
	Ast *x = parser->parseUnary(parser);
	Ast *y = NULL, *z = NULL;
	while (parser->look->tag == MULTIPLY || parser->look->tag == DIVIDE) {
		parser->readToken(parser);
		y = parser->parseRel(parser);
		z = CreateAst();
		z->TermInit(z, x, parser->look, y);
		x = z;
//		if (parser->look->tag == MULTIPLY) {
//			parser->readToken(parser);
//			parser->parseUnary(parser);
//			printf("MUL\n");
//		} else {
//			parser->readToken(parser);
//			parser->parseUnary(parser);
//			printf("DIV\n");
//		}
	}
	return x;
}

Ast* parseUnary(Parser *parser) {
	Ast *x = NULL, *y = NULL;
	if (parser->look->tag == MINUS) {
		Token *t = parser->look;
		parser->readToken(parser);
		y = parser->parseUnary(parser);
		x = CreateAst();
		x->UnaryInit(x, y, t);
//		printf("NEG\n");
	} else if (parser->look->tag == NOT) {
		Token *t = parser->look;
		parser->readToken(parser);
		y = parser->parseUnary(parser);
		x = CreateAst();
		x->UnaryInit(x, y, t);
//		printf("NOT\n");
	} else {
		return parser->parseFactor(parser);
	}
	return x;
}

Ast* parseFactor(Parser *parser) {
	Ast *x = NULL;
	Symbol *s = NULL;
	switch (parser->look->tag) {
	case LPARAN: {
		parser->readToken(parser);
		x = parser->parseBool(parser);
		parser->readToken(parser);
		break;
	}
	case NUM: {
//		printf("CIPUSH %d\n", parser->look->int_num);
		s = CreateSymbol();
		s->SymbolIntInit(s, parser->look->int_num);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
	case REAL: {
//		printf("CFPUSH %f\n", parser->look->float_num);
		s = CreateSymbol();
		s->SymbolFloatInit(s, parser->look->float_num);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
	case CHARA: {
//		printf("CCPUSH %s\n", parser->look->lexeme);
		s = CreateSymbol();
		s->SymbolCharInit(s, parser->look->lexeme[0]);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
//	case STRING: {
//		printf("CSPUSH %s\n", parser->look->lexeme);
//		parser->readToken(parser);
//		break;
//	}
	case TRUE: {
//		printf("CBTPUSH\n");
		s = CreateSymbol();
		s->SymbolBoolInit(s, 1);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
	case FALSE: {
//		printf("CBFPUSH\n");
		s = CreateSymbol();
		s->SymbolBoolInit(s, 0);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
	case ID: {
		Symbol *s = parser->symtab->get(parser->symtab, parser->look->lexeme);
		if (s == NULL) {
			printf("\nUndefined ID %s\n", parser->look->lexeme);
			parser->readToken(parser);
			return NULL;
		}
//		printf("V%cPUSH %d\n", getprefix(s->type), s->offset);
		x = CreateAst();
		x->FactorInit(x, s);
		parser->readToken(parser);
		break;
	}
	default: {
		printf("\nFactor Error \n %s \n", parser->look->lexeme);
		return NULL;
	}
	}
	return x;
}

int getsize(char *type) {
	if (!strcmp(type, "int")) {
		return sizeof(int);
	} else if (!strcmp(type, "float")) {
		return sizeof(float);
	} else { // (!strcmp(type,"char")) {
		return sizeof(char);
	}
}

SymbolType getsymboltype(char *type) {
	if (!strcmp(type, "int")) {
		return VAR_INT;
	} else if (!strcmp(type, "float")) {
		return VAR_FLOAT;
	} else if (!strcmp(type, "char")) { // (!strcmp(type,"char")) {
		return VAR_CHAR;
	} else if (!strcmp(type, "bool")) {
		return VAR_BOOL;
	} else {
		// ERROR
		return -1;
	}
}

char getprefix(SymbolType s) {
	if (s == VAR_INT)
		return 'I';
	else if (s == VAR_FLOAT)
		return 'F';
	else if (s == VAR_CHAR)
		return 'C';
	else if (s == VAR_BOOL)
		return 'B';
	return ' ';
}

#endif /* PARSER_PARSER_H_ */
