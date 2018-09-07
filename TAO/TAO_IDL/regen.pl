eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

use strict;
use Env 'ACE_ROOT';

my $features = 'tao_idl_fe_gen=1';
system "$ACE_ROOT/bin/mpc.pl -type gnuace -features $features tao_idl_fe.mpc";
utime undef, undef, 'fe/keywords.dat';
system "make -f GNUmakefile.TAO_IDL_GEN $features lex.yy y.tab all";
