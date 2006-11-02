eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

#$confmod = "-static";

$iorfile = PerlACE::LocalFile ("server.ior");
$confserver = PerlACE::LocalFile ("server$confmod.conf");
$confclient = PerlACE::LocalFile ("client$confmod.conf");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -ORBSvcConf server.conf -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server",
                                "@ARGV -ORBSvcConf $confserver -o $iorfile");
}

$CL = new PerlACE::Process ("client",
                            "@ARGV -n 1 -ORBSvcConf $confclient -k file://$iorfile");

print STDERR $SV->CommandLine()."\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "$0: ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR $CL->CommandLine()."\n";
$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "$0: ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "$0: ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

#$confmod = "-static";

$iorfile = PerlACE::LocalFile ("server.ior");
$confserver = PerlACE::LocalFile ("server$confmod.conf");
$confclient = PerlACE::LocalFile ("client$confmod.conf");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -ORBSvcConf server.conf -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server",
                                "@ARGV -ORBSvcConf $confserver -o $iorfile");
}

$CL = new PerlACE::Process ("client",
                            "@ARGV -n 1 -ORBSvcConf $confclient -k file://$iorfile");

print STDERR $SV->CommandLine()."\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "$0: ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR $CL->CommandLine()."\n";
$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "$0: ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "$0: ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

#$confmod = "-static";

$iorfile = PerlACE::LocalFile ("server.ior");
$confserver = PerlACE::LocalFile ("server$confmod.conf");
$confclient = PerlACE::LocalFile ("client$confmod.conf");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -ORBSvcConf server.conf -o server.ior");
}
else {
    $SV = new PerlACE::Process ("server",
                                "@ARGV -ORBSvcConf $confserver -o $iorfile");
}

$CL = new PerlACE::Process ("client",
                            "@ARGV -n 1 -ORBSvcConf $confclient -k file://$iorfile");

print STDERR $SV->CommandLine()."\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "$0: ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR $CL->CommandLine()."\n";
$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "$0: ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "$0: ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
