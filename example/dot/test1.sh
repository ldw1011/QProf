opt -S  -loops -mem2reg  -loop-rotate  -lcssa -loop-unroll -unroll-threshold=10000000 -simplifycfg -mem2reg -constprop -sccp -debug-pass=Structure dct.ll > dct_o.ll
