eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile_1 = PerlACE::LocalFile ("ior_1");
$iorfile_2 = PerlACE::LocalFile ("ior_2");
$iorfile_3 = PerlACE::LocalFile ("ior_3");

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;

$SV = new PerlACE::Process ("server");

$CL1_1 = new PerlACE::Process ("client", "-k file://$iorfile_1");
$CL1_2 = new PerlACE::Process ("client", "-k file://$iorfile_1");
$CL1_3 = new PerlACE::Process ("client", "-k file://$iorfile_1");

$CL2_1 = new PerlACE::Process ("client", "-k file://$iorfile_2");
$CL2_2 = new PerlACE::Process ("client", "-k file://$iorfile_2");
$CL2_3 = new PerlACE::Process ("client", "-k file://$iorfile_2");

$CL3_1 = new PerlACE::Process ("client", "-k file://$iorfile_3");
$CL3_2 = new PerlACE::Process ("client", "-k file://$iorfile_3");
$CL3_3 = new PerlACE::Process ("client", "-k file://$iorfile_3 -x");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile_1, 5) == -1 ||
    PerlACE::waitforfile_timed ($iorfile_2, 5) == -1 ||
    PerlACE::waitforfile_timed ($iorfile_3, 5) == -1)
{
    print STDERR "ERROR: cannot find file ior files: $iorfile_1; $iorfile_2; or $iorfile_3\n";
    goto kill_server;
}

print STDERR "\n**************************************************\n";
print STDERR "Invoking methods on servant in default thread pool\n";
print STDERR "**************************************************\n\n";

$CL1_1->Spawn ();
$CL1_2->Spawn ();
$CL1_3->Spawn ();

$client = $CL1_1->WaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL1_2->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL1_3->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

if ($status != 0) 
{
    goto kill_server;
}

print STDERR "\n**************************************************\n";
print STDERR "Invoking methods on servant in first RT thread pool\n";
print STDERR "***************************************************\n\n";

$CL2_1->Spawn ();
$CL2_2->Spawn ();
$CL2_3->Spawn ();

$client = $CL2_1->WaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL2_2->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL2_3->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

if ($status != 0) 
{
    goto kill_server;
}

print STDERR "\n**************************************************\n";
print STDERR "Invoking methods on servant in second RT thread pool\n";
print STDERR "****************************************************\n\n";

$CL3_1->Spawn ();
$CL3_2->Spawn ();
$CL3_3->Spawn ();

$client = $CL3_1->WaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL3_2->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$client = $CL3_3->WaitKill (30);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

if ($status != 0) 
{
    goto kill_server;
}

kill_server:

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1
}

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;

exit $status
