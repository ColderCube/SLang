/*
 * ast_helper.c
 *
 *  Created on: 04-Mar-2023
 *      Author: user
 */

#ifndef UTIL_AST_HELPER_C_
#define UTIL_AST_HELPER_C_

#include <stdio.h>

#include "../Ast/ast.h"

void print_tab(int tab);

void print_ast(Ast *ast, int tab) {
	switch (ast->asttype) {
	case SEQ: {
		print_tab(tab++);
		printf("Sequence \n");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;
	}
	case BooL: {
		print_tab(tab++);
		printf("Bool [ OR ]\n");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;
	}
	case JOIN: {
		print_tab(tab++);
		printf("JOIN [ AND ]\n");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;

	}
	case EQUALITY: {
		print_tab(tab++);
		printf("EQUALITY [ %s ]\n", ast->token->tag==EQ ? "EQ":"NE");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;

	}
	case REL: {
		print_tab(tab++);
		printf("REL [ %s ]\n",ast->token->lexeme);
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;

	}
	case EXPR: {
		print_tab(tab++);
		printf("Expression [ %s ]\n",ast->token->lexeme);
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;

	}
	case TERM: {
		print_tab(tab++);
		printf("TERM [ %s ]\n",ast->token->lexeme);
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;

	}
	case UNARY: {
		print_tab(tab++);
		printf("UNARY [ %s ]\n",ast->token->lexeme);
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		break;

	}
	case FACTOR : {
		print_tab(tab++);
		printf("Factor\n");
		print_tab(tab);
		switch (ast->symbol->type) {
		case CONST_INT: {
			printf("%d\n", ast->symbol->ival);
			break;
		}
		case CONST_FLOAT: {
			printf("%f\n", ast->symbol->fval);
			break;
		}
		case CONST_CHAR: {
			printf("%c\n", ast->symbol->cval);
			break;
		}
		case CONST_BOOL: {
			printf("%s\n", ast->symbol->bool == 1 ? "True" : "False");
			break;
		}
		case VAR_INT:
		case VAR_FLOAT:
		case VAR_CHAR:
		case VAR_BOOL: {
			printf("%s %d\n", ast->symbol->name, ast->symbol->offset);
			break;
		}
		}
		break;
	}
	case ASSIGNMENT : {
		print_tab(tab++);
		printf("Assignment\n");
		print_tab(tab);
		printf("%s %d\n", ast->symbol->name, ast->symbol->offset);
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		break;

	}
	case IFS : {
		print_tab(tab++);
		printf("IF \n");
		print_tab(tab);
		printf("Condition : \n");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		print_tab(tab);
		printf("Body : \n");
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		break;
	}
	case IFELSE: {
		print_tab(tab++);
		printf("IF \n");
		print_tab(tab);
		printf("Condition : \n");
		if (ast->node1 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node1, tab);
		}
		print_tab(tab);
		printf("Body : \n");
		if (ast->node2 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node2, tab);
		}
		print_tab(tab);
		printf("ELSE \n");
		print_tab(tab);
		printf("Body : \n");
		if (ast->node3 == NULL) {
			print_tab(tab);
			printf("NULL\n");
		} else {
			print_ast(ast->node3, tab);
		}
		break;
	}
	}
}
void print_tab(int tab) {
	for (int i = 0; i < tab; i++) {
		printf("   ");
	}
}
#endif /* UTIL_AST_HELPER_C_ */
