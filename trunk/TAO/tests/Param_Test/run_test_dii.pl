eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

@ARGS=('run_test.pl', '-i', 'dii');
push @ARGS, @ARGV;

system ('perl ', join ' ', @ARGS);

