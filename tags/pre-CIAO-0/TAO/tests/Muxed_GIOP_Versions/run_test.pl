eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

$TARGETHOSTNAME = "localhost";
$orb_port=12000 + PerlACE::uniqueid ();
$logfile = PerlACE::LocalFile("orb.$orb_port.log");
unlink $iorfile;
unlink $logfile;


# Testing Options:
# most effective is $serveriterations=1000 (with or without clients)
$verbose = 1;
$serverthreads = '10';
$clientthreads = '5';
$serveriterations = 1000;
$clientiterations = 1000;
$clients = 1;

# use GIOP 1.0
$corbaloc_str = "corbaloc:iiop:1.0\@$TARGETHOSTNAME:$orb_port/SomeObjectNameThatDontExist";


# ORBDebugLevel 10 seems to encourage the problem
# -ORBCollocation no  is required for server to produce the problem
$serverargs = "-ORBCollocation no -ORBdebuglevel 10 -ORBLogFile $logfile -ORBEndpoint iiop://$TARGETHOSTNAME:$orb_port -o $iorfile -i $serveriterations -n $serverthreads -l $corbaloc_str";

$SV = new PerlACE::Process ("server", $serverargs);

$clientargs = "-ORBdebuglevel 10 -ORBLogFile $logfile -l $corbaloc_str -k file://$iorfile -n $clientthreads -i $clientiterations";

$CL1 = new PerlACE::Process ("client", $clientargs);
$CL2 = new PerlACE::Process ("client", $clientargs);
$CL3 = new PerlACE::Process ("client", $clientargs);
$CLS = new PerlACE::Process ("client", "-l $corbaloc_str -k file://$iorfile -i 1 -x ");


#print STDERR $SV->CommandLine(), "\n" if $verbose;

print STDERR "***** Start the server*** \n" if $verbose;

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 50) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

# leave server reap some self abuse before clients start abusing it
if ($serveriterations > 20) {
  $server = $SV->TimedWait(10);
}

print STDERR "****** Start the clients*** \n" if $verbose;
if ($clients > 0) {
  print STDERR $CL1->CommandLine(), "\n" if $verbose;
  $CL1->Spawn ();
}
if ($clients > 1) { $CL2->Spawn (); }
if ($clients > 2) { $CL3->Spawn (); }


if ($clients > 0) {
  $client = $CL1->WaitKill (180);

  if ($client != 0) {
    print STDERR "ERROR: client 1 returned $client\n";
    $status = 1;
  }
}

if ($clients > 1) {
  $client = $CL2->WaitKill (5);
  
  if ($client != 0) {
    print STDERR "ERROR: client 2 returned $client\n";
    $status = 1;
  }
}

if ($clients > 2) {
  $client = $CL3->WaitKill (5);
  
  if ($client != 0) {
    print STDERR "ERROR: client 3 returned $client\n";
    $status = 1;
}

}

$client = $CLS->SpawnWaitKill (15);

if ($client != 0) {
    print STDERR "ERROR: client 4 returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (120);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $logfile;

exit $status








