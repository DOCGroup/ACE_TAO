eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "lf.ior";
my $tp_conf_base = "tp$PerlACE::svcconf_ext";
my $select_mt_conf_base = "select_mt$PerlACE::svcconf_ext";

# init server configuration files
my $server_tp_conf_file = $server->LocalFile ($tp_conf_base);
my $server_select_mt_conf_file = $server->LocalFile ($select_mt_conf_base);

# init client configuration files
my $client_tp_conf_file = $client->LocalFile ($tp_conf_base);
my $client_select_mt_conf_file = $client->LocalFile ($select_mt_conf_base);

# init ior files
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server");
$CL = $client->CreateProcess ("client");

sub run_client ($)
{
    my $args = shift;

    $CL->Arguments ("-k file://$client_iorfile " . $args);

    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 185);

    if ($client_status != 0) {
        my $time = localtime;
        print STDERR "ERROR: client returned $client_status at $time\n";
        $status = 1;
    }
}

sub run_clients ()
{
    print STDERR "\n\n*** No event loop threads ***\n\n\n";

    print STDERR "\nSelect Reactor\n\n";

    run_client ("-ORBsvcconf $client_select_mt_conf_file -e 0");

    print STDERR "\nTP Reactor\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 0");

    print STDERR "\n\n*** Single-threaded client event loop: Select Reactor ***\n\n\n";

    print STDERR "\nSingle-threaded client running event loop for 3 seconds\n\n";

    run_client ("-ORBsvcconf $client_select_mt_conf_file -e 1 -t 3000");

    print STDERR "\nSingle-threaded client running event loop for 10 seconds\n\n";

    run_client ("-ORBsvcconf $client_select_mt_conf_file -e 1 -t 10000");

    print STDERR "\nSingle-threaded client running event loop for 20 seconds\n\n";

    run_client ("-ORBsvcconf $client_select_mt_conf_file -e 1 -t 20000");

    print STDERR "\n\n*** Single-threaded client event loop: TP Reactor ***\n\n\n";

    print STDERR "\nSingle-threaded client running event loop for 3 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 1 -t 3000");

    print STDERR "\nSingle-threaded client running event loop for 10 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 1 -t 10000");

    print STDERR "\nSingle-threaded client running event loop for 20 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 1 -t 20000");

    print STDERR "\n\n*** Multi-threaded client event loop: TP Reactor ***\n\n\n";

    print STDERR "\nMulti-threaded client running event loop for 3 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 5 -t 3000");

    print STDERR "\nMulti-threaded client running event loop for 10 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 5 -t 10000");

    print STDERR "\nMulti-threaded client running event loop for 20 seconds\n\n";

    run_client ("-ORBsvcconf $client_tp_conf_file -e 5 -t 20000 -x");
}

sub run_server ($)
{
    my $args = shift;

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);

    $SV->Arguments ("-o $server_iorfile " . $args);

    my $server_status = $SV->Spawn ();

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        return 1;
    }

    if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($server->GetFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    run_clients ();

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 85);

    if ($server_status != 0) {
        my $time = localtime;
        print STDERR "ERROR: server returned $server_status at $time\n";
        $status = 1;
    }

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
}

my $single = 1;
my $multi = 0;

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-m] -[a]\n";
        print "\n";
        print "-m  -- tests the multithreaded server (default is single)\n";
        print "-a  -- tests both multi and single threaded servers\n";
        exit 0;
    }
    elsif ($ARGV[$i] eq "-m") {
        $multi = 1;
        $single = 0;
    }
    elsif ($ARGV[$i] eq "-a") {
        $multi = 1;
        $single = 1;
    }
}

# Copy configuration files to the targets
if ($server->PutFile ($tp_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$server_tp_conf_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}
if ($server->PutFile ($select_mt_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$server_select_mt_conf_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}

if ($client->PutFile ($tp_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_tp_conf_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}
if ($client->PutFile ($select_mt_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_select_mt_conf_file>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}

if ($single == 1) {
    print STDERR "\n\n*** Single threaded server ***\n\n\n";
    $run_status = run_server("-ORBsvcconf $server_select_mt_conf_file");
    if ($run_status != 0) {
        exit 1;
    }
}

if ($multi == 1) {
    print STDERR "\n\n*** Thread-Pool server ***\n\n\n";
    $run_status = run_server("-e 5 -ORBsvcconf $server_tp_conf_file");
    if ($run_status != 0) {
        exit 1;
    }
}

exit $status;
