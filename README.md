[![Build Status](https://travis-ci.org/Wheest/libsimdpp_mwe.svg?branch=master)](https://travis-ci.org/Wheest/libsimdpp_mwe)

# SIMD MWE: Vector Add

A minimum working example using the [libsimdpp](https://github.com/p12tic/libsimdpp) SIMD library, and the [CMake build system](https://cmake.org/).

Calculates the sum of two vectors.

This repository uses git submodules, thus to build the repo, and run the example:

```
git clone --recurse-submodules https://github.com/Wheest/libsimdpp_mwe
mkdir -p libsimdpp_mwe/_build
cd libsimdpp_mwe/_build
cmake ..
make
./simd_mwe [vector length] [number of repeats]
```

To see the assembly output of the code, and verify that there are SIMD instructions being used, run

```
# generate asm
objdump -drwCS -Mintel simd_mwe > a.asm

# print any SSE instructions used
awk '/[ \t](addps|addss|andnps|andps|cmpps|cmpss|comiss|cvtpi2ps|cvtps2pi|cvtsi2ss|cvtss2s|cvttps2pi|cvttss2si|divps|divss|ldmxcsr|maxps|maxss|minps|minss|movaps|movhlps|movhps|movlhps|movlps|movmskps|movntps|movss|movups|mulps|mulss|orps|rcpps|rcpss|rsqrtps|rsqrtss|shufps|sqrtps|sqrtss|stmxcsr|subps|subss|ucomiss|unpckhps|unpcklps|xorps|pavgb|pavgw|pextrw|pinsrw|pmaxsw|pmaxub|pminsw|pminub|pmovmskb|psadbw|pshufw)[ \t]/' a.asm
```
