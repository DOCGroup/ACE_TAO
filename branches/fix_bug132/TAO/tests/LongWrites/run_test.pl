eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;
use Cwd;
use Getopt::Std;

my $iorfile = PerlACE::LocalFile ("test.ior");
unlink $iorfile;

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

foreach my $i ("ONEWAY", "WRITE", "READ_WRITE") {

  print "================ Running test $i ================\n";

  my $SV = new PerlACE::Process ("server", "-o $iorfile $server_args");

  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
  }

  $CL1 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");
  $CL2 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");
  $CL3 = new PerlACE::Process ("client", " -k file://$iorfile -t $i");

  $CL1->Spawn ();
  $CL2->Spawn ();
  $CL3->Spawn ();

  $client1 = $CL1->WaitKill (120);
  $client2 = $CL2->WaitKill (120);
  $client3 = $CL3->WaitKill (120);
  $server = $SV->WaitKill (5);

  unlink $iorfile;
  
  if ($server != 0
      || $client1 != 0 || $client2 != 0 || $client3 != 0) {
    print "ERROR: non-zero status returned by the server or clients\n";
    print "ERROR: server = $server, client1 = $client1, client2 = $client2, client3 = $client3\n";
    exit 1;
  }
}

exit 0;
