#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h> // For mathematical functions

// Structure to represent a NeuArray
typedef struct {
    int* val;   // Pointer to the array data
    int dim;    // Dimension of the array
    int* shape; // Shape of the array (size along each dimension)
    int size;   // Total number of elements in the array
} NeuArray;

// Create a NeuArray based on the provided shape and dimension
NeuArray* create_NeuArray(const int* shape, int dim) {
    NeuArray* n1 = (NeuArray*)malloc(sizeof(NeuArray));
    if (n1 == NULL) {
        fprintf(stderr, "DMA Failed | NeuArray\n");
        return NULL;
    }
    n1->shape = (int*)malloc(dim * sizeof(int));
    if (n1->shape == NULL) {
        fprintf(stderr, "DMA Failed | NeuArray Shape\n");
        free(n1);
        return NULL;
    }
    n1->dim = dim;
    n1->size = 1;
    for (int i = 0; i < dim; i++) {
        n1->shape[i] = shape[i];
        n1->size *= shape[i];
    }
    n1->val = (int*)malloc(n1->size * sizeof(int));
    if (n1->val == NULL) {
        fprintf(stderr, "DMA Failed | NeuArray Values\n");
        free(n1->shape);
        free(n1);
        return NULL;
    }
    return n1;
}

// Calculate the total number of elements based on shape and dimension
int len(const int* shape, int dim) {
    int s = 1;
    for (int i = 0; i < dim; i++) {
        s *= shape[i];
    }
    return s;
}

// Print a NeuArray's shape, size, and elements
void print_NeuArray(const NeuArray* n1) {
    if (n1 == NULL) {
        printf("Empty Array\n");
        return;
    }
    printf("NeuArray |  Shape : (");
    for (int i = 0; i < n1->dim; i++) {
        printf("%d", n1->shape[i]);
        if (i < n1->dim - 1) {
            printf(", ");
        }
    }
    printf(") |  Size : %d\n\n", n1->size);
    for (int i = 0; i < n1->size; i++) {
        printf("%d", n1->val[i]);
        if ((i + 1) % n1->shape[n1->dim - 1] == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
}

// Free the memory occupied by a NeuArray
void free_NeuArray(NeuArray* n1) {
    if (n1 != NULL) {
        free(n1->shape);
        free(n1->val);
        free(n1);
    }
}

// Create a NeuArray initialized with zeros
NeuArray* zeros(const int* shape, int dim) {
    NeuArray* n1 = create_NeuArray(shape, dim);
    if (n1 == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        n1->val[i] = 0;
    }
    return n1;
}

// Create a NeuArray initialized with ones
NeuArray* ones(const int* shape, int dim) {
    NeuArray* n1 = create_NeuArray(shape, dim);
    if (n1 == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        n1->val[i] = 1;
    }
    return n1;
}

// Create an identity matrix (diagonal elements set to 1)
NeuArray* eye(const int* shape, int dim) {
    if (dim != 2 || shape[0] != shape[1]) {
        fprintf(stderr, "Error: Eye function only supports square matrices.\n");
        return NULL;
    }
    NeuArray* n1 = create_NeuArray(shape, dim);
    if (n1 == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        n1->val[i] = 0;
    }
    for (int i = 0; i < shape[0]; i++) {
        n1->val[i * shape[0] + i] = 1; // Set diagonal elements to 1
    }
    return n1;
}

// Check if two NeuArrays have the same shape
int check_shape(const NeuArray* n1, const NeuArray* n2) {
    if (n1->dim != n2->dim || n1->size != n2->size) {
        return 0;
    }
    for (int i = 0; i < n1->dim; i++) {
        if (n1->shape[i] != n2->shape[i]) {
            return 0;
        }
    }
    return 1;
}

// Add two NeuArrays element-wise
NeuArray* add(const NeuArray* n1, const NeuArray* n2) {
    if (!check_shape(n1, n2)) {
        fprintf(stderr, "Error: Shape Mismatch\n");
        return NULL;
    }
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = n1->val[i] + n2->val[i];
    }
    return res;
}

// Subtract two NeuArrays element-wise
NeuArray* subtract(const NeuArray* n1, const NeuArray* n2) {
    if (!check_shape(n1, n2)) {
        fprintf(stderr, "Error: Shape Mismatch\n");
        return NULL;
    }
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = n1->val[i] - n2->val[i];
    }
    return res;
}

// Multiply two NeuArrays element-wise
NeuArray* multiply(const NeuArray* n1, const NeuArray* n2) {
    if (!check_shape(n1, n2)) {
        fprintf(stderr, "Error: Shape Mismatch\n");
        return NULL;
    }
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = n1->val[i] * n2->val[i];
    }
    return res;
}

// Divide two NeuArrays element-wise
NeuArray* divide(const NeuArray* n1, const NeuArray* n2) {
    if (!check_shape(n1, n2)) {
        fprintf(stderr, "Error: Shape Mismatch\n");
        return NULL;
    }
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        if (n2->val[i] == 0) {
            fprintf(stderr, "Error: Can't divide by 0\n");
            free_NeuArray(res);
            return NULL;
        }
        res->val[i] = n1->val[i] / n2->val[i];
    }
    return res;
}

// Negate the elements of a NeuArray (in-place)
NeuArray* neg(NeuArray* n1) {
    for (int i = 0; i < n1->size; i++) {
        n1->val[i] = -n1->val[i];
    }
    return n1;
}

