eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfilebase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase -e 400 -l 800");
}
else {
    $SV  = new PerlACE::Process ("server", "-o $iorfile -e 400 -l 800");
}
$CL1 = new PerlACE::Process ("client", " -k file://$iorfile");
$CL2 = new PerlACE::Process ("client", " -k file://$iorfile");

for ($n = 0; $n < 10; ++$n) {
  print "=================================================================\n";
  unlink $iorfile;

  $SV->Spawn ();
  if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find file <$iorfile>\n";
      $SV->Kill ();
      exit 1;
  }

  $CL1->Spawn();
  $CL2->Spawn();

  $client1 = $CL1->WaitKill (40);
  if ($client1 != 0) {
      print STDERR "ERROR: client 1 returned $client1\n";
      $CL2->Kill();
      $SV->Kill();
      exit 1;
  }

  $client2 = $CL2->WaitKill (40);
  if ($client2 != 0) {
      print STDERR "ERROR: client 2 returned $client2\n";
      $SV->Kill();
      exit 1;
  }

  $server = $SV->WaitKill (60);
  if ($server != 0) {
      print STDERR "ERROR: server returned $server\n";
      exit 1;
  }
}

unlink $iorfile;

exit 0;
