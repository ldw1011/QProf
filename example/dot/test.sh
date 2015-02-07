cp ../../obj/simple-tool .
clang++ -emit-llvm -o dct.bc -c dct.cpp 
llvm-dis dct.bc
./simple-tool dct.bc  dct_o.bc
llvm-dis dct_o.bc
opt -correlated-propagation  -loop-instsimplify -loop-simplify -gvn -sccp -mem2reg -instcombine -debug-pass=Structure dct_o.bc > dct_f.bc
llvm-dis dct_f.bc

clang++ -o dot_new dct_o.bc test.cpp
./dot_new
./dot

