eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;

$iorfile = PerlACE::LocalFile ("server.ior");
$nsiorfile = PerlACE::LocalFile ("ns.ior");

unlink $iorfile;
unlink $nsiorfile;

$status = 0;
$host = hostname();

$ior = "file://$iorfile";
$server_port = 8088;
$name_port = 8087;
$server_config = PerlACE::LocalFile ("outside.conf");

foreach $i (@ARGV) {
    if ($i eq '-corbaloc') {
        $ior = "corbaloc:htiop:$host:$server_port/HelloObj";
    }
    elsif ($i eq '-corbaname') {
        $ior = "corbaname:htiop:$host:$name_port#HelloObj";
        $use_ns = 1;
    }
}

$NS =
    new PerlACE::Process ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          "-ORBSvcConf $server_config "
                          . "-ORBEndpoint \"iiop://;htiop://$host:$name_port\" "
                          . "-o $nsiorfile");

$NS->IgnoreExeSubDir ();
$NS->Spawn ();
print "Waiting for Name Service to start\n";
if (PerlACE::waitforfile_timed ($nsiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: nameserver not started\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SV =
  new PerlACE::Process ("server",
                        "-o $iorfile "
                        . "-ORBInitRef NameService=file://$nsiorfile "
                        . "-ORBSvcConf $server_config "
                        . "-ORBEndpoint htiop://$host:$server_port");

$CL = new PerlACE::Process ("client", " -k $ior");

$SV->Spawn ();
print "Waiting for server to start\n";
if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print "Running Client\n";
$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
    $SV->Kill(); $SV->TimedWait (1);
}
else {
    $server = $SV->WaitKill (10);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }
}

$NS->Kill ();
$NS->TimedWait (1);

unlink $nsiorfile;
unlink $iorfile;

exit $status;
