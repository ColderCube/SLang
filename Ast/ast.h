/*
 * newast.h
 *
 *  Created on: 06-May-2023
 *      Author: user
 */

#ifndef AST_NEWAST_H_
#define AST_NEWAST_H_

#include "../Symbol/symbol.h"
#include "../Token/token.h"

char getprefix(SymbolType s);

enum ast_type_e {
	SEQ,
	EXPR,
	IFS,
	IFELSE,
	WHILELOOP,
	DOWHILE,
	ASSIGNMENT,
	REL,
	BooL,
	JOIN,
	EQUALITY,
	ARITH,
	UNARY,
	TERM,
	FACTOR
};

typedef enum ast_type_e AstType;

struct ast_t {

	AstType asttype;
	Token *token;

	struct ast_t *node1;
	struct ast_t *node2;
	struct ast_t *node3;
	struct ast_t *node4;

	Symbol *symbol;
	int ival;
	float fval;
	int boolean;
	char cval;
	char *sval;
	int assign_offset;
	char *label1;
	char *label2;

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
			struct ast_t *body_ast);
	void (*IfElseStatementInit)(struct ast_t *ast, struct ast_t *cond_ast,
				struct ast_t *body_ast, struct ast_t *else_body_ast);
	void (*WhileStatement)(struct ast_t *ast, struct ast_t *cond_ast, struct ast_t *body_ast);
};

typedef struct ast_t Ast;

void SeqInit(Ast *ast, Ast *stmt, Ast *nextstmt);
void BoolInit(Ast *ast, Ast *bool_ast1, Ast *bool_ast2);
void JoinInit(Ast *ast, Ast *join_ast1, Ast *join_ast2);
void EqualityInit(Ast *ast, Ast *equality_ast1, Token *token,
		Ast *equality_ast2);
void RelInit(Ast *ast, Ast *rel_ast1, Token *token, Ast *rel_ast2);
void ExprInit(Ast *ast, Ast *expr_ast1, Token *token, Ast *expr_ast2);
void TermInit(Ast *ast, Ast *term_ast1, Token *token, Ast *term_ast2);
void UnaryInit(Ast *ast, Ast *unary_ast, Token *token);
void FactorInit(Ast *ast, Symbol *symbol);
void AssignmentInit(Ast *ast, Symbol *symbol, Ast *bool_ast);
void IfStatementInit(Ast *ast, Ast *cond_ast, Ast *body_ast);
void IfElseStatementInit(Ast *ast, Ast *cond_ast, Ast *ifbody_ast, Ast *elsebody_ast);
void WhileInit(Ast *ast, Ast *cond_ast, Ast *body_ast);

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
	temp->IfElseStatementInit = IfElseStatementInit;
	return temp;
}

void SeqInit(Ast *ast, Ast *stmt, Ast *nextstmt) {
	ast->asttype = SEQ;
	ast->node1 = stmt;
	ast->node2 = nextstmt;
}

void BoolInit(Ast *ast, Ast *bool_ast1, Ast *bool_ast2) {
	ast->asttype = BooL;
	ast->dtype =
			bool_ast1->dtype == bool_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->node1 = bool_ast1;
	ast->node2 = bool_ast2;
}

void JoinInit(Ast *ast, Ast *join_ast1, Ast *join_ast2) {
	ast->asttype = JOIN;
	ast->dtype =
			join_ast1->dtype == join_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->node1 = join_ast1;
	ast->node2 = join_ast2;
}

void EqualityInit(Ast *ast, Ast *equality_ast1, Token *token,
		Ast *equality_ast2) {
	ast->asttype = EQUALITY;
	ast->dtype =
			equality_ast1->dtype == equality_ast2->dtype ?
					BOOLEAN : (DataType) NULL;
	ast->node1 = equality_ast1;
	ast->node2 = equality_ast2;
	ast->token = token;
}

void RelInit(Ast *ast, Ast *rel_ast1, Token *token, Ast *rel_ast2) {
	ast->asttype = REL;
	ast->dtype = rel_ast1->dtype == rel_ast2->dtype ? BOOLEAN : (DataType) NULL;
	ast->node1 = rel_ast1;
	ast->node2 = rel_ast2;
	ast->token = token;
}

void ExprInit(Ast *ast, Ast *expr_ast1, Token *token, Ast *expr_ast2) {
	ast->asttype = EXPR;
	ast->dtype = max(expr_ast1->dtype, expr_ast2->dtype);
	if (ast->dtype == -1) {
		printf("Type Error\n");
	}
	ast->node1 = expr_ast1;
	ast->node2 = expr_ast2;
	ast->token = token;
}

void TermInit(Ast *ast, Ast *term_ast1, Token *token, Ast *term_ast2) {
	ast->asttype = ARITH;
	ast->dtype = max(term_ast1->dtype, term_ast2->dtype);
	if (ast->dtype == -1) {
		printf("Type Error\n");
	}
	ast->node1 = term_ast1;
	ast->node2 = term_ast2;
	ast->token = token;
}

void UnaryInit(Ast *ast, Ast *unary_ast, Token *token) {
	ast->asttype = UNARY;
	ast->dtype = max(INTEGER, unary_ast->dtype);
	if (ast->dtype == -1 && token->tag != NOT) {
		printf("Type Error\n");
	}
	ast->node1 = unary_ast;
	ast->token = token;
}

void FactorInit(Ast *ast, Symbol *symbol) {
	ast->asttype = FACTOR;
	ast->dtype = symbol->dtype;
	ast->symbol = symbol;
}

void AssignmentInit(Ast *ast, Symbol *symbol, Ast *bool_ast) {
	ast->asttype = ASSIGNMENT;
	ast->assign_offset = symbol->offset;
	ast->node1 = bool_ast;
	ast->dtype = symbol->dtype;
	ast->symbol = symbol;
}

void IfStatementInit(Ast *ast, Ast *cond_ast, Ast *body_ast) {
	ast->asttype = IFS;
	ast->node1 = cond_ast;
	ast->node2 = body_ast;
//	ast->label1=endif_label;
//	print_ast(cond_ast, 0);
//	print_ast(body_ast, 0);
}

void IfElseStatementInit(Ast *ast, Ast *cond_ast, Ast *ifbody_ast, Ast *elsebody_ast) {
	ast->asttype = IFELSE;
	ast->node1 = cond_ast;
	ast->node2 = ifbody_ast;
	ast->node3 = elsebody_ast;
}

#endif /* AST_NEWAST_H_ */
