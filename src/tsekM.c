#include <math.h>
#include <stdint.h>
#include <stdio.h>

void tsekM_mul(float* out, float* mat1, float* mat2, uint32_t dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      float sum = 0;
      for (int k = 0; k < dim; k++) {
        sum += mat2[k * dim + i] * mat1[j * dim + k];
      }
      out[j * dim + i] = sum;
    }
  }
}

void tsekM_display_matrix(float* matrix, uint32_t dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      printf("%-6.2f ", matrix[i * dim + j]);
    }
    printf("\n");
  }
}