// Create a copy of a NeuArray
NeuArray* copy_NeuArray(const NeuArray* n1) {
    NeuArray* cpy = create_NeuArray(n1->shape, n1->dim);
    if (cpy == NULL) return NULL;
    memcpy(cpy->val, n1->val, n1->size * sizeof(int));
    return cpy;
}

// Compare two NeuArrays for equality
int compare_NeuArray(const NeuArray* n1, const NeuArray* n2) {
    if (n1->dim != n2->dim || n1->size != n2->size) {
        return 0;
    }
    for (int i = 0; i < n1->dim; i++) {
        if (n1->shape[i] != n2->shape[i]) {
            return 0;
        }
    }
    for (int i = 0; i < n1->size; i++) {
        if (n1->val[i] != n2->val[i]) {
            return 0;
        }
    }
    return 1;
}

// Standard matrix multiplication (for general cases)
NeuArray* matrix_multiply(const NeuArray* A, const NeuArray* B) {
    if (A->dim < 2 || B->dim < 2 || A->shape[A->dim - 1] != B->shape[B->dim - 2]) {
        fprintf(stderr, "Error: Dimensions are wrong for matrix multiplication\n");
        return NULL;
    }

    int res_shape[2] = {A->shape[0], B->shape[1]};
    NeuArray* res = create_NeuArray(res_shape, 2);
    if (res == NULL) return NULL;

    for (int i = 0; i < A->shape[0]; i++) {
        for (int j = 0; j < B->shape[1]; j++) {
            res->val[i * res_shape[1] + j] = 0; // Initialize to 0
            for (int k = 0; k < A->shape[1]; k++) {
                res->val[i * res_shape[1] + j] += A->val[i * A->shape[1] + k] * B->val[k * B->shape[1] + j];
            }
        }
    }
    return res;
}

// Generates a NeuArray with random integer values between low and high
NeuArray* randint(int low, int high, const int* shape, int dim) {
    NeuArray* res = create_NeuArray(shape, dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < res->size; i++) {
        res->val[i] = low + rand() % (high - low);
    }
    return res;
}

// Reshapes a given NeuArray to the new specified shape and dimension
NeuArray* reshape(const NeuArray* n1, const int* new_shape, int new_dim) {
    int new_size = len(new_shape, new_dim);
    if (new_size != n1->size) {
        fprintf(stderr, "Error: New shape must have the same number of elements.\n");
        return NULL;
    }
    NeuArray* res = create_NeuArray(new_shape, new_dim);
    if (res == NULL) return NULL;
    memcpy(res->val, n1->val, n1->size * sizeof(int));
    return res;
}

// Transposes a 2D NeuArray (swaps rows and columns)
NeuArray* transpose(const NeuArray* n1) {
    if (n1->dim != 2) {
        fprintf(stderr, "Error: Transpose is only implemented for 2D array.\n");
        return NULL;
    }
    int new_shape[2] = {n1->shape[1], n1->shape[0]};
    NeuArray* res = create_NeuArray(new_shape, 2);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->shape[0]; i++) {
        for (int j = 0; j < n1->shape[1]; j++) {
            res->val[j * new_shape[0] + i] = n1->val[i * n1->shape[1] + j];
        }
    }
    return res;
}

// Applies the square root function to each element in the NeuArray
NeuArray* sqrt_NeuArray(const NeuArray* n1) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)sqrt(n1->val[i]);
    }
    return res;
}

// Computes the mean (average) of the elements in the NeuArray
double mean(const NeuArray* n1) {
    double sum = 0;
    for (int i = 0; i < n1->size; i++) {
        sum += n1->val[i];
    }
    return sum / n1->size;
}

// Finds the maximum value in the NeuArray
int max(const NeuArray* n1) {
    int max_val = n1->val[0];
    for (int i = 1; i < n1->size; i++) {
        if (n1->val[i] > max_val) max_val = n1->val[i];
    }
    return max_val;
}

// Finds the minimum value in the NeuArray
int min(const NeuArray* n1) {
    int min_val = n1->val[0];
    for (int i = 1; i < n1->size; i++) {
        if (n1->val[i] < min_val) min_val = n1->val[i];
    }
    return min_val;
}

// Applies the exponential function to each element in the NeuArray
NeuArray* exp_NeuArray(const NeuArray* n1) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)exp(n1->val[i]);
    }
    return res;
}

// Raises each element in the NeuArray to a given power
NeuArray* power(const NeuArray* n1, double expn) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)pow(n1->val[i], expn);
    }
    return res;
}

// Applies the sine function to each element in the NeuArray
NeuArray* sin_NeuArray(const NeuArray* n1) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)sin(n1->val[i]);
    }
    return res;
}

// Applies the cosine function to each element in the NeuArray
NeuArray* cos_NeuArray(const NeuArray* n1) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)cos(n1->val[i]);
    }
    return res;
}

// Applies the tangent function to each element in the NeuArray
NeuArray* tan_NeuArray(const NeuArray* n1) {
    NeuArray* res = create_NeuArray(n1->shape, n1->dim);
    if (res == NULL) return NULL;
    for (int i = 0; i < n1->size; i++) {
        res->val[i] = (int)tan(n1->val[i]);
    }
    return res;
}