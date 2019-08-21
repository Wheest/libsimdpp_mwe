# SIMD MWE

A minimum working example using the [libsimdpp](https://github.com/p12tic/libsimdpp) SIMD library, and the [CMake build system](https://cmake.org/).

This repository uses git submodules, thus to build the repo, and run the example:

```
git clone --recurse-submodules https://github.com/Wheest/libsimdpp_mwe
mkdir -p libsimdpp_mwe/_build
cd libsimdpp_mwe/_build
cmake ..
make
./simd_mwe
```
