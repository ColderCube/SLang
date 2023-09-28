/*
 * symbol.h
 *
 *  Created on: 27-Feb-2023
 *      Author: user
 */

#ifndef SYMBOL_SYMBOL_TABLE_H_
#define SYMBOL_SYMBOL_TABLE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "symbol.h"

typedef struct symboltable_t {
	Symbol **table;
	uint32_t len;
	uint16_t max;
	uint32_t pos;
	struct symboltable_t *prev;
	void (*SymbolTableInit)(struct symboltable_t *sym,
			struct symboltable_t *prev);
	void (*shrink)(struct symboltable_t *sym);
	int (*put)(struct symboltable_t *sym, Symbol *var);
	Symbol* (*getScope)(struct symboltable_t *sym, char *name);
	Symbol* (*get)(struct symboltable_t *sym, char *name);
} SymbolTable;

void SymbolTableInit(SymbolTable *sym, SymbolTable *prev);
void shrink(SymbolTable *sym);
int put(SymbolTable *sym, Symbol *var);
Symbol* get(SymbolTable *sym, char *name);
Symbol* getScope(SymbolTable *sym, char *name);

SymbolTable* CreateSymbolTable() {
	SymbolTable *temp = (SymbolTable*) malloc(sizeof(SymbolTable));
	if (temp == NULL) {
		printf("Error Creating SymbolTable\n");
		return NULL;
	}
	temp->SymbolTableInit = SymbolTableInit;
	temp->shrink = shrink;
	temp->put = put;
	temp->get = get;
	temp->getScope = getScope;
	return temp;
}

void SymbolTableInit(SymbolTable *sym, SymbolTable *prev) {
	sym->prev = prev;
	if (prev != NULL) {
		sym->pos = prev->pos;
	} else {
		sym->pos = 0;
	}
	sym->max = 2;
	sym->len = 0;
	sym->table = (Symbol**) calloc(sym->max, sizeof(Symbol*));
}

int put(SymbolTable *sym, Symbol *var) {
	Symbol *s = sym->getScope(sym, var->name);
	if (s != NULL) {
		printf("Variable already defined");
		return 0;
	}
	if (sym->len < sym->max) {
		sym->table[sym->len] = var;
	} else {
		sym->max *= 2;
		sym->table = (Symbol**) realloc(sym->table, sym->max * sizeof(Symbol*));
		sym->table[sym->len] = var;
	}
	sym->len += 1;
	sym->pos += 1;
	return 1;
}

Symbol* getScope(SymbolTable *sym, char *name) {
	if (sym->len == 0)
		return NULL;
	for (int i = 0; i < sym->len; i++) {
		if (!strcmp(sym->table[i]->name, name)) {
			return sym->table[i];
		}
	}
	return NULL;
}

Symbol* get(SymbolTable *sym, char *name) {
	SymbolTable *symtab = sym;
	Symbol *s;
	while (symtab != NULL) {
		s = symtab->getScope(symtab, name);
		if (s != NULL)
			return s;
		symtab = (symtab->prev);
	}
	return NULL;
}

void FreeSymbolTable(SymbolTable *symtab) {
//	for (int i = 0; i < symtab->len; i++)
//		FreeSymbol(symtab->table[i]);
	free(symtab->table);
	free(symtab);
}

void shrink(SymbolTable *sym) {
}

#endif /* SYMBOL_SYMBOL_TABLE_H_ */
