eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$locking = "";

$iorfile = "if_repo.ior";

$status = 0;

$nice = "-ORBDottedDecimalAddresses 1";
$debug = "";
$test = "";
$iterations = "";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-d") {
        $debug = "-d";
    }
    elsif ($ARGV[$i] eq "-t") {
        $test = "-t ".$ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-i") {
        $iterations = "-i ".$ARGV[$i + 1];
        $i++;
    }
    else {
        $other .= $ARGV[$i];
    }
}

unlink $iorfile;

$SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " $nice " . " $locking");
$CL = new PerlACE::Process ("IFR_Test", 
                            "-ORBInitRef InterfaceRepository=file://$iorfile"
                            . " $debug $test $iterations");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;

