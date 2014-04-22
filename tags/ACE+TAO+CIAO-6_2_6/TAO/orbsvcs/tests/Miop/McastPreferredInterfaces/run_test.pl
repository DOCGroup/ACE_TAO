eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server_box = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client_box = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server_box->LocalFile ($iorbase);
my $client_iorfile = $client_box->LocalFile ($iorbase);
$server_box->DeleteFile($iorbase);
$client_box->DeleteFile($iorbase);

$port = $server_box->RandomPort ();

## NOTE we duplicate the server's CORBALOC in the client to test for CORBALOC processing of -ORBPreferredInterfaces
$SV = $server_box->CreateProcess ("server", "-o $server_iorfile -u corbaloc:miop:1.0\@1.0-foo-1/225.1.1.8:$port -ORBSvcConf miop_svc.conf");
$CL = $client_box->CreateProcess ("client", "-ORBPreferredInterfaces 225.*=127.0.0.1 -ORBEnforcePreferredInterfaces=1 -ORBIPMulticastLoop 1 -ORBSvcConf miop_svc.conf -k corbaloc:miop:1.0\@1.0-foo-1/225.1.1.8:$port");

print "Starting first part of test.\n";
$server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}
if ($server_box->WaitForFileTimed ($iorbase,
                               $server_box->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server_box->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client_box->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client_box->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}
if ($SV->TimedWait(1) != -1) {
    print "Server has exited. This means the server received the 1st client message & so the preferred interfaces option might not have worked.\n";
}
else {
  # Now we run the client again to close down the server cleanly
  # and we use the server written ior file here to test the normal cdr -ORBPreferredInterfaces processing.
  print "Starting second part of test.\n";
  $CL->Arguments("-ORBSvcConf miop_svc.conf -ORBIPMulticastLoop 1 -k file://$client_iorfile");

  $client_status = $CL->SpawnWaitKill ($client_box->ProcessStartWaitInterval());
  if ($client_status != 0) {
      print STDERR "ERROR: client returned $client_status trying to shutdown server\n";
      $status = 1;
  }

  $server_status = $SV->WaitKill ($server_box->ProcessStopWaitInterval());
  if ($server_status != 0) {
      print STDERR "ERROR: server returned $server_status\n";
      $status = 1;
  }
}

$server_box->DeleteFile($iorbase);
$client_box->DeleteFile($iorbase);

print "End of test.\n";
exit $status;
