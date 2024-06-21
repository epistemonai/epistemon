#ifndef epistemon_autodiff_h_
#define epistemon_autodiff_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

typedef enum {
        EPI_ADD,
        EPI_SUB, 
        EPI_MUL,
        EPI_POW,
        EPI_TANH,
        EPI_RELU,
        EPI_SIGMOID,
        EPI_COUNT
} epiOpType;

typedef struct {
        float data;
        float grad;
        epiOpType op;
        size_t l_child;
        size_r r_child;
} epiValue;

typedef struct {
        epiValue *valbuf;
        size_t count;
        size_t cap;
} epiTape;

// tape functions
void epi_tape_init(epiTape *tape);
void epi_tape_destroy(epiTape *tape);
void epi_tape_set(epiTape *tape);
void epi_tape_release();
void epi_tape_print();

// basic operations
size_t epi_val(float value);
size_t epi_add(size_t a, size_t b);
size_t epi_sub(size_t a, size_t b);
size_t epi_mul(size_t a, size_t b);
size_t epi_pow(size_t a, size_t b);

// activation functions (to be implemented)

// compute gradients of value in reverse mode
void epi_reverse(size_t y);

void epi_tree_print(size_t y);

#endif