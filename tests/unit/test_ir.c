#include "unity.h"
#include "ir.h"

#include <string.h>
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {}

void test_init(void) {
    ir_result ir;
    ir.symbol_count = 10;
    ir.dep_count = 7;

    ir_init(&ir);

    TEST_ASSERT_EQUAL_INT(0, ir.symbol_count);
    TEST_ASSERT_EQUAL_INT(0, ir.dep_count);
}

void test_add_symbol(void) {
    ir_result ir;
    ir_init(&ir);

    ir_symbol *sym = ir_add_symbol(&ir, "sum", IR_SYMBOL_FUNCTION, "py", "examples/example.py", 1);

    TEST_ASSERT_NOT_NULL(sym);
    TEST_ASSERT_EQUAL_INT(1, ir.symbol_count);
    TEST_ASSERT_EQUAL_STRING("sum", sym->name);
    TEST_ASSERT_EQUAL_INT(IR_SYMBOL_FUNCTION, sym->kind);
    TEST_ASSERT_EQUAL_INT(1, sym->is_exported);
    TEST_ASSERT_EQUAL_STRING("py", sym->lang);
    TEST_ASSERT_EQUAL_STRING("examples/example.py", sym->file);
    TEST_ASSERT_EQUAL_INT(1, sym->line);
}

void test_symbol_overflow(void) {
    ir_result ir;
    ir_init(&ir);

    for (int i = 0; i < IR_MAX_SYMBOLS; ++i) {
        char name[32];
        snprintf(name, sizeof(name), "f%d", i);
        ir_symbol *sym = ir_add_symbol(&ir, name, IR_SYMBOL_FUNCTION, "py", "a.py", i + 1);
        TEST_ASSERT_NOT_NULL(sym);
    }

    TEST_ASSERT_EQUAL_INT(IR_MAX_SYMBOLS, ir.symbol_count);
    TEST_ASSERT_NULL(ir_add_symbol(&ir, "overflow", IR_SYMBOL_FUNCTION, "py", "a.py", 1));
}

void test_add_args(void) {
    ir_result ir;
    ir_init(&ir);

    ir_symbol *sym = ir_add_symbol(&ir, "sum", IR_SYMBOL_FUNCTION, "py", "examples/example.py", 1);
    TEST_ASSERT_NOT_NULL(sym);

    ir_symbol_add_args(sym, "a");
    ir_symbol_add_args(sym, "b");

    TEST_ASSERT_EQUAL_INT(2, sym->args_count);
    TEST_ASSERT_EQUAL_STRING("a", sym->args[0]);
    TEST_ASSERT_EQUAL_STRING("b", sym->args[1]);
}

void test_arg_limit(void) {
    ir_result ir;
    ir_init(&ir);

    ir_symbol *sym = ir_add_symbol(&ir, "many", IR_SYMBOL_FUNCTION, "js", "examples/example.js", 1);
    TEST_ASSERT_NOT_NULL(sym);

    for (int i = 0; i < IR_MAX_ARGS + 5; ++i) {
        ir_symbol_add_args(sym, "arg");
    }

    TEST_ASSERT_EQUAL_INT(IR_MAX_ARGS, sym->args_count);
}

void test_arg_truncation(void) {
    ir_result ir;
    ir_init(&ir);

    ir_symbol *sym = ir_add_symbol(&ir, "longarg", IR_SYMBOL_FUNCTION, "js", "a.js", 1);
    TEST_ASSERT_NOT_NULL(sym);

    char long_arg[IR_ARG_LEN * 2];
    memset(long_arg, 'x', sizeof(long_arg));
    long_arg[sizeof(long_arg) - 1] = '\0';

    ir_symbol_add_args(sym, long_arg);

    TEST_ASSERT_EQUAL_INT(1, sym->args_count);
    TEST_ASSERT_EQUAL_INT(IR_ARG_LEN - 1, (int)strlen(sym->args[0]));
}

void test_add_dep(void) {
    ir_result ir;
    ir_init(&ir);

    ir_add_dependency(&ir, "examples/example.js", "./example.py", "require", "js");

    TEST_ASSERT_EQUAL_INT(1, ir.dep_count);
    TEST_ASSERT_EQUAL_STRING("examples/example.js", ir.deps[0].from_file);
    TEST_ASSERT_EQUAL_STRING("./example.py", ir.deps[0].module);
    TEST_ASSERT_EQUAL_STRING("require", ir.deps[0].type);
    TEST_ASSERT_EQUAL_STRING("js", ir.deps[0].lang);
}

void test_dep_overflow(void) {
    ir_result ir;
    ir_init(&ir);

    for (int i = 0; i < IR_MAX_DEPS; ++i) {
        ir_add_dependency(&ir, "a.js", "./m.py", "require", "js");
    }

    TEST_ASSERT_EQUAL_INT(IR_MAX_DEPS, ir.dep_count);
    ir_add_dependency(&ir, "a.js", "./extra.py", "require", "js");
    TEST_ASSERT_EQUAL_INT(IR_MAX_DEPS, ir.dep_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_add_symbol);
    RUN_TEST(test_symbol_overflow);
    RUN_TEST(test_add_args);
    RUN_TEST(test_arg_limit);
    RUN_TEST(test_arg_truncation);
    RUN_TEST(test_add_dep);
    RUN_TEST(test_dep_overflow);
    return UNITY_END();
}
