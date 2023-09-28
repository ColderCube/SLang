/*
 * ast.h
 *
 *  Created on: 28-Feb-2023
 *      Author: user
 */

#ifndef AST_AST_H_
#define AST_AST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Token/token.h"
#include "../Parser/parser.h"

char getprefix(SymbolType s);

enum ast_type_e {
	SEQ,
	IFS,
	IFELSE,
	WHILELOOP,
	DOWHILE,
	ASSIGNMENT,
	REL,
	BooL,
	JOIN,
	EQUALITY,
	EXPR,
	TERM,
	UNARY,
	FACTOR
};

typedef enum ast_type_e AstType;

struct ast_t {

	AstType asttype;
	Token *token;

	//Statements
	struct ast_t *stmt;
	struct ast_t *nextstmt;

	//If
	struct ast_t *if_condition;
	struct ast_t *ifstmt;
	char *endif_label;
	//..Else
	char *else_if_label;
	struct ast_t *elsestmt;

	//While
	struct ast_t *while_condition;
	struct ast_t *whilestmt;
	char *while_label;

	//Do..While
	char *do_label;
	struct ast_t *dostmt;
	struct ast_t *do_condition;

	//Assign
	int assign_offset;
	struct ast_t *value_bool;

	//Bool
	struct ast_t *bool_ast1;
	struct ast_t *bool_ast2;

	//Join
	struct ast_t *join_ast1;
	struct ast_t *join_ast2;

	//Equality
	struct ast_t *equality_ast1;
	struct ast_t *equality_ast2;

	//Rel
	struct ast_t *rel_ast1;
	struct ast_t *rel_ast2;

	//Expr
	struct ast_t *expr_ast1;
	struct ast_t *expr_ast2;

	//Term
	struct ast_t *term_ast1;
	struct ast_t *term_ast2;

	//Unary
	struct ast_t *unary_ast;

	//Factor
	Symbol *symbol;
	int ival;
	float fval;
	int boolean;
	char cval;
	char *sval;

	int id_offset;

	DataType dtype;

	void (*SeqInit)(struct ast_t *ast, struct ast_t *stmt,
			struct ast_t *nextstmt);
	void (*BoolInit)(struct ast_t *ast, struct ast_t *bool_ast1,
			struct ast_t *bool_ast2);
	void (*JoinInit)(struct ast_t *ast, struct ast_t *join_ast1,
			struct ast_t *join_ast2);
	void (*EqualityInit)(struct ast_t *ast, struct ast_t *equality_ast1,
			Token *token, struct ast_t *equality_ast2);
	void (*RelInit)(struct ast_t *ast, struct ast_t *rel_ast1, Token *token,
			struct ast_t *rel_ast2);
	void (*ExprInit)(struct ast_t *ast, struct ast_t *expr_ast1, Token *token,
			struct ast_t *expr_ast2);
	void (*TermInit)(struct ast_t *ast, struct ast_t *term_ast1, Token *token,
			struct ast_t *term_ast2);
	void (*UnaryInit)(struct ast_t *ast, struct ast_t *unary_ast, Token *token);
	void (*FactorInit)(struct ast_t *ast, Symbol *symbol);
	void (*AssignmentInit)(struct ast_t *ast, Symbol *symbol,
			struct ast_t *bool_ast);

	void (*IfStatementInit)(struct ast_t *ast, struct ast_t *cond_ast,
			struct ast_t *body_ast, char *endif_label);
	void (*Ifemit)(struct ast_t *ast);

	void (*emit)(struct ast_t *ast);
};

typedef struct ast_t Ast;

#include "../util/ast_helper.h"

void SeqInit(Ast *ast, Ast *stmt, Ast *nextstmt);
void Seqemit(Ast *ast);

void BoolInit(Ast *ast, Ast *bool_ast1, Ast *bool_ast2);
void Boolemit(Ast *ast);

void JoinInit(Ast *ast, Ast *join_ast1, Ast *join_ast2);
void Joinemit(Ast *ast);

void EqualityInit(Ast *ast, Ast *equality_ast1, Token *token,
		Ast *equality_ast2);
void Equalityemit(Ast *ast);

void RelInit(Ast *ast, Ast *rel_ast1, Token *token, Ast *rel_ast2);
void Relemit(Ast *ast);

void ExprInit(Ast *ast, Ast *expr_ast1, Token *token, Ast *expr_ast2);
void Expremit(Ast *ast);

void TermInit(Ast *ast, Ast *term_ast1, Token *token, Ast *term_ast2);
void Termemit(Ast *ast);

void UnaryInit(Ast *ast, Ast *unary_ast, Token *token);
void Unaryemit(Ast *ast);

void FactorInit(Ast *ast, Symbol *symbol);
void Factoremit(Ast *ast);

void AssignmentInit(Ast *ast, Symbol *symbol, Ast *bool_ast);
void Assignmentemit(Ast *ast);

void IfStatementInit(Ast *ast, Ast *cond_ast, Ast *body_ast, char *endif_label);
void Ifemit(Ast *ast);

Ast* CreateAst() {
	Ast *temp = (Ast*) malloc(sizeof(Ast));
	if (temp == NULL) {
		printf("Error Creating Ast\n");
		return NULL;
	}
	temp->SeqInit = SeqInit;
	temp->AssignmentInit = AssignmentInit;
	temp->BoolInit = BoolInit;
	temp->EqualityInit = EqualityInit;
	temp->ExprInit = ExprInit;
	temp->FactorInit = FactorInit;
	temp->JoinInit = JoinInit;
	temp->RelInit = RelInit;
	temp->TermInit = TermInit;
	temp->UnaryInit = UnaryInit;
	temp->IfStatementInit = IfStatementInit;
	return temp;
}

void SeqInit(Ast *ast, Ast *stmt, Ast *nextstmt) {
	ast->asttype = SEQ;
	ast->stmt = stmt;
	ast->nextstmt = nextstmt;
	ast->emit = Seqemit;
}

void Seqemit(Ast *ast) {
	if (ast->stmt != NULL)
		ast->stmt->emit(ast->stmt);
	if (ast->nextstmt != NULL)
		ast->nextstmt->emit(ast->nextstmt);
}

void BoolInit(Ast *ast, Ast *bool_ast1, Ast *bool_ast2) {
	ast->asttype = BooL;
	ast->dtype =
			bool_ast1->dtype == bool_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->bool_ast1 = bool_ast1;
	ast->bool_ast2 = bool_ast2;
	ast->emit = Boolemit;
}

void Boolemit(Ast *ast) {
	ast->bool_ast1->emit(ast->bool_ast1);
	ast->bool_ast2->emit(ast->bool_ast2);
	printf("OR\n");
}

void JoinInit(Ast *ast, Ast *join_ast1, Ast *join_ast2) {
	ast->asttype = JOIN;
	ast->dtype =
			join_ast1->dtype == join_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->join_ast1 = join_ast1;
	ast->join_ast2 = join_ast2;
	ast->emit = Joinemit;
}

void Joinemit(Ast *ast) {
	ast->join_ast1->emit(ast->join_ast1);
	ast->join_ast2->emit(ast->join_ast2);
	printf("AND\n");
}

void EqualityInit(Ast *ast, Ast *equality_ast1, Token *token,
		Ast *equality_ast2) {
	ast->asttype = EQUALITY;
	ast->dtype =
			equality_ast1->dtype == equality_ast2->dtype ?
					BOOLEAN : (DataType) NULL;
	ast->equality_ast1 = equality_ast1;
	ast->equality_ast2 = equality_ast2;
	ast->token = token;
	ast->emit = Equalityemit;
}

void Equalityemit(Ast *ast) {
	ast->equality_ast1->emit(ast->equality_ast1);
	ast->equality_ast2->emit(ast->equality_ast2);
	switch (ast->token->tag) {
	case EQ: {
		printf("EQ\n");
		break;
	}
	case NE: {
		printf("NE\n");
		break;
	}
	default: {
	}
	}
}

void RelInit(Ast *ast, Ast *rel_ast1, Token *token, Ast *rel_ast2) {
	ast->asttype = REL;
	ast->dtype = rel_ast1->dtype == rel_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->rel_ast1 = rel_ast1;
	ast->rel_ast2 = rel_ast2;
	ast->token = token;
	ast->emit = Relemit;
}

void Relemit(Ast *ast) {
	ast->rel_ast1->emit(ast->rel_ast1);
	ast->rel_ast2->emit(ast->rel_ast2);
	switch (ast->token->tag) {
	case LT: {
		printf("LT\n");
		break;
	}
	case LE: {
		printf("LE\n");
		break;
	}
	case GT: {
		printf("GT\n");
		break;
	}
	case GE: {
		printf("GE\n");
		break;
	}
	default: {
	}
	}
}

