eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile = "ior";

$oneway = "";
$iterations = 100;

$extra_args = "";

$status = 0;

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

$iorfile1 = $iorfile."_1";
$iorfile2 = $iorfile."_2";

unlink $iorfile1, $iorfile2;

$SV  = new PerlACE::Process ("server", "-f $iorfile $extra_args");
$CL1 = new PerlACE::Process ("../Generic_Servant/client", 
                             "$extra_args $oneway -i $iterations -f $iorfile1");
$CL2 = new PerlACE::Process ("../Generic_Servant/client",
                             "$extra_args $oneway -i $iterations -f $iorfile2 -x"); 

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile2, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL1->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 1 returned $client\n";
    $status = 1;
}

$client = $CL2->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 2 returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $SV->Kill (); 
    $status = 1;
}

unlink $iorfile1, $iorfile2;

exit $status;
