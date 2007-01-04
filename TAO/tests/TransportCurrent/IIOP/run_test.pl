eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

sub add_path {
    my($name)  = shift;
    my($value) = shift;
    if (defined $ENV{$name}) {
        $ENV{$name} .= ':' . $value
        }
    else {
        $ENV{$name} = $value;
    }
}

# Set the library path for the client to be able to load
# the common library library.
add_path('LD_LIBRARY_PATH', '../lib');
add_path('LIBPATH', '../lib');
add_path('SHLIB_PATH', '../lib');

$status = 0;

$client = PerlACE::LocalFile ("client");

$mode = shift (@ARGV);
if ( $mode =~ /-dynamic/) {
    $client_conf_file = PerlACE::LocalFile ("client_dynamic.conf");
    $server_conf_file = PerlACE::LocalFile ("server_dynamic.conf");
}
elsif  ( $mode =~ /-static/) {
    $client_conf_file = PerlACE::LocalFile ("client_static.conf");
    $server_conf_file = PerlACE::LocalFile ("server_static.conf");
}
else {
    print STDERR "Unknown $mode. Specify -static or -dynamic\n";
    exit 1;
}

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server",
                                  "@ARGV -c 0 -ORBSvcConf $server_conf_file -o server.ior");
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
