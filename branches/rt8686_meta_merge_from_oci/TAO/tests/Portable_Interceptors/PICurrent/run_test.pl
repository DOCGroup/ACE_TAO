eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");
$cvsfile= PerlACE::LocalFile ("svc.conf");
unlink $cvsfile;

sub DoTest
{
  $status = 0;

  if (PerlACE::is_vxworks_test()) {
      $SV = new PerlACE::ProcessVX ("server", "-o test.ior");
  }
  else {
      $SV = new PerlACE::Process ("server", "-o $file");    
  }
  $CL = new PerlACE::Process ("client", "-k file://$file");

  print STDERR "\n\n==== Running PICurrent test\n";

  unlink $file;
  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($file, 15) == -1) {
      print STDERR "ERROR: cannot find file <$file>\n";
      $SV->Kill (); 
      return 1;
  }

  $client = $CL->SpawnWaitKill (60);

  if ($client != 0) {
      print STDERR "ERROR: client returned $client\n";
      $status = 1;
  }

  $serverexit = $SV->WaitKill (5);

  if ($serverexit != 0) {
      print STDERR "ERROR: server returned $serverexit\n";
      $status = $serverexit;
  }
  
  return $status;
}

print STDERR "\n\n==== Without svc.conf (single-threaded)\n";
$status= DoTest();
if (0 != $status) {
   exit $status
}

$line= "static Server_Strategy_Factory \"-ORBConcurrency thread-per-connection\"\n";
open ($fh, '>', $cvsfile);
syswrite ($fh, $line);
close ($fh);
print STDERR "\n\n==== With svc.conf (thread-per-connection)\n";
$status= DoTest();
if (0 != $status) {
   exit $status
}

unlink $file;
unlink $cvsfile;

exit $status;
