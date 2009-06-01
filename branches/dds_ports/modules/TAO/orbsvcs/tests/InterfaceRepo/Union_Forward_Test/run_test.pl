eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$locking = "";

$iorfile = "if_repo.ior";

$status = 0;

$nice = "";
$ifrref = "-ORBInitRef InterfaceRepository=file://$iorfile";
unlink $iorfile;

$SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service",
                            " $nice " . " -o $iorfile" . " $locking");
$TAO_IFR = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/tao_ifr", 
                                 $ifrref . " test.idl");
$CLIENT = new PerlACE::Process ("./client", $ifrref);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$tao_ifr = $TAO_IFR->SpawnWaitKill (60);

if ($tao_ifr != 0) {
    print STDERR "ERROR: tao_ifr returned $tao_ifr\n";
    $status = 1;
}
else {
  $client = $CLIENT->SpawnWaitKill (60);

  if ($client != 0) {
      print STDERR "ERROR: client returned $client\n";
      $status = 1;
  }
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
