#ifndef IR_H
#define IR_H

#include <stddef.h>

#define IR_SYMBOLS 512
#define IR_DEPS    1024

typedef struct {
    char name[64];
    char lang[8]; 
    char file[256];
    int line;
} ir_symbol;

// for dependency graph
typedef struct{
    char from_file[256];
    char module[256];
    char type[24];
    char lang[8];
} ir_dep;

typedef struct {
    ir_symbol symbols[IR_SYMBOLS];
    int       symbol_count;

    ir_dep    deps[IR_DEPS];
    int       dep_count;
} ir_result;

//  api
void ir_init(ir_result *ir);
void ir_add_symbol(ir_result *ir, const char *name, const char *lang, const char *file, int line);
void ir_add_dependency(ir_result *ir, const char *from_file, const char *module, const char *type, const char *lang);

#endif