eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");
$middlefile = PerlACE::LocalFile ("middle.ior");
$dum_core = PerlACE::LocalFile ("core");
$status = 0;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client", "");
$MD = new PerlACE::Process ("middle", "-o $middlefile -k file://$iorfile");
$PI = new PerlACE::Process ("ping", "-o $iorfile");
$PO = new PerlACE::Process ("pong", "-k file://$iorfile -p 100 -i 60 -t 30");

###############################################################################
print STDERR "===== Base test, no crashes\n";

unlink $iorfile;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$CL->Arguments ("-k file://$iorfile -i 100");

$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

###############################################################################
print STDERR "===== Client crash (abort) during upcall\n";

$CL->Arguments ("-k file://$iorfile -i 100 -s");

$client = $CL->SpawnWaitKill (20);

# since the client crashes, skip check for $client != 0
if ($client == -1) {
    $status = 1;
}

###############################################################################
print STDERR "===== Client crash during upcall\n";

$CL->Arguments ("-k file://$iorfile -i 100 -z");

$client = $CL->SpawnWaitKill (20);

# since the client crashes, skip check for $client != 0
if ($client == -1) {
    $status = 1;
}

###############################################################################
print STDERR "===== Server crash (abort) during upcall\n";

$CL->Arguments ("-k file://$iorfile -i 100 -a");

$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (20);

# since the server crashes, skip check for $server != 0
if ($server == -1) {
    $status = 1;
}

###############################################################################
print STDERR "===== Server crash during upcall\n";

unlink $iorfile;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$CL->Arguments ("-k file://$iorfile -i 100 -c");

$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (20);

# since the server crashes, skip check for $server != 0
if ($server == -1) {
    $status = 1;
}

###############################################################################
print STDERR "===== Three-way test, client crashes and server detects\n";

unlink $iorfile;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

unlink $middlefile;
$MD->Spawn ();

if (PerlACE::waitforfile_timed ($middlefile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$middlefile>\n";
    $MD->Kill (); 
    exit 1;
}

$CL->Arguments ("-k file://$middlefile -i 100 -s");

$client = $CL->SpawnWaitKill (100);

# since the client crashes, skip check for $client != 0
if ($client == -1) {
    $status = 1;
}

$CL->Arguments (" -k file://$middlefile -i 10 -x");

$client = $CL->SpawnWaitKill (100);

# since the client crashes, skip check for $client != 0
if ($client == -1) {
    $status = 1;
}

$server = $SV->WaitKill (20);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$middle = $MD->WaitKill (20);

if ($middle != 0) {
    print STDERR "ERROR: the middle test failed, middle returned $middle\n";
    $status = 1;
}

###############################################################################
print STDERR "===== Ping-pong test, server crashes but client continues\n";

unlink $iorfile;
$PI->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

if ($PO->SpawnWaitKill (60) == -1) {
    $status = 1;
}

if ($PI->WaitKill (60) == -1) {
    $status = 1;
}

unlink $dum_core;

exit $status;
