eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use Getopt::Std;

local ($opt_i, $opt_b);

if (!getopts ('i:b:')) {
    print "Usage: run_test.pl [-b payload_size] [-i iterations]\n";
    exit 1;
}

my $server_args = " -p 3";
if (defined $opt_i) {
    $server_args .= " -i ".$opt_i;
}
if (defined $opt_b) {
    $server_args .= " -b ".$opt_b;
}

$iorfile = PerlACE::LocalFile ("server.ior");

$status = 0;
unlink $iorfile;
if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior $server_args");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile $server_args");    
}
$CL1 = new PerlACE::Process ("client", " -k file://$iorfile");
$CL2 = new PerlACE::Process ("client", " -k file://$iorfile");
$CL3 = new PerlACE::Process ("client", " -k file://$iorfile");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL1->Spawn ();
$CL2->Spawn ();
$CL3->Spawn ();

$client1 = $CL1->WaitKill (300);

if ($client1 != 0) {
  print STDERR "ERROR: client 1 returned $client1\n";
  $status = 1;
}

$client2 = $CL2->WaitKill (300);

if ($client2 != 0) {
  print STDERR "ERROR: client 2 returned $client2\n";
  $status = 1;
}

$client3 = $CL3->WaitKill (300);

if ($client3 != 0) {
  print STDERR "ERROR: client 3 returned $client3\n";
  $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  $status = 1;
}

unlink $iorfile;

exit $status;
