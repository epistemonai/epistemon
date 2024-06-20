#ifndef epistemon_tensor_h_
#define epistemon_tensor_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
        EPI_TENSOR_ONES,
        EPI_TENSOR_ZEROS,
} epiTensorCreate;

typedef struct {
        int ndim;
        int size;
        int shape[4];
        float *f32;
} epiTensor;

typedef struct {
        int shape[4];
        float value;
        float *set;
	epiTensorCreate create;
} epiTensorDesc;

/*
 General
*/
// Tensors must match exactly
void epi_tensor_strict_assert(epiTensor *a, epiTensor *b);
// Tensor A can be broadcasted into B (to be implemented)
void epi_tensor_assert(epiTensor *a, epiTensor *b);
epiTensor epi_tensor(epiTensorDesc tensor_desc);
void epi_tensor_destroy(epiTensor *tensor);
/*
 Element-wise functions
*/
void epi_tensor_apply(float (*function)(float), epiTensor *tensor);
void epi_tensor_add(epiTensor *a, epiTensor *b, epiTensor *result);
void epi_tensor_sub(epiTensor *a, epiTensor *b, epiTensor *result);
void epi_tensor_mul(epiTensor *a, epiTensor *b, epiTensor *result);
void epi_tensor_div(epiTensor *a, epiTensor *b, epiTensor *result);
void epi_tensor_addf(epiTensor *a, float b, epiTensor *result);
void epi_tensor_subf(epiTensor *a, float b, epiTensor *result);
void epi_tensor_mulf(epiTensor *a, float b, epiTensor *result);
void epi_tensor_divf(epiTensor *a, float b, epiTensor *result);
/*
 Linear Algebra
*/
void epi_tensor_transpose(epiTensor *a, epiTensor *result);
void epi_tensor_dot(epiTensor *a, epiTensor *b, epiTensor *result);
/*
 Misc
*/
void epi_tensor_print(epiTensor *tensor);

#endif
