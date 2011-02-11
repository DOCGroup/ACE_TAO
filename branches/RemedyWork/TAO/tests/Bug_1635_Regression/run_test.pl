eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '10';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $debug_log = "debug.log";

$client_log = $client->LocalFile ($debug_log);
$client->DeleteFile ($debug_log);

$CL = $client->CreateProcess ("client", "-ORBDebugLevel $debug_level -ORBLogFile $client_log");

sub run_client{
    # Run the client
    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    # Open the log file, read a line, close, and delete it.
    open (HANDLE, "$client_log");
    $_ = <HANDLE>;
    close HANDLE;
    $client->DeleteFile ($debug_log);
}

# Run the client with no value for the environment variable
$ENV {'ACE_LOG_TIMESTAMP'} = "";
run_client;

# Check result is as expected, i.e. starts like:
# TAO (12...etc.
if (!/^TAO \(/){
    print STDERR "ERROR: Bug 1635 Regression failed. Unexpected normal log format.\n";
    $ENV {'ACE_LOG_TIMESTAMP'} = "";
    exit 1;
}

# Try again with the 'TIME' logging property set
$ENV {'ACE_LOG_TIMESTAMP'} = "TIME";
run_client;

# Check result is as expected, i.e. starts like:
# 14:36:38.222000|TAO (12...etc.
if (!/^\d{2}:\d{2}:\d{2}.\d{6}\|TAO \(/){
    print STDERR "ERROR: Bug 1635 Regression failed. Unexpected 'TIME' log format.\n";
    $ENV {'ACE_LOG_TIMESTAMP'} = "";
    exit 1;
}

# Try again with the 'DATE' logging property set
$ENV {'ACE_LOG_TIMESTAMP'} = "DATE";
run_client;

# Check result is as expected, i.e. starts like:
# Wed Feb 12 2003 14:36:38.222000|TAO (12...etc.
if (!/^[A-Z][a-z]{2} [A-Z][a-z]{2} \d{2} \d{4} \d{2}:\d{2}:\d{2}.\d{6}\|TAO \(/){
    print STDERR "ERROR: Bug 1635 Regression failed. Unexpected 'DATE' log format.\n";
    $ENV {'ACE_LOG_TIMESTAMP'} = "";
    exit 1;
}

$ENV {'ACE_LOG_TIMESTAMP'} = "";

exit 0;
