#include <stdio.h>
#include <cuda_runtime.h>

// CUDA kernel
__global__ void vectorAdd(float *a, float *b, float *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

// Macro to check for CUDA errors
#define CHECK_CUDA_ERROR(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            fprintf(stderr, "CUDA error in %s at line %d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
            return 1; \
        } \
    } while(0)

int main() {
    int n = 1 << 20;
    size_t size = n * sizeof(float);

    // Host memory allocation
    float *h_a = (float *)malloc(size);
    float *h_b = (float *)malloc(size);
    float *h_c = (float *)malloc(size);

    // Initialize input vectors
    for (int i = 0; i < n; i++) {
        h_a[i] = float(i);
        h_b[i] = float(i * 2);
    }

    // Device memory allocation
    float *d_a, *d_b, *d_c;
    CHECK_CUDA_ERROR(cudaMalloc((void **)&d_a, size));
    CHECK_CUDA_ERROR(cudaMalloc((void **)&d_b, size));
    CHECK_CUDA_ERROR(cudaMalloc((void **)&d_c, size));

    // Copy data to device
    CHECK_CUDA_ERROR(cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice));
    CHECK_CUDA_ERROR(cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice));

    // Launch kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, n);

    // Check for any kernel launch errors
    CHECK_CUDA_ERROR(cudaGetLastError());

    // Copy result back to host
    CHECK_CUDA_ERROR(cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost));

    // Print some results to verify
    for (int i = 0; i < 9; i++) {
        printf("c[%d] = %f\n", i, h_c[i]);
    }

    // Free memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(h_a);
    free(h_b);
    free(h_c);

    return 0;
}
