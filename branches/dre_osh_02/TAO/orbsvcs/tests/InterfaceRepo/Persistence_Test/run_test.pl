eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
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

print STDERR "\t IFR Persistence Test\n\n";

$IFR = new PerlACE::Process ("../../../IFR_Service/IFR_Service", $persistent);
$T   = new PerlACE::Process ("Persistence_Test");

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $IFR->Kill (); 
    exit 1;
}

$T->Arguments ($init_ref);

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: populate test returned $test\n";
    $status = 1;
}

$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $iorfile;

$IFR->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $IFR->Kill (); 
    exit 1;
}

$T->Arguments ("$init_ref $debug $query_opt");


$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: query test returned $test\n";
    $status = 1;
}


$server = $IFR->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: IFR returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $backing_file;

exit $status;

