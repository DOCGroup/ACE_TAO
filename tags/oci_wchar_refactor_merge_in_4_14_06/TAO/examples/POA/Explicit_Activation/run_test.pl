eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("ior");

$oneway = "";
$iterations = 100;

$extra_args = "";

# Parse the arguments
for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-h] [-i iterations] [-o] [-f ior file]\n";
        print "\n";
        print "-h                  -- prints this information\n";
        print "-f                  -- ior file\n";
        print "-i iterations       -- specifies iterations\n";
        print "-o                  -- call issued are oneways\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-o") {
        $oneway = "-o";
    }
    elsif ($ARGV[$i] eq "-i") {
        $iterations = $ARGV[$i + 1];
        $i++;
    }
    elsif ($ARGV[$i] eq "-f") {
        $iorfile = $ARGV[$i + 1];
        $i++;
    }
    else {
        $extra_args .= " " . $ARGV[$i];
    }
}

$iorfile_1 = $iorfile."_1";
$iorfile_2 = $iorfile."_2";
$iorfile_3 = $iorfile."_3";

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;

$status = 0;

$SV = new PerlACE::Process ("server", "-f $iorfile $extra_args");
$CL = new PerlACE::Process ("../Generic_Servant/client");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile_1, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_1>\n";
    $SV->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile_2, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_2>\n";
    $SV->Kill (); 
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile_3, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_3>\n";
    $SV->Kill (); 
    exit 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_1");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 1 returned $client\n";
    $status = 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_2");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 2 returned $client\n";
    $status = 1;
}

$CL->Arguments ("$extra_args $oneway -i $iterations -k file://$iorfile_3 -x");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 3 returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;

exit $status;
