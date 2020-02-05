// taking inspiration from https://pastebin.com/L14DCrky
//#define SIMDPP_ARCH_X86_SSE4_1 true
#include <simdpp/simd.h>
#include <assert.h>
#include <iostream>
#include <chrono>

// Initializes vector to store values
void init_vector(float* a, float* b, size_t size)
{
    for (int i=0; i<size; i++)
    {
        a[i] = i * 1.0;
        b[i] = (size * 1.0) - i - 1;
    }
}

// Test result of operations on vector
void test_result(float* result, size_t size)
{
    for (int i=0; i<size; i++)
        assert(result[i] == size - 1);
}

void non_simd(float* vec_a, float* vec_b, float* result, size_t SIZE,
              uint runs)
{
    for (int j = 0; j < runs; j++)
    {
#pragma loop(no_vector)
	for (auto i = 0; i < SIZE; i++)
	    result[i] = vec_a[i]  + vec_b[i];
    }
}

void simd_version(float* vec_a, float* vec_b, float* result, size_t SIZE,
                  uint runs)
{
    for (int j = 0; j < runs; j++)
    {
        for (int i=0; i<SIZE; i+=8) {
            simdpp::float32<8> xmmA = simdpp::load(vec_a + i);  //loads 8 floats into xmmA
            simdpp::float32<8> xmmB = simdpp::load(vec_b + i);  //loads 8 floats into xmmB
            simdpp::float32<8> xmmC = simdpp::add(xmmA, xmmB);  //Vector add of xmmA and xmmB
            simdpp::store(result + i, xmmC);            //Store result into the vector
        }
    }

}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "provide SIZE and runs" << std::endl;
        exit(1);
    }

    const size_t SIZE = std::stoi(argv[1]);
    const uint runs = std::stoi(argv[2]);

    float vec_a[SIZE];
    float vec_b[SIZE];
    float result1[SIZE];
    float result2[SIZE];

    // standard version
    init_vector(vec_a, vec_b, SIZE);
    auto t1 = std::chrono::high_resolution_clock::now();
    non_simd(vec_a, vec_b, result1, SIZE, runs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();

    test_result(result1, SIZE);
    std::cout << "Standard: " << total << " ms\n";


    // SIMD version
    init_vector(vec_a, vec_b, SIZE);
    auto t3 = std::chrono::high_resolution_clock::now();
    simd_version(vec_a, vec_b, result2, SIZE, runs);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto total2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count();
    test_result(result2, SIZE);
    std::cout << "SIMD: " << total2 << " ms\n";

    return 0;
}
