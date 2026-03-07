#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    SEXP_INT,
    SEXP_SYM,
    SEXP_LIST
} SExpType;

typedef struct SExp SExp;

struct SExp {
    SExpType type;

    union {

        int i;

        char *s;

        struct {
            SExp **elements;
            int count;
        } list;

    } value;
};



// =======================
// ENV
// =======================

typedef struct {

    char name[32];
    int value;

} Var;

Var vars[32];
int var_count = 0;


void set_var(const char *name, int v) {

    for (int i = 0; i < var_count; i++) {

        if (strcmp(vars[i].name, name) == 0) {

            vars[i].value = v;
            return;
        }
    }

    strcpy(vars[var_count].name, name);
    vars[var_count].value = v;

    var_count++;
}


int get_var(const char *name) {

    for (int i = 0; i < var_count; i++) {

        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }

    return 0;
}



// =======================
// SEXP BUILDERS
// =======================

SExp* make_int(int i) {

    SExp* n = malloc(sizeof(SExp));

    n->type = SEXP_INT;
    n->value.i = i;

    return n;
}


SExp* make_sym(const char* s) {

    SExp* n = malloc(sizeof(SExp));

    n->type = SEXP_SYM;

    n->value.s = strdup(s);

    return n;
}



SExp* make_list(int count) {

    SExp* n = malloc(sizeof(SExp));

    n->type = SEXP_LIST;

    n->value.list.count = count;

    n->value.list.elements =
        malloc(sizeof(SExp*) * count);

    return n;
}



// =======================
// PRINT
// =======================

void print_exp(SExp* e) {

    if (!e) return;

    if (e->type == SEXP_INT)
        printf("%d", e->value.i);

    else if (e->type == SEXP_SYM)
        printf("%s", e->value.s);

    else {

        printf("(");

        for (int i = 0; i < e->value.list.count; i++) {

            print_exp(
                e->value.list.elements[i]
            );

            if (i + 1 < e->value.list.count)
                printf(" ");
        }

        printf(")");
    }
}



// =======================
// COMMANDS
// =======================

int cmd_add(SExp* list) {

    int sum = 0;

    for (int i = 1; i < list->value.list.count; i++) {

        SExp* e = list->value.list.elements[i];

        if (e->type == SEXP_INT)
            sum += e->value.i;
    }

    return sum;
}


int cmd_mul(SExp* list) {

    int r = 1;

    for (int i = 1; i < list->value.list.count; i++) {

        SExp* e = list->value.list.elements[i];

        if (e->type == SEXP_INT)
            r *= e->value.i;
    }

    return r;
}



// =======================
// EVAL
// =======================

SExp* eval(SExp* exp) {

    if (!exp) return NULL;

    if (exp->type == SEXP_INT)
        return exp;

    if (exp->type == SEXP_SYM)
        return exp;


    if (exp->type == SEXP_LIST) {

        if (exp->value.list.count == 0)
            return exp;


        SExp* head =
            exp->value.list.elements[0];


        if (head->type != SEXP_SYM)
            return exp;


        char *sym = head->value.s;


        // (+ ...)
        if (strcmp(sym, "+") == 0) {

            int r = cmd_add(exp);

            return make_int(r);
        }


        // (* ...)
        if (strcmp(sym, "*") == 0) {

            int r = cmd_mul(exp);

            return make_int(r);
        }


        // (defvar x 10)
        if (strcmp(sym, "defvar") == 0) {

            char *name =
                exp->value.list.elements[1]->value.s;

            int v =
                exp->value.list.elements[2]->value.i;

            set_var(name, v);

            return make_sym(name);
        }


        // (get x)
        if (strcmp(sym, "get") == 0) {

            char *name =
                exp->value.list.elements[1]->value.s;

            int v = get_var(name);

            return make_int(v);
        }


        // (help)
        if (strcmp(sym, "help") == 0) {

            printf(
                "Commands: + * defvar get help\n"
            );

            return NULL;
        }

    }

    return exp;
}



// =======================
// DEMO REPL
// =======================

int main() {

    printf("LISP CORE v0.3\n");



    // (+ 1 2 3)

    SExp* list = make_list(4);

    list->value.list.elements[0] = make_sym("+");
    list->value.list.elements[1] = make_int(1);
    list->value.list.elements[2] = make_int(2);
    list->value.list.elements[3] = make_int(3);

    SExp* r = eval(list);

    print_exp(r);
    printf("\n");


    // defvar

    SExp* dv = make_list(3);

    dv->value.list.elements[0] =
        make_sym("defvar");

    dv->value.list.elements[1] =
        make_sym("x");

    dv->value.list.elements[2] =
        make_int(10);

    eval(dv);


    // get

    SExp* g = make_list(2);

    g->value.list.elements[0] =
        make_sym("get");

    g->value.list.elements[1] =
        make_sym("x");

    print_exp(eval(g));

    printf("\n");



    return 0;
}