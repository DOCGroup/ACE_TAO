eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;
use Getopt::Std;

local ($opt_i, $opt_b);

if (!getopts ('i:')) {
    print "Usage: run_test.pl [-i iterations]\n";
    exit 1;
}

my $server_args = " -p 2";
if (defined $opt_i) {
    $server_args .= " -i ".$opt_i;
}

my $iorfile = PerlACE::LocalFile ("server.ior");
my $client_conf = PerlACE::LocalFile ("client.conf");

unlink $iorfile;
$SV  = new PerlACE::Process ("server", "-o $iorfile $server_args");

my $client_opts = "-ORBSvcConf $client_conf -k file://$iorfile";
$CL1 = new PerlACE::Process ("client", $client_opts);
$CL2 = new PerlACE::Process ("client", $client_opts);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL1->Spawn ();
$CL2->Spawn ();

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

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
  print STDERR "ERROR: server returned $server\n";
  $status = 1;
}

unlink $iorfile;

exit $status;
