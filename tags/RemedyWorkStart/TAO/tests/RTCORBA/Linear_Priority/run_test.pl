eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$status = 0;

@configurations = ({
         server => "-b empty_file -l empty_file",
         client => "-b empty_file -p empty_file -x",}, {
         server => "-b bands -l empty_file",
         client => "-b empty_file -p empty_file -x",}, {
         server => "-b empty_file -l lanes",
         client => "-b empty_file -p empty_file -x",}, {
         server => "-b bands -l lanes",
         client => "-b empty_file -p empty_file -x",}, {
         server => "-b empty_file -l empty_file",
         client => "-b bands -p empty_file -x", }, {
         server => "-b empty_file -l lanes",
         client => "-b bands -p empty_file -x", }, {
         server => "-b empty_file -l empty_file",
         client => "-b empty_file -p invocation_priorities -x",}, {
         server => "-b bands -l empty_file",
         client => "-b empty_file -p invocation_priorities -x",}, {
         server => "-b bands -l lanes",
         client => "-b empty_file -p invocation_priorities -x",}, {
         server => "-b empty_file -l empty_file",
         client => "-b bands -p invocation_priorities -x",}, {
         server => "-b empty_file -l lanes",
         client => "-b bands -p invocation_priorities -x",}, );

sub run_client
{
    my $arg = shift;
    $CL = $client->CreateProcess ("client", "-k file://$client_iorfile " . $arg);

    $CL->Spawn ();

    $client_status = $CL->WaitKill ($client->ProcessStopWaitInterval ());

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server
{
    $SV = $server->CreateProcess ("server", @_);
    if ($SV->Spawn () == -1) {
        exit 1;
    }

    if ($server->WaitForFileTimed ($iorbase,
                  $server->ProcessStartWaitInterval()) == -1) {
        check_supported_priorities ($SV);
        print STDERR "ERROR: cannot find ior file: $server_iorfile\n";
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

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);

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
    print STDERR "\n******************************************************\n";

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);

    run_server ($test->{server});

    run_client ($test->{client});

    zap_server (0);
}

exit $status;
