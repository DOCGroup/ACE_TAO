# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../bin";
require ACEutils;

$brace="\#\#\#\#\#";
$cdr_iter = 100;
@tests = ("basic_types", "tc", "growth");
@argslist = ("-n 256 -l 10", "", "-l 64 -h 256 -s 4 -n 100");
$test = "";
$args = "";
$| = 1;
print STDERR "\n";

sub run_test 
{
    #my $test = shift (@tests);
    my $args = shift (@argslist);
    
    print STDERR "\n$brace $test $args test BEGUN\n";
    $TST = Process::Create  ($EXEPREFIX."$test".$Process::EXE_EXT,
			     "$args");
    print STDERR "\t$test RUNNING\n";
    $retval = ($TST->Wait ()) / 256; 
    if (retval != 0)
    {
	print STDERR "$brace $test $args FAILED; exited with value $retval\n";
    }
    print STDERR "$brace $test ENDED successfully\n";
}

foreach $test (@tests)
{
    run_test ($test, $args);
}


