eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
use Getopt::Std;

local ($opt_i, $opt_p);

if (!getopts ('i:p:')) {
    print "Usage: run_test.pl [-i iterations]\n";
    exit 1;
}

my $iterations = 2000;
if (defined $opt_i) {
    $iterations = $opt_i;
}

$SV = new PerlACE::Process ("server", "-o $iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

for ($i = 0; $i != $iterations; $i++) {

    $CL = new PerlACE::Process ("client", " -k file://$iorfile");
    $client = $CL->SpawnWaitKill (20);

    if ($client != 0) {
	print STDERR "ERROR: client returned $client in iteration $i\n";
	$status = 1;
    }
    if ($i % 100 == 0) {
	print STDERR "Iteration $i / $iterations\n";
    }
}

$CL = new PerlACE::Process ("client", " -k file://$iorfile -x");
$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client during test shutdown\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
