eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;  
 
# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$file1ior = PerlACE::LocalFile ("file1.ior");
$file2ior = PerlACE::LocalFile ("file2.ior");
$outputior = PerlACE::LocalFile ("output.ior");

unlink $file1ior, $file2ior, $outputior;

$SERV1 = new PerlACE::Process ("server", "-o $file1ior");
$SERV2 = new PerlACE::Process ("server", "-o $file2ior");
$MANAGER = new PerlACE::Process ("Manager", "-a file://$file1ior -b file://$file2ior -c $outputior");

print STDERR "Starting Server\n";

$SERV1->Spawn ();

if (PerlACE::waitforfile_timed ($file1ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file1ior>\n";
	$SERV1->Kill ();
	exit 1;
}

print STDERR "Starting Server\n";

$SERV2->Spawn ();

if (PerlACE::waitforfile_timed ($file2ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file2ior>\n";
	$SERV1->Kill ();
	$SERV2->Kill ();
	exit 1;
}

print STDERR "Starting Manager\n";

$MANAGER->Spawn ();

$manager = $MANAGER->WaitKill (30);

$SERV1->WaitKill(5);

$SERV2->WaitKill(5);

if ($manager != 0) {
	print STDERR "ERROR: Manager returned $manager\n";
	$status = 1;
}

unlink $file1ior, $file2ior, $outputior;

exit $status
