#include <iostream>
#include <immintrin.h>
#include <assert.h>
#include <random>
#include <chrono>

#define DIV sizeof(__m256i)

uint64_t addNonAVX(uint8_t* a, uint8_t* b, uint8_t* c, unsigned size) {

    auto ts = std::chrono::system_clock::now();

    for (unsigned i = 0; i < size; ++i) {
        c[i] = a[i] + b[i];
    }

    auto te = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(te - ts).count();
}

uint64_t addAVX(uint8_t* a, uint8_t* b, uint8_t* c, unsigned size) {

    assert(size % DIV == 0); // just for this showcase

    auto ts = std::chrono::system_clock::now();

    // TODO 





    auto te = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(te - ts).count();
}


int main()
{
    // create buffer
    unsigned size = DIV * 1000;
    uint8_t* a = new uint8_t[size];
    uint8_t* b = new uint8_t[size];
    uint8_t* cNon = new uint8_t[size];
    uint8_t* cAVX = new uint8_t[size];

    // init random gen
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0, 127);
    generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());

    // fill a & b
    for (unsigned i = 0; i < size; ++i) {
        a[i] = (uint8_t)distribution(generator);
        b[i] = (uint8_t)distribution(generator);
    }

    // reset c buffers
    memset(cNon, 0, size);
    memset(cAVX, 0, size);

    // add
    auto tNon = addNonAVX(a, b, cNon, size);
    auto tAVX = addAVX(a, b, cAVX, size);

    // check
    for (unsigned i = 0; i < size; ++i) {
        assert(cNon[i] == cAVX[i]); // result not the same
    }

    std::cout << "same result :)" << std::endl;
    std::cout << "nonAVX time: " << tNon << "us" << std::endl;
    std::cout << "AVX time: " << tAVX << "us" << std::endl;
    std::cout << "Speedup = : " << tNon / tAVX << std::endl;

    // remove buffer
    delete[] a;
    delete[] b;
    delete[] cNon;
    delete[] cAVX;
}
