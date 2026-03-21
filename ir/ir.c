#include "ir.h"

#include <stdio.h>
#include <string.h>

/* need to define
void ir_init(ir_result *ir);
void ir_add_symbol(ir_result *ir, const char *name, const char *lang, const char *file, int line);
void ir_add_dependency(ir_result *ir, const char *from_file, const char *module, const char *type, const char *lang);
*/

void ir_init(ir_result *ir) {
    ir->symbol_count = 0;
    ir->dep_count = 0;
}

void ir_add_symbol(ir_result *ir, const char *name, const char *lang, const char *file, int line) {
    if (ir->symbol_count >= IR_MAX_SYMBOLS) {
        fprintf(stderr, "IR symbol table limit reached, skipping function: %s\n", name);
        return;
    }
    ir_symbol *syb = &ir->symbols[ir->symbol_count++];
    strncpy(syb->name, name, sizeof(syb->name));
    strncpy(syb->lang, lang, sizeof(syb->lang));
    strncpy(syb->file, file, sizeof(syb->file));
    syb->line = line;

}

void ir_add_dependency(ir_result *ir, const char *from_file, const char *module, const char *type, const char *lang) {
    if(ir->dep_count >= IR_MAX_DEPS) {
        fprintf(stderr, "IR dependency table limit reached, skipping dependency: %s -> %s\n", from_file, module);
        return;
    }
    ir_dep *dep = &ir->deps[ir->dep_count];
    strncpy(dep->from_file, from_file, sizeof(dep->from_file));
    strncpy(dep->module, module, sizeof(dep->module));
    strncpy(dep->type, type, sizeof(dep->type));
    strncpy(dep->lang, lang, sizeof(dep->lang));
    ir->dep_count++;
}