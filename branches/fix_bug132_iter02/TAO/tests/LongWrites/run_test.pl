eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use Getopt::Std;

local ($opt_i, $opt_p);

if (!getopts ('i:p:')) {
    print "Usage: run_test.pl [-p payload_size] [-i iterations]\n";
    exit 1;
}

my $server_args = "";
if (defined $opt_i) {
    $server_args .= " -i ".$opt_i;
}
if (defined $opt_p) {
    $server_args .= " -p ".$opt_p;
}

$iorfile = PerlACE::LocalFile ("server.ior");

foreach my $i ("ONEWAY") { # , "WRITE", "READ_WRITE") {

  print "================ Running test $i ================\n";


  unlink $iorfile;
  $SV  = new PerlACE::Process ("server", "-o $iorfile $server_args");
  $CL1 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");
  $CL2 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");
  $CL3 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");


  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
  }

  $CL1->Spawn (1200);
  $CL2->Spawn (1200);
  $CL3->Spawn (1200);

  $client1 = $CL1->WaitKill (60);

  if ($client1 != 0) {
    print STDERR "ERROR: client 1 returned $client1\n";
    $status = 1;
  }

  $client2 = $CL2->WaitKill (60);

  if ($client2 != 0) {
    print STDERR "ERROR: client 2 returned $client2\n";
    $status = 1;
  }

  $client3 = $CL3->WaitKill (60);

  if ($client3 != 0) {
    print STDERR "ERROR: client 3 returned $client3\n";
    $status = 1;
  }

  $server = $SV->TerminateWaitKill (5);

  if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
  }
}

unlink $iorfile;

exit $status;
