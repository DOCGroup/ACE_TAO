eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$persistent = "-p";

$status = 0;

$iorfile = "if_repo.ior";
$backing_file = PerlACE::LocalFile ("ifr_default_backing_store");

$init_ref = "-ORBInitRef InterfaceRepository=file://$iorfile";

$debug = "";
$query_opt = "-q";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-d") {
        $debug = "-d";
    }
    else {
        $other .= $ARGV[$i];
    }
}

unlink $iorfile;
unlink $backing_file;

$IFR = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " -o $iorfile" . " $persistent");
$T   = new PerlACE::Process ("Persistence_Test");

print "Starting IFR_Service\n";
$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $IFR->Kill ();
    unlink $backing_file;
    exit 1;
}

$T->Arguments ($init_ref);

print "Starting Persistence_Test\n";
$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: populate test returned $test\n";
    $status = 1;
}

print "Terminating IFR_Service\n";
$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $iorfile;

if ($status == 1) {
    print STDERR "ERROR: There is a problem during the first run\n";
    unlink $backing_file;
    exit 1;
}

print "Starting IFR_Service\n";
$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $IFR->Kill ();
    unlink $backing_file;
    exit 1;
}

$T->Arguments ("$init_ref $debug $query_opt");

print "Starting Persistence_Test\n";
$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: query test returned $test\n";
    $status = 1;
}

print "Terminating IFR_Service\n";
$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $backing_file;

exit $status;
