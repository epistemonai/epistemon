#include "autodiff.h"

#define EXTEND(N) (N == 0 ? 8 : (N*2))
#define GET(V) g_tape->valbuf[(v)]
#define INIT_TAPE_SIZE 8
#define OPERATOR_FUNC_BINARY(OP_SYMBOL, OP_TYPE) \
        float data       = GET(a).data OP_SYMBOL GET(b).data; \
        size_t out       = epi_val(data); \
        GET(out).l_child = a; \
        GET(out).r_child = b; \
        GET(out).op      = OP_TYPE; \
        return out; \

static epiTape *g_tape = NULL;

const char *operators[] = {
        "add ", "sub ", "mul ", "pow ", "tanh", "relu", "sigmoid" "nil"
};

void epi_tape_init(epiTape *tape)
{
        tape->valbuf = calloc(INIT_TAPE_SIZE, sizeof(epiVal));
        tape->cap    = INIT_TAPE_SIZE;
        tape->count  = 1;
}

void epi_tape_set(epiTape *tape)
{
        g_tape = tape;
}

void epi_tape_release()
{
        g_tape = NULL;
}

void epi_tape_destroy(epiTape *tape)
{
        free(tape->valbuf);
}

void epi_tape_extend()
{
        if (g_tape->count >= g_tape->cap) {
                g_tape->cap    = EXTEND(g_tape->cap);
                g_tape->valbuf = realloc(g_tape->valbuf, sizeof(epiVal) * g_tape->cap);
                
                if (!g_tape->valbuf) {
                        fprintf(stderr, "Not enough memory!\n");
                        exit(1);
                }
        }
}

size_t epi_val(float value)
{
        epi_tape_extend();        // extends tape if needed
        epiVal *res  = &g_tape->valbuf[g_tape->count];
        res->data    = value;
        res->grad    = 0.0f;
        res->l_child = 0;
        res->r_child = 0;
        res->op      = COUNT;
        g_tape->count++;
        return g_tape->count-1;
}

size_t epi_add(size_t a, size_t b) { OPERATOR_FUNC_BINARY(+, EPI_ADD); }
size_t epi_sub(size_t a, size_t b) { OPERATOR_FUNC_BINARY(-, EPI_SUB); }
size_t epi_mul(size_t a, size_t b) { OPERATOR_FUNC_BINARY(*, EPI_MUL); }
size_t epi_pow(size_t a, size_t b)
{
        float data           = powf(GET(a).data, GET(b).data);
        size_t out           = epi_val(data);
        GET(out).left_child  = a;
        GET(out).right_child = b;
        GET(out).op          = EPI_POW;
        return out;
}

void _epi_reverse(size_t index)
{
        epiVal y = GET(index);

        switch (y.op)
        {
                case EPI_ADD: {
                        GET(y.l_child).grad += y.grad * 1.0f;
                        GET(y.r_child).grad += y.grad * 1.0f;
                } break;

                case EPI_SUB: {
                        GET(y.l_child).grad += y.grad *  1.0f;
                        GET(y.r_child).grad += y.grad * -1.0f;
                } break;

                case EPI_MUL: {
                        GET(y.l_child).grad += y.grad * GET(y.r_child).data;
                        GET(y.r_child).grad += y.grad * GET(y.l_child).data;
                } break;

                case EPI_POW: {
                        float l_data = GET(y.l_child).data;
                        float r_data = GET(y.r_child).data;
                        GET(y.l_child).grad += y.grad * r_data * powf(l_data, r_data - 1);
                        GET(y.r_child).grad += y.grad * log(l_data) * powf(l_data, r_data);
                } break;

                default: break;
        }
}

void epi_reverse(size_t index)
{
        GET(index).grad = 1.0;
        for (size_t i = g_tape->count-1; i >= 1; --i)
                _epi_reverse(i);
}

void _epi_tree_print(size_t index, size_t indent){
        if (index == 0) return;
        epiVal y = GET(index);
        for (size_t i = 0; i < indent; ++i) printf(" ");
        printf("[idx: %d, %s] node (data: %g, grad: %g)\n", index, operators[y.op], y.data, y.grad);
        _epi_tree_print(y.l_child, indent+4);
        _epi_tree_print(y.r_child, indent+4);
}

void epi_print_tree(size_t index){
        printf("------------- Computation graph -------------\n");
        _epi_tree_print(index, 0);
        printf("--------------------------------------------\n");
}

void epi_tape_print(){
    for (size_t i = 0; i < g_tape->count; ++i){
        printf("val: %2g, index: %3zu, left: %3zu, right: %3zu, op: %s\n", 
            g_tape->valbuf[i].data, i, g_tape->valbuf[i].l_child, g_tape->valbuf[i].r_child, operators[g_tape->valbuf[i].op]);
    }
}