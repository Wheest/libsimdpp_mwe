// taking inspiration from https://pastebin.com/L14DCrky
//#define SIMDPP_ARCH_X86_SSE4_1 true
#include <simdpp/simd.h>
#include <assert.h>
#include <iostream>
#include <chrono>
#include <immintrin.h>

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
	    result[i] = vec_a[i] + vec_b[i];
    }
}

// void simd_version_2(float* vec_a, float* vec_b, float* result, size_t SIZE,
//                   uint runs)
// {
//     for (int j = 0; j < runs; j++)
//     {
//         for (int i=0; i<SIZE; i+=2) {
//             simdpp::float32<2> xmmA = simdpp::load(vec_a + i);  //loads 8 floats into xmmA
//             simdpp::float32<2> xmmB = simdpp::load(vec_b + i);  //loads 8 floats into xmmB
//             simdpp::float32<2> xmmC = simdpp::add(xmmA, xmmB);  //Vector add of xmmA and xmmB
//             simdpp::store(result + i, xmmC);            //Store result into the vector
//         }
//     }
// }

void simd_version_4(float* vec_a, float* vec_b, float* result, size_t SIZE,
                    uint runs)
{
    for (int j = 0; j < runs; j++)
    {
        for (int i=0; i<SIZE; i+=4) {
            simdpp::float32<4> xmmA = simdpp::load(vec_a + i);  //loads 8 floats into xmmA
            simdpp::float32<4> xmmB = simdpp::load(vec_b + i);  //loads 8 floats into xmmB
            simdpp::float32<4> xmmC = simdpp::add(xmmA, xmmB);  //Vector add of xmmA and xmmB
            simdpp::store(result + i, xmmC);            //Store result into the vector
        }
    }
}

void simd_version_8(float* vec_a, float* vec_b, float* result, size_t SIZE,
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

void simd_intrin_version(float* vec_a, float* vec_b, float* result,
                         size_t SIZE, uint runs)
{
    for (int j = 0; j < runs; j++)
    {
        for (int i=0; i<SIZE; i+=4) {
            __m128 xmmA = _mm_load_ps(vec_a + i);
            __m128 xmmB = _mm_load_ps(vec_b + i);
            __m128 xmmC = _mm_add_ps(xmmA, xmmB);
            _mm_store_ps(result + i, xmmC);

        }
    }
}

void simd_intrin_version_16(float* vec_a, float* vec_b, float* result,
                            size_t SIZE, uint runs)
{
    for (int j = 0; j < runs; j++)
    {
        for (int i=0; i<SIZE; i+=4) {
            __m512 xmmA = _mm512_load_ps(vec_a + i);
            __m512 xmmB = _mm512_load_ps(vec_b + i);
            __m512 xmmC = _mm512_add_ps(xmmA, xmmB);
            _mm512_store_ps(result + i, xmmC);
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
    float result[SIZE];

    // standard version
    init_vector(vec_a, vec_b, SIZE);
    auto t1 = std::chrono::high_resolution_clock::now();

    non_simd(vec_a, vec_b, result, SIZE, runs);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    test_result(result, SIZE);
    std::cout << "Standard: " << total << " ms\n";


    // SIMD version
    // init_vector(vec_a, vec_b, SIZE);
    // t1 = std::chrono::high_resolution_clock::now();

    // simd_version_2(vec_a, vec_b, result, SIZE, runs);

    // t2 = std::chrono::high_resolution_clock::now();
    // total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    // test_result(result, SIZE);
    // std::cout << "SIMD (2): " << total << " ms\n";

    // SIMD version
    init_vector(vec_a, vec_b, SIZE);
    t1 = std::chrono::high_resolution_clock::now();
    simd_version_4(vec_a, vec_b, result, SIZE, runs);
    t2 = std::chrono::high_resolution_clock::now();
    total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    test_result(result, SIZE);
    std::cout << "libsimdpp (4): " << total << " ms\n";

    // SIMD version
    init_vector(vec_a, vec_b, SIZE);
    t1 = std::chrono::high_resolution_clock::now();
    simd_version_8(vec_a, vec_b, result, SIZE, runs);
    t2 = std::chrono::high_resolution_clock::now();
    total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    test_result(result, SIZE);
    std::cout << "libsimdpp (8): " << total << " ms\n";


    // SIMD Intrinsics version
    init_vector(vec_a, vec_b, SIZE);
    t1 = std::chrono::high_resolution_clock::now();
    simd_intrin_version(vec_a, vec_b, result, SIZE, runs);
    t2 = std::chrono::high_resolution_clock::now();
    total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    test_result(result, SIZE);
    std::cout << "SSE Intrinsics (4): " << total << " ms\n";

    // SIMD Intrinsics version
    init_vector(vec_a, vec_b, SIZE);
    t1 = std::chrono::high_resolution_clock::now();
    simd_intrin_version_16(vec_a, vec_b, result, SIZE, runs);
    t2 = std::chrono::high_resolution_clock::now();
    total = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    test_result(result, SIZE);
    std::cout << "SSE Intrinsics (16): " << total << " ms\n";

    return 0;
}