void ExprInit(Ast *ast, Ast *expr_ast1, Token *token, Ast *expr_ast2) {
	ast->asttype = EXPR;
	ast->dtype = max(expr_ast1->dtype, expr_ast2->dtype);
	if (ast->dtype == -1) {
		printf("Type Error\n");
	}
	ast->expr_ast1 = expr_ast1;
	ast->expr_ast2 = expr_ast2;
	ast->token = token;
	ast->emit = Expremit;
}

void Expremit(Ast *ast) {
	ast->expr_ast1->emit(ast->expr_ast1);
	ast->expr_ast2->emit(ast->expr_ast2);
	switch (ast->token->tag) {
	case PLUS: {
		printf("ADD\n");
		break;
	}
	case MINUS: {
		printf("SUB\n");
		break;
	}
	default: {
	}
	}
}

void TermInit(Ast *ast, Ast *term_ast1, Token *token, Ast *term_ast2) {
	ast->asttype = TERM;
	ast->dtype = max(term_ast1->dtype, term_ast2->dtype);
	if (ast->dtype == -1) {
		printf("Type Error\n");
	}
	ast->equality_ast1 = term_ast1;
	ast->equality_ast2 = term_ast2;
	ast->token = token;
	ast->emit = Termemit;
}

void Termemit(Ast *ast) {
	ast->term_ast1->emit(ast->term_ast1);
	ast->term_ast2->emit(ast->term_ast2);
	switch (ast->token->tag) {
	case MULTIPLY: {
		printf("MUL\n");
		break;
	}
	case DIVIDE: {
		printf("DIV\n");
		break;
	}
	default: {
	}
	}
}

void UnaryInit(Ast *ast, Ast *unary_ast, Token *token) {
	ast->asttype = UNARY;
	ast->dtype = max(INTEGER, unary_ast->dtype);
	if (ast->dtype == -1 && token->tag!=NOT) {
		printf("Type Error\n");
	}
	ast->unary_ast = unary_ast;
	ast->token = token;
	ast->emit = Unaryemit;
}

void Unaryemit(Ast *ast) {
	ast->unary_ast->emit(ast->unary_ast);
	switch (ast->token->tag) {
	case MINUS: {
		printf("NEG\n");
		break;
	}
	case NOT: {
		printf("NOT\n");
		break;
	}
	default: {
	}
	}
}

void FactorInit(Ast *ast, Symbol *symbol) {
	ast->asttype = FACTOR;
	ast->dtype = symbol->dtype;
	ast->symbol = symbol;
	ast->emit = Factoremit;
}

void Factoremit(Ast *ast) {
	switch (ast->symbol->type) {
	case CONST_INT: {
		printf("CIPUSH %d\n", ast->symbol->ival);
		break;
	}
	case CONST_FLOAT: {
		printf("CFPUSH %f\n", ast->symbol->fval);
		break;
	}
	case CONST_CHAR: {
		printf("CCPUSH %c\n", ast->symbol->cval);
		break;
	}
	case CONST_BOOL: {
		printf("%s\n", ast->symbol->bool == 1 ? "CBTPUSH" : "CBFPUSH");
		break;
	}
	case VAR_INT:
	case VAR_FLOAT:
	case VAR_CHAR:
	case VAR_BOOL: {
		printf("V%cPUSH %d\n", getprefix(ast->symbol->type),
				ast->symbol->offset);
		break;
	}
	default: {
	}
	}
}

void AssignmentInit(Ast *ast, Symbol *symbol, Ast *bool_ast) {
	ast->asttype = ASSIGNMENT;
	ast->assign_offset = symbol->offset;
	ast->value_bool = bool_ast;
	ast->dtype = symbol->dtype;
	ast->symbol = symbol;
	ast->emit = Assignmentemit;
}

void Assignmentemit(Ast *ast) {
	ast->value_bool->emit(ast->value_bool);
	printf("V%cSTORE %d\n", getprefix(ast->symbol->type), ast->symbol->offset);
}

void IfStatementInit(Ast *ast, Ast *cond_ast, Ast *body_ast, char *endif_label) {
	ast->asttype = IFS;
	ast->if_condition = cond_ast;
	ast->ifstmt = body_ast;
	ast->endif_label = endif_label;
	ast->emit = Ifemit;
	print_ast(cond_ast, 0);
	print_ast(body_ast, 0);
}

void Ifemit(Ast *ast) {
	ast->if_condition->emit(ast->if_condition);
	printf("JNE %s\n", ast->endif_label);
	ast->ifstmt->emit(ast->ifstmt);
	printf("%s\n", ast->endif_label);
}

#endif /* AST_AST_H_ */
