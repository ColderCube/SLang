/*
 * symbol.h
 *
 *  Created on: 27-Feb-2023
 *      Author: user
 */

#ifndef SYMBOL_SYMBOL_H_
#define SYMBOL_SYMBOL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT sizeof(int)
#define FLOAT sizeof(float)
#define CHAR 1
#define BOOL 1

enum variable_type_e {
	VAR_INT, VAR_FLOAT, VAR_CHAR, VAR_STR, VAR_BOOL,

	CONST_INT, CONST_FLOAT, CONST_CHAR, CONST_STR, CONST_BOOL
};

typedef enum variable_type_e SymbolType;

enum datatype_e {
	INTEGER, FLOATING, CHARACTER, BOOLEAN
};

typedef enum datatype_e DataType;

typedef struct symbol_t {
	char *name;
	SymbolType type;
	int is_arr;
	int size;
	int offset;
	int defined;

	DataType dtype;

	int ival;
	float fval;
	char cval;
	char *sval;
	int bool;

	void (*SymbolIntInit)(struct symbol_t *symbol, int value);
	void (*SymbolFloatInit)(struct symbol_t *symbol, float value);
	void (*SymbolCharInit)(struct symbol_t *symbol, char value);
	void (*SymbolBoolInit)(struct symbol_t *symbol, int value);
	void (*SymbolVarInit)(struct symbol_t *symbol, char *name);
	void (*VariableInit)(struct symbol_t *symbol, char *name, SymbolType type,
			int is_arr);
	void (*LocalVariableInit)(struct symbol_t *symbol, char *name,
			SymbolType type, int is_arr, int offset);
} Symbol;

typedef struct symbol_t Symbol;

void SymbolIntInit(struct symbol_t *symbol, int value);
void SymbolFloatInit(struct symbol_t *symbol, float value);
void SymbolCharInit(struct symbol_t *symbol, char value);
void SymbolBoolInit(struct symbol_t *symbol, int value);
void SymbolVarInit(Symbol *symbol, char *name);
void VariableInit(Symbol *symbol, char *name, SymbolType type, int is_arr);
void LocalVariableInit(Symbol *symbol, char *name, SymbolType type, int is_arr,
		int offset);

Symbol* CreateSymbol() {
	Symbol *temp = (Symbol*) malloc(sizeof(Symbol));
	if (temp == NULL) {
		printf("Error Creating Symbol\n");
		return NULL;
	}
	temp->SymbolIntInit = SymbolIntInit;
	temp->SymbolFloatInit = SymbolFloatInit;
	temp->SymbolCharInit = SymbolCharInit;
	temp->SymbolBoolInit = SymbolBoolInit;
	temp->LocalVariableInit = LocalVariableInit;
	temp->SymbolVarInit = SymbolVarInit;
	temp->VariableInit = VariableInit;
	temp->defined = 0;
	return temp;
}

void SymbolIntInit(struct symbol_t *symbol, int value) {
	symbol->ival = value;
	symbol->type = CONST_INT;
	symbol->dtype = INTEGER;
}
void SymbolFloatInit(struct symbol_t *symbol, float value) {
	symbol->fval = value;
	symbol->type = CONST_FLOAT;
	symbol->dtype = FLOAT;
}
void SymbolCharInit(struct symbol_t *symbol, char value) {
	symbol->cval = value;
	symbol->type = CONST_CHAR;
	symbol->dtype = CHARACTER;
}
void SymbolBoolInit(struct symbol_t *symbol, int value) {
	symbol->bool = value;
	symbol->type = CONST_BOOL;
	symbol->dtype = BOOLEAN;
}
void SymbolVarInit(Symbol *symbol, char *name) {
	symbol->name = strdup(name);
}
void VariableInit(Symbol *symbol, char *name, SymbolType type, int is_arr) {
	symbol->SymbolVarInit(symbol, name);
	symbol->type = type;
	switch (type) {
	case CONST_INT:
	case VAR_INT: {
		symbol->dtype = INTEGER;
		break;
	}
	case CONST_FLOAT:
	case VAR_FLOAT: {
		symbol->dtype = FLOATING;
		break;
	}
	case CONST_CHAR:
	case VAR_CHAR: {
		symbol->dtype = CHARACTER;
		break;
	}
	case CONST_BOOL:
	case VAR_BOOL: {
		symbol->dtype = BOOLEAN;
		break;
	}
	default : {
		symbol->dtype = -1;
		break;
	}
	}
	symbol->is_arr = is_arr;
}
void LocalVariableInit(Symbol *symbol, char *name, SymbolType type, int is_arr,
		int offset) {
	symbol->VariableInit(symbol, name, type, is_arr);
	symbol->offset = offset;
}

void FreeSymbol(Symbol *symbol) {
	if (symbol->type < 4)
		free(symbol->name);
	if (symbol->type == CONST_STR || symbol->type == VAR_STR)
		free(symbol->sval);
	free(symbol);
}

int numeric(DataType dtype) {
	if (dtype== CHARACTER ||dtype== INTEGER || dtype== FLOATING)
		return 1;
	return 0;
}

DataType max(DataType dtype1, DataType dtype2) {
	if (!numeric(dtype1) || !numeric(dtype2)) {
		return -1;
	} else if (dtype1 == FLOATING || dtype2 == FLOATING) {
		return FLOATING;
	} else if (dtype1 == INTEGER || dtype2 == INTEGER) {
		return INTEGER;
	} else {
		return CHARACTER;
	}
}

#endif /* SYMBOL_SYMBOL_H_ */
