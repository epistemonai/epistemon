#include "tensor.h"

#define _TENSOR_SET(TENSOR, VALUE) \
do {\
	for (int _i = 0; _i < TENSOR.size; _i++) TENSOR.f32[_i] = VALUE;\
} while (0)

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
        for (; i < 4; i++) {
                tensor.size      *= tensor_desc.shape[i];  
                tensor.shape[3-i] = tensor_desc.shape[i];
        }
	tensor.ndim = i;

        tensor.f32 = (float*)malloc(tensor.size * sizeof(float));

	switch (tensor_desc.create) {
		case EPI_TENSOR_ZEROS: _TENSOR_SET(tensor, 0.0); break;
		case EPI_TENSOR_ONES:  _TENSOR_SET(tensor, 1.0); break;
	};
        
	if (tensor_desc.value != 0.0) for (int j = 0; j < tensor.size; j++) tensor.f32[j] = tensor_desc.value;
        if (tensor_desc.set != NULL)  for (int j = 0; j < tensor.size; j++) tensor.f32[j] = tensor_desc.set[j];

        return tensor;
}

void epi_tensor_destroy(epiTensor *tensor)
{
        free(tensor->f32);
}

/*
 Element-wise functions
*/
void epi_tensor_apply(float (*function)(float), epiTensor *tensor)
{
        for (int i=0;i<tensor->size;i++) tensor->f32[i]=function(tensor->f32[i]);
}

void epi_tensor_add(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a,b); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]+b->f32[i];
}

void epi_tensor_sub(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a,b); for (int i=0; i<a->size;i++) result->f32[i]=a->f32[i]-b->f32[i];
}

void epi_tensor_mul(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a,b); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]*b->f32[i];
}

void epi_tensor_div(epiTensor *a, epiTensor *b, epiTensor *result)
{
        epi_tensor_strict_assert(a,b); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]/b->f32[i];
}

void epi_tensor_addf(epiTensor *a, float b, epiTensor *result)
{
        epi_tensor_strict_assert(a,result); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]+b;
}

void epi_tensor_subf(epiTensor *a, float b, epiTensor *result)
{
        epi_tensor_strict_assert(a,result); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]-b;
}

void epi_tensor_mulf(epiTensor *a, float b, epiTensor *result)
{
        epi_tensor_strict_assert(a,result); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]*b;
}

void epi_tensor_divf(epiTensor *a, float b, epiTensor *result)
{
        epi_tensor_strict_assert(a,result); for (int i=0;i<a->size;i++) result->f32[i]=a->f32[i]/b;
}

/*
 Linear Algebra
*/
void epi_tensor_transpose(epiTensor *a, epiTensor *result)
{
        result->shape[0] = a->shape[1];
        result->shape[1] = a->shape[0];
}

void epi_tensor_dot(epiTensor *a, epiTensor *b, epiTensor *result)
{

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
                if (dim+1 >= tensor->ndim) printf("%.2f,", tensor->f32[_g_index++]);
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