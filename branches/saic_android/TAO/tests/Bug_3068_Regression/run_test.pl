eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
use Getopt::Long;

$status = 0;
$opt = "-ORBListenendpoints iiop://:/reuse_addr=0 ";
$conf_client = "";
$conf_server = "";
$iorfile = PerlACE::LocalFile ("test.ior");

sub options () {
  my $debug;        # handled locally

  # Process options.
  if ( @ARGV > 0 ) {
      GetOptions ('debug=i' => \$debug);
  }

  $conf_client = " -ORBSvcConf client.conf";
  $conf_server = " -ORBSvcConf server.conf";

  if ($debug) {
    $opt = "$opt -ORBDebugLevel $debug";
  }

  return 1;
}

options () or die "Nothing executed";

unlink $iorfile;

print STDERR "Executing, server options=$conf_server $opt -o $iorfile\n";
$SV = new PerlACE::Process ("server",
                            "$conf_server $opt -o $iorfile");

print STDERR "Executing, client options=$conf_client -k file://$iorfile\n";
$CL = new PerlACE::Process ("client",
                            "$conf_client -k file://$iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

if ($status != 0) {
  print STDERR "Test failed\n";
}
else {
  print STDERR "Test succeded\n";
}

exit $status;
