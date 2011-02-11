eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use English;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "ior_2";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

my $client_debuglevel = 0;
my $server_debuglevel = 0;

my $client_debug = 0;
my $server_debug = 0;

my $iiop = 1;
my $shmiop = 1;
my $status = 0;

$extra_client_args = "-k file://$client_iorfile -ORBdebuglevel $client_debuglevel -d $client_debug -x";
$extra_server_args = "-ORBdebuglevel $server_debuglevel -d $server_debug";

if ($OSNAME eq "solaris") {
    $lanes = "lanes_r";
    $bands = "bands_r";
    $inv_prio = "invocation_priorities_r";
}
else {
    $lanes = "lanes";
    $bands = "bands";
    $inv_prio = "invocation_priorities";
}

sub setup_configurations_2
{
    my $server_args = shift;
    my $client_args = shift;
    my $iiop = shift;
    my $shmiop = shift;
    my $server_config = shift;

    if ($iiop)   { $server_args .= "-ORBendpoint iiop:// "; };
    if ($shmiop) { $server_args .= "-ORBendpoint shmiop:// "; };

                           $configurations[$server_config]->{server} = "$server_args";
                   $j = 0; $configurations[$server_config]->{clients}[$j] = "$client_args " ."-p iiop_shmiop";
                   $j++;   $configurations[$server_config]->{clients}[$j] = "$client_args " ."-p shmiop_iiop";
    if ($iiop)   { $j++;   $configurations[$server_config]->{clients}[$j] = "$client_args " ."-p iiop"; }
    if ($shmiop) { $j++;   $configurations[$server_config]->{clients}[$j] = "$client_args " ."-p shmiop"; }
}

sub setup_configurations_1
{
    my $iiop = shift;
    my $shmiop = shift;

    $server_config++;    setup_configurations_2 ("", "", $iiop, $shmiop, $server_config);
    $server_config++;    setup_configurations_2 ("", "-t $inv_prio ", $iiop, $shmiop, $server_config);
    $server_config++;    setup_configurations_2 ("", "-b $bands -t $inv_prio ", $iiop, $shmiop, $server_config);
    $server_config++;    setup_configurations_2 ("-b $bands ", "-t $inv_prio ", $iiop, $shmiop, $server_config);
    $server_config++;    setup_configurations_2 ("-l $lanes ", "-b $bands -t $inv_prio ", $iiop, $shmiop, $server_config);
    $server_config++;    setup_configurations_2 ("-b $bands -l $lanes ", "-t $inv_prio ", $iiop, $shmiop, $server_config);
}

$server_config = -1;
                        setup_configurations_1 (0, 0);
if ($iiop)            { setup_configurations_1 (1, 0); }
if ($shmiop)          { setup_configurations_1 (0, 1); }
if ($iiop && $shmiop) { setup_configurations_1 (1, 1); }

sub run_client
{
    my $client_args = "@_" . " $extra_client_args";

    print "client $client_args\n";

    $CL = $client->CreateProcess ("client", "$client_args");

    $CL->Spawn ();

    $client_status = $CL->WaitKill ($client->ProcessStopWaitInterval (120));

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server
{
    my $server_args = "@_" . " $extra_server_args";

    print "server $server_args\n";

    $SV = $server->CreateProcess ("server", "$server_args");
    $SV->Spawn ();

    if ($server->WaitForFileTimed ($iorbase,
                                   $server->ProcessStartWaitInterval()) == -1) {
        check_supported_priorities ($SV);
        print STDERR "ERROR: cannot find ior file <$server_iorfile>\n";
        $status = 1;
        zap_server (1);
    }
}

sub zap_server
{
    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile ($iorbase);

    if ($_[0]) {
        exit $status;
    }
}

sub check_supported_priorities
{
    $process = shift;
    $returnVal = $process->TimedWait (1);
    if ($returnVal == 2) {
        # Mark as no longer running to avoid errors on exit.
        $process->{RUNNING} = 0;
        exit 0;
    }
}

for $test (@configurations) {
    my $clients = $test->{clients};

    for $args (@$clients) {
        print STDERR "\n******************************************************\n\n";

        $server->DeleteFile ($iorbase);
        $client->DeleteFile ($iorbase);

        run_server ($test->{server});

        run_client ($args);

        zap_server (0);
    }
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
