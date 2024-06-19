#ifndef epistemon_tensor_h_
#define epistemon_tensor_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _TENSOR_SET(TENSOR, VALUE) \
do {\
	for (int _i = 0; _i < TENSOR.size; _i++) TENSOR.f32data[_i] = VALUE;\
} while (0)

typedef enum {
        EPI_TENSOR_ONES,
        EPI_TENSOR_ZEROS,
} epiTensorCreate;

typedef struct {
        int ndim;
        int size;
        int shape[4];
        float *f32data;
} epiTensor;

typedef struct {
        int shape[4];
        float value;
        float *set;
	epiTensorCreate create;
} epiTensorDesc;

// Tensors must match exactly
void epi_tensor_strict_assert(epiTensor *a, epiTensor *b)
{
        if (a->ndim != b->ndim) {
                printf("Tensor has %d dimensions and the other has %d\n", a->ndim, b->ndim);
                exit(0);  
        } 

        for (int i = 0; i < a->ndim; i++) {
                if (a->shape[i] != b->shape[i]) {
                        printf("Tensor shapes don't match\n");
                        exit(0);
                }
        }
}

// Tensor A can be broadcasted into B (to be implemented)
void epi_tensor_assert(epiTensor *a, epiTensor *b)
{
	
}

epiTensor epi_tensor(epiTensorDesc tensor_desc)
{
        epiTensor tensor;
        tensor.size = 1;

	int i = 0;
        for (; tensor_desc.shape[i] != 0; i++) {
                tensor.size    *= tensor_desc.shape[i];  
                tensor.shape[i] = tensor_desc.shape[i];
        }
	tensor.ndim = i;

        tensor.f32data = (float*)malloc(tensor.size * sizeof(float));

	switch (tensor_desc.create) {
		case EPI_TENSOR_ZEROS: _TENSOR_SET(tensor, 0.0); break;
		case EPI_TENSOR_ONES:  _TENSOR_SET(tensor, 1.0); break;
	};
        
	if (tensor_desc.value != 0.0) for (int j = 0; j < tensor.size; j++) tensor.f32data[j] = tensor_desc.value;
        if (tensor_desc.set != NULL)  for (int j = 0; j < tensor.size; j++) tensor.f32data[j] = tensor_desc.set[j];

        return tensor;
}

void epi_tensor_apply(float (*function)(float), epiTensor *tensor)
{
        for (int i = 0; i < tensor->size; i++) tensor->f32data[i] = function(tensor->f32data[i]);
}

void epi_tensor_add(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a, b);
        for (int i = 0; i < a->size; i++) result->f32data[i] = a->f32data[i] + b->f32data[i%b->size];
}

void epi_tensor_sub(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a, b);
        for (int i = 0; i < a->size; i++) result->f32data[i] = a->f32data[i] - b->f32data[i];
}

void epi_tensor_mul(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a, b);
        for (int i = 0; i < a->size; i++) result->f32data[i] = a->f32data[i] * b->f32data[i];
}

void epi_tensor_div(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a, b);
        for (int i = 0; i < a->size; i++) result->f32data[i] = a->f32data[i] / b->f32data[i];
}

void epi_tensor_destroy(epiTensor *tensor)
{
        free(tensor->f32data);
}

static int _g_index = 0;
void _epi_tensor_print(epiTensor *tensor, int dim, int index)
{
        if (index == 0) { 
                printf("[");
        } else {
                for (int i = 0; i < dim; i++) printf(" ");
                printf("[");
        }           

        for (int i = 0; i < tensor->shape[dim]; i++)
                if (dim+1 >= tensor->ndim) printf("%.2f,", tensor->f32data[_g_index++]);
                else                       _epi_tensor_print(tensor, dim+1, i);

        if      ((dim-1 >= 0) && (index+1 == tensor->shape[dim-1])) printf("]");
        else if (dim == 1 && tensor->ndim > 2) printf("]\n\n");
        else                                   printf("]\n");
}

void epi_tensor_print(epiTensor *tensor)
{
        _epi_tensor_print(tensor, 0, 0);
        _g_index = 0;
}

#endif
