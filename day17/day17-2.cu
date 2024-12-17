#include <iostream>
#include <fstream>

//  nvcc -o gpu day17-2.cu && ./gpu

__managed__ long long start_A = 190300000000000;
__managed__ long long end_A = 19040000000000; // Too high, around 8^15 * 5 + 15000000000000, which is when the last digits start to look good, but down a couple tens of billions


#define LENGTH 16

__managed__ long long default_B = 0, default_C = 0;
__managed__ int program[LENGTH] = {2, 4, 1, 2, 7, 5, 0, 3, 4, 7, 1, 7, 5, 5, 3, 0};

__device__ long long get_combo(long long num, long long A, long long B, long long C)
{
    if (num <= 3)
        return num;
    if (num == 4)
        return A;
    if (num == 5)
        return B;
    if (num == 6)
        return C;
    return -1;
}

__device__ bool areArraysEqual(int arr1[], int arr2[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (arr1[i] != arr2[i])
        {
            return false;
        }
    }
    return true;
}

__global__ void testValidity()
{
    long long index = (long long)(blockIdx.y * gridDim.x + blockIdx.x) * (long long)(blockDim.x * blockDim.y) + (long long)(threadIdx.y * blockDim.x + threadIdx.x);
    long long A, B, C;
    long long i = start_A + index;
    if (i % 1000000000 == 0)
        printf("Reached: %lld, %d, %d, %d, %d, %d, %d, %lld\n", i, threadIdx.x, threadIdx.y, blockIdx.x, blockIdx.y, blockDim.x, blockDim.y, index);
    A = i;
    B = default_B;
    C = default_C;

    int out[LENGTH];
    int out_i{0};
    int ip{0};
    while (ip < LENGTH)
    {
        int op = program[ip];
        int in = program[ip + 1];
        switch (op)
        {
        case 0:
            A = A >> get_combo(in, A, B, C);
            break;
        case 1:
            B = B ^ in;
            break;
        case 2:
            B = get_combo(in, A, B, C) % 8;
            break;
        case 3:
            if (A != 0)
            {
                ip = in;
                continue;
            }
            break;
        case 4:
            B = B ^ C;
            break;
        case 5:
            out[out_i++] = get_combo(in, A, B, C) % 8;
            break;
        case 6:
            B = A >> get_combo(in, A, B, C);
            break;
        case 7:
            C = A >> get_combo(in, A, B, C);
            break;
        default:
            break;
        }

        ip += 2;
    }

    if (areArraysEqual(out, program, LENGTH))
    {
        printf("Solution: %lld\n", i);
    }
}

int main()
{

    dim3 dimBlock(32, 32);
    dim3 dimGrid(10000, 10000);

    testValidity<<<dimGrid, dimBlock>>>();
    cudaDeviceSynchronize();
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        std::cerr << "CUDA Kernel Launch Error: " << cudaGetErrorString(err) << std::endl;
        exit(-1);
    }

    return 0;
}
