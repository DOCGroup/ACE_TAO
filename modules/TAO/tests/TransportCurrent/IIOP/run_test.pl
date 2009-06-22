eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use Config;
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

sub add_path {
    my($name)  = shift;
    my($value) = shift;
    if (defined $ENV{$name}) {
        $ENV{$name} .= $Config{'path_sep'} . $value
        }
    else {
        $ENV{$name} = $value;
    }
}

$status = 0;

$client = PerlACE::LocalFile ("client");

$mode = shift (@ARGV);
if ( $mode =~ /-dynamic/) {
    $base_client_conf = "client_dynamic.conf";
    $base_server_conf = "server_dynamic.conf";
    $client_conf_file = PerlACE::LocalFile ("$base_client_conf");
    $server_conf_file = PerlACE::LocalFile ("$base_server_conf");
}
elsif  ( $mode =~ /-static/) {
    $base_client_conf = "client_static.conf";
    $base_server_conf = "server_static.conf";
    $client_conf_file = PerlACE::LocalFile ("$base_client_conf");
    $server_conf_file = PerlACE::LocalFile ("$base_server_conf");
}
else {
    print STDERR "Unknown $mode. Specify -static or -dynamic\n";
    exit 1;
}

$baseiorfile = "server.ior";
$iorfile = PerlACE::LocalFile ("$baseiorfile");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -c 0 -ORBSvcConf $base_server_conf -o $baseiorfile");
}
else {
    $SV = new PerlACE::Process ("server",
                                "@ARGV -c 0 -ORBSvcConf $server_conf_file -o $iorfile");
}

$CL = new PerlACE::Process ($client,
                            "@ARGV -n 1 -ORBSvcConf $client_conf_file -k file://$iorfile");

print STDERR $SV->CommandLine()."\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "$0: ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR $CL->CommandLine()."\n";
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "$0: ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "$0: ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
