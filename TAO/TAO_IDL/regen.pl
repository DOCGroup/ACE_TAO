eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

use strict;
use Env 'ACE_ROOT';

my %args = map { $_ => 1 } @ARGV;
my $debug = exists($args{'-d'});

my $features = 'tao_idl_fe_gen=1';
system "$ACE_ROOT/bin/mpc.pl -type gnuace -features $features tao_idl_fe.mpc";
utime undef, undef, 'fe/keywords.dat';

my $bison_target = $debug ? 'debug_bison' : 'y.tab';
system "make -f GNUmakefile.TAO_IDL_GEN $features lex.yy $bison_target all";
