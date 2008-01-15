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
$output10ior = PerlACE::LocalFile ("iogr_10.ior");
$output11ior = PerlACE::LocalFile ("iogr_11.ior");

unlink $file1ior, $file2ior, $output10ior, $output11ior;

$SERV1 = new PerlACE::Process ("server", "-o $file1ior -f file://$output11ior -k KEY1");
$SERV2 = new PerlACE::Process ("server", "-o $file2ior -k KEY2");
$CLIENT = new PerlACE::Process ("client", "-a file://$file1ior -k KEY1 -b file://$file2ior -l KEY2 -g $output10ior -h $output11ior");

# ---------------

print STDERR "Starting Server 1\n";
$SERV1->Spawn ();
if (PerlACE::waitforfile_timed ($file1ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file1ior>\n";
	$SERV1->Kill ();
	exit 1;
}

print STDERR "Starting Server 2\n";
$SERV2->Spawn ();
if (PerlACE::waitforfile_timed ($file2ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file2ior>\n";
	$SERV1->Kill ();
	$SERV2->Kill ();
	exit 1;
}

print STDERR "Starting Client\n";
$CLIENT->Spawn ();
$client = $CLIENT->WaitKill (30);

$SERV1->WaitKill(5);
$SERV2->WaitKill(5);

if ($client != 0) {
	print STDERR "ERROR: client returned $client\n";
	++ $status;
}

unlink $file1ior, $file2ior, $output10ior, $output11ior;

exit $status
