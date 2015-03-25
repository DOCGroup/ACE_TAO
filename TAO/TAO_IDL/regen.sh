mpc.pl -type gnuace -features tao_idl_fe_gen=1 tao_idl_fe.mpc && \
touch fe/keywords.dat && \
make -f GNUmakefile.TAO_IDL_GEN tao_idl_fe_gen=1 lex.yy y.tab
