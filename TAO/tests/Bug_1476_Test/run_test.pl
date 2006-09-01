eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

$iorfile = PerlACE::LocalFile ("test.ior");

#The below hack is required for proper PerlACE initialization
$dum = new PerlACE::Process ("client");

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    exit 1;
}

@synchs = ("none","delayed");
@levels = ("obj", "orb", "thread");

print STDOUT @synchs;

for ($s = 0; $s < @synchs; $s++)
{
    $synch = $synchs[$s];

    for ($l = 0; $l < @levels; $l++)
    {
	$level = $levels[$l];

	$CL = new PerlACE::Process ("client", "-ORBDebuglevel $debug_level -k file://$iorfile -s$synch -l$level");

	$client = $CL->SpawnWaitKill (300);

	if ($client != 0) {
	    print STDERR "ERROR: client returned $client\n";
	    $status = 1;
	}

	print STDOUT "----------\n";
    }
}

exit $status;
