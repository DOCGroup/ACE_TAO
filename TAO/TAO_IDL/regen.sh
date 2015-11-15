if [[ tao_idl_fe.mpc -nt GNUmakefile.TAO_IDL_GEN ]]; then
    mpc.pl -type gnuace -features tao_idl_fe_gen=1 tao_idl_fe.mpc && \
    touch fe/keywords.dat
fi && \
make -f GNUmakefile.TAO_IDL_GEN tao_idl_fe_gen=1 lex.yy y.tab all && \
make
