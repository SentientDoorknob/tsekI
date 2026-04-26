#ifndef TSEKM
#define TSEKM

#include <stdint.h>
#include <math.h>

void tsekM_mul(float* out, float* mat1, float* mat2, uint32_t dim);
void tsekM_transform(float* out, float* mat, float* vec, uint32_t dim);

void tsekM_dot(float* out, float* vec1, float* vec2, uint32_t dim);
void tsekM_cross(float* out, float* vec1, float* vec2, uint32_t dim);

void tsekM_scale(float* out, float* tensor, uint32_t scalar, uint32_t count);

void tsekM_display_matrix(float* matrix, uint32_t dim);
void tsekM_display_vector(float* vector, uint32_t dim);

#endif
