/*
 * main.c
 *
 *  Created on: 23-Feb-2023
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>

#include "Token/token.h"
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "util/ast_helper.h"

int main() {
	printf("Hello World\n");
	FILE* file = fopen("source.slang","r");
	if (file==NULL) {
		printf("Error While Opening the file\n");
		return -1;
	}
	Lexer *lexer = CreateLexer();
	lexer->LexerInit(lexer,file);
	Parser* parser = CreateParser();
	parser->ParserInit(parser,lexer);
	Ast* ast = parser->parseStatements(parser);
	print_ast(ast,0);
//	for (int i=0;i<11;i++) {
//		Ast* ast1 = parser->parseStatement(parser);
//		if (ast1!=NULL) {
//			if (ast1->asttype == IFS) {
//				printf("If statement\n");
//			}
//			print_ast(ast1, 0);
//
//		}
//	}
	printf("Symbol Table Size : %d\n",parser->symtab->max);
	return 1;
}
