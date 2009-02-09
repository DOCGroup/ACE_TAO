eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ('server.ior');
$TARGETHOSTNAME = 'localhost';
$port = PerlACE::uniqueid () + 12000;

## No ORB fragments GIOP 1.0 messages.
## The JDK ORB only fragments GIOP 1.2 messages.
foreach my $giop ('1.2') {
  print "Testing GIOP $giop Fragmentation\n";
  unlink $iorfile;

  $SV  = new PerlACE::Process ('server',
                               '-ORBEndpoint ' .
                               "iiop://$giop\@$TARGETHOSTNAME" . ":$port");
  $SV->Spawn ();

  if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: cannot find file <$iorfile>\n";
      $SV->Kill (); $SV->TimedWait (1);
      exit 1;
  }

  my($cl) = system('java client');
  if ($cl != 0) {
      print STDERR "ERROR: client returned $cl\n";
      ++$status;
  }

  $server = $SV->WaitKill (20);

  if ($server != 0) {
      print STDERR "ERROR: server returned $server\n";
      ++$status;
  }

  unlink $iorfile;

  if ($status) {
    last;
  }
}

exit $status;
