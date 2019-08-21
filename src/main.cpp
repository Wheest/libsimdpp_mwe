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


int main(int argc, char *argv[])
{
    std::cout << "awright, world?" << std::endl;

    const unsigned long SIZE = 4 * 160000;

    float vec_a[SIZE];
    float vec_b[SIZE];
    float result[SIZE];

    // standard version
    init_vector(vec_a, vec_b, SIZE);
    auto t1 = std::chrono::high_resolution_clock::now();

    for (auto i = 0; i < SIZE; i++) {
        result[i] = vec_a[i]  + vec_b[i];
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();

    test_result(result, SIZE);
    std::cout << "Standard: " << total << " ms\n";


    // SIMD version
    init_vector(vec_a, vec_b, SIZE);

    t1 = std::chrono::high_resolution_clock::now();

    for (int i=0; i<SIZE; i+=4) {
	simdpp::float32<4> xmmA = simdpp::load(vec_a + i);  //loads 4 floats into xmmA
	simdpp::float32<4> xmmB = simdpp::load(vec_b + i);  //loads 4 floats into xmmB
	simdpp::float32<4> xmmC = simdpp::add(xmmA, xmmB);  //Vector add of xmmA and xmmB
	simdpp::store(result + i, xmmC);            //Store result into the vector
    }

    t2 = std::chrono::high_resolution_clock::now();
    total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();

    test_result(result, SIZE);
    std::cout << "SIMD: " << total << " ms\n";

    return 0;
}
