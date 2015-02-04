cp ../../obj/simple-tool .
clang++ -emit-llvm -o dct.bc -c dct.cpp 
llvm-dis dct.bc
./simple-tool dct.bc  dct_o.bc
llvm-dis dct_o.bc

