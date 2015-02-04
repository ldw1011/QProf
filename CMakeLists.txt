cmake_minimum_required(VERSION 2.8.8)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
SET(CMAKE_CXX_FLAGS  "-fPIC -std=c++11 -fno-rtti ")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_executable(simple-tool QProf.cpp main.cpp QTag.cpp QProfUtil.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs analysis arm armasmparser armasmprinter armcodegen armdesc armdisassembler arminfo asmparser asmprinter bitreader bitwriter codegen core cppbackend cppbackendcodegen cppbackendinfo debuginfo engine executionengine hexagon hexagonasmprinter hexagoncodegen hexagondesc hexagoninfo instcombine instrumentation interpreter ipa ipo irreader jit lineeditor linker lto mc mcanalysis mcdisassembler mcjit mcparser mips mipsasmparser mipsasmprinter mipscodegen mipsdesc mipsdisassembler mipsinfo msp430 msp430asmprinter msp430codegen msp430desc msp430info native nativecodegen nvptx nvptxasmprinter nvptxcodegen nvptxdesc nvptxinfo objcarcopts object option powerpc powerpcasmparser powerpcasmprinter powerpccodegen powerpcdesc powerpcdisassembler powerpcinfo profiledata r600 r600asmprinter r600codegen r600desc r600info runtimedyld scalaropts selectiondag sparc sparcasmparser sparcasmprinter sparccodegen sparcdesc sparcdisassembler sparcinfo support systemz systemzasmparser systemzasmprinter systemzcodegen systemzdesc systemzdisassembler systemzinfo tablegen target transformutils vectorize x86 x86asmparser x86asmprinter x86codegen x86desc x86disassembler x86info x86utils xcore xcoreasmprinter xcorecodegen xcoredesc xcoredisassembler xcoreinfo)

# Link against LLVM libraries
target_link_libraries(simple-tool ${llvm_libs})
