eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

print STDERR "================ Thread Per Connection Sequence Latency Test -- octet sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t octet "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}


print STDERR "================ Thread Per Connection Sequence Latency Test -- long sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t long "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;


print STDERR "================ Thread Per Connection Sequence Latency Test -- short sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t short "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;


print STDERR "================ Thread Per Connection Sequence Latency Test -- char sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t char "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;


print STDERR "================ Thread Per Connection Sequence Latency Test -- longlong sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t longlong "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;


print STDERR "================ Thread Per Connection Sequence Latency Test -- double sequence\n";

unlink $iorfile;

$SV = new PerlACE::Process ("server",
                            "-o $iorfile");

$CL = new PerlACE::Process ("client",
                            "-t doube "
                            . "-k file://$iorfile "
                            . " -i 10000");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);
$server = $SV->WaitKill (10);

unlink $iorfile;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
