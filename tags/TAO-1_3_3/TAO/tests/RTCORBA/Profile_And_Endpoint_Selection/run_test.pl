eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;
use English;

$status = 0;
$iorfile = "ior_2";
$client_debuglevel = 0;
$server_debuglevel = 0;
$client_debug = 0;
$server_debug = 0;
$iiop = 1;
$shmiop = 1;

$extra_client_args = "-k file://$iorfile -ORBdebuglevel $client_debuglevel -d $client_debug -x";
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

    $CL = new PerlACE::Process ("client", "$client_args");

    $CL->Spawn ();

    $client = $CL->WaitKill (120);

    if ($client != 0)
    {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server
{
    my $server_args = "@_" . " $extra_server_args";

    print "server $server_args\n";

    $SV = new PerlACE::Process ("server", "$server_args");
    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, 10) == -1)
    {
        check_supported_priorities ($SV);
	print STDERR "ERROR: cannot find ior file: $iorfile\n";
	$status = 1;
	zap_server (1);
    }
}

sub zap_server
{
    $server = $SV->WaitKill (5);

    if ($server != 0)
    {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    unlink $iorfile;

    if ($_[0])
    {
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

for $test (@configurations)
{
    my $clients = $test->{clients};

    for $args (@$clients)
    {
        print STDERR "\n******************************************************\n\n";

        unlink $iorfile;

        run_server ($test->{server});

        run_client ($args);

        zap_server (0);
    }
}

unlink "ior_1";

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
