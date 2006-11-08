eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Do not use environment variables here since not all platforms use ACE_ROOT
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use English;

$status = 0;
$exflags = "";

# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-d]\n";
        print "\n";
        print "-d                  -- set the debug flag\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-d") {
        $exflags .= " -d ";
    }
    else {
        print STDERR "ERROR: Unknown Option: ".$ARGV[$i]."\n";
    }
}

# Run the test

$EX = new PerlACE::Process ("EventAny");
$EX->Arguments ($exflags);
$test = $EX->SpawnWaitKill (10);

if ($test != 0) {
    print STDERR "ERROR: test error status $test\n";
    $status = 1;
}

exit $status;
