eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

$status = 0;
$endopoint = "";
$use_uiop = 0;
$use_shmiop = 0;
$server_count = 20;
@SV = ();

foreach $arg (@ARGV) {
  if ($arg eq "-h") {
    print "Usage: $0 [-u | -s]\n" .
          "       -u for UIOP\n" .
          "       -s for SHMIOP\n";
    exit(0);
  }
  elsif ($arg eq "-u") {
    $use_uiop = 1;
  }
  elsif ($arg eq "-s") {
    $use_shmiop = 1;
  }
}

for($i = 0; $i < $server_count; $i++) {
  if ($use_uiop) {
    $socket = PerlACE::LocalFile ("socket.$i");
    unlink $socket;
    $endpoint = "-ORBEndpoint uiop://$socket";
  }
  elsif ($use_shmiop) {
    $server_shmiop_conf = PerlACE::LocalFile ("server_shmiop.conf");
    $endpoint = "-ORBEndpoint shmiop:// -ORBSvcConf $server_shmiop_conf";
  }

  unlink "$iorfile.$i";

  $SV[$i] = new PerlACE::Process ("server", "$endpoint -o $iorfile.$i");
  $SV[$i]->Spawn ();

  if (PerlACE::waitforfile_timed ("$iorfile.$i", 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile.$i>\n";
    $SV[$i]->Kill (); $SV[$i]->TimedWait (1);
    exit 1;
  }
}

foreach $conf ("client.lru.conf", "client.lfu.conf",
               "client.fifo.conf", "client.null.conf") {
  print "========== Client using $conf configurator file =========\n";
  $CL = new PerlACE::Process ("client", " -k $iorfile " .
                                        "-ORBSvcConf $conf ".
                                        "-ORBDebugLevel 1");

  $client = $CL->SpawnWaitKill (60);

  if ($client != 0) {
      print STDERR "ERROR: client returned $client\n";
      $status = 1;
  }
}

for($i = 0; $i < $server_count; $i++) {
  $SV[$i]->Kill ();
  unlink "$iorfile.$i";
  unlink "socket.$i";
}

exit $status;
