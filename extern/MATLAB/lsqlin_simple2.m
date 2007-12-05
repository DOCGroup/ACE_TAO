function X=lsqlin_simple(C,d,A,b)
X = lsqlin(C,d,A,b);

%mcc -W cpplib:liblsq -T link:lib lsqlin_simple2.m
%mbuild lsqcall.cpp liblsq.lib