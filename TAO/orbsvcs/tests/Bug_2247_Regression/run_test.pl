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

$SERV1 = new PerlACE::Process ("server", "-o $file1ior -k KEY1");
$SERV2 = new PerlACE::Process ("server", "-o $file2ior -k KEY2");
$MANAGER_00 = new PerlACE::Process ("Manager", "-a file://$file1ior -k KEY1 -b file://$file2ior -l KEY2 -c $outputior");
$MANAGER_01 = new PerlACE::Process ("Manager", "-a file://$file1ior -k KEY1 -b file://$file2ior -l KEY2 -c $outputior -s");
$MANAGER_10 = new PerlACE::Process ("Manager", "-a file://$file1ior -k KEY1 -b file://$file2ior -l KEY2 -c $outputior -m");
$MANAGER_11 = new PerlACE::Process ("Manager", "-a file://$file1ior -k KEY1 -b file://$file2ior -l KEY2 -c $outputior -m -s");

# ---------------
print STDERR "Starting ABORT test\n";

print STDERR "Starting Server 1\n";
$SERV1->Spawn ();
if (PerlACE::waitforfile_timed ($file1ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file1ior>\n";
	$SERV1->Kill ();
	exit 1;
}

print STDERR "Starting ABORT Manager\n";
$MANAGER_00->Spawn ();
$manager = $MANAGER_00->WaitKill (30);

$SERV1->WaitKill(5);

if ($manager != 0) {
	print STDERR "ERROR: Manager returned $manager\n";
	++ $status;
}

unlink $file1ior, $outputior;

# ---------------
print STDERR "Starting SHUTDOWN test\n";

print STDERR "Starting Server 1\n";
$SERV1->Spawn ();
if (PerlACE::waitforfile_timed ($file1ior, $PerlACE::wait_interval_for_process_creation) == -1) {
	print STDERR "ERROR: cannot find file <$file1ior>\n";
	$SERV1->Kill ();
	exit 1;
}

print STDERR "Starting SHUTDOWN Manager\n";
$MANAGER_01->Spawn ();
$manager = $MANAGER_01->WaitKill (30);

$SERV1->WaitKill(5);

if ($manager != 0) {
	print STDERR "ERROR: Manager returned $manager\n";
	++ $status;
}

unlink $file1ior, $outputior;

# ---------------
print STDERR "Starting MERGED ABORT test\n";

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

print STDERR "Starting ABORT Manager\n";
$MANAGER_10->Spawn ();
$manager = $MANAGER_10->WaitKill (30);

$SERV1->WaitKill(5);
$SERV2->WaitKill(5);

if ($manager != 0) {
	print STDERR "ERROR: Manager returned $manager\n";
	++ $status;
}

unlink $file1ior, $file2ior, $outputior;

# ---------------
print STDERR "Starting MERGED SHUTDOWN test\n";

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

print STDERR "Starting SHUTDOWN Manager\n";
$MANAGER_11->Spawn ();
$manager = $MANAGER_11->WaitKill (30);

$SERV1->WaitKill(5);
$SERV2->WaitKill(5);

if ($manager != 0) {
	print STDERR "ERROR: Manager returned $manager\n";
	++ $status;
}

unlink $file1ior, $file2ior, $outputior;

exit $status
