eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

$brace="\#\#\#\#\#";
%tests = ("basic_types" => "-n 256 -l 10",
          "tc" => "",
          "growth" => "-l 64 -h 256 -s 4 -n 10 -q",
          "alignment" => "",
          "allocator" => "-q");
$test = "";
$args = "";
$status = 0;
print STDERR "\n";

while (($test,$args) = each %tests) {
    print STDERR "\n$brace $test $args test BEGUN\n";
    $TST = $target->CreateProcess ($test, $args);
    print STDERR "\tRUNNING $test\n";
    
    $retval = $TST->SpawnWaitKill (90);

    if ($retval != 0) { 
        print STDERR "ERROR $brace $test $args, exit value $retval\n";
        $status = 1;
    }
    print STDERR "$brace $test FINISHED successfully\n";
}

$target->GetStderrLog();

exit $status;
