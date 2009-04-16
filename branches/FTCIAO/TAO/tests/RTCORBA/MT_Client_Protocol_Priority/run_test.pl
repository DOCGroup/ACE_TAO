eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$status = 0;
$quiet = 0;

# check for -q flag
if ($#ARGV >= 0 && $ARGV[0] eq '-q') {
    $quiet = 1;
}

# Test parameters.

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$database = "test_run.data";
$data_file = $server->LocalFile ($database);
$server->DeleteFile ($database);

$debug_level = 1;
$iterations = 50;

if ($^O eq "MSWin32") {
    $priority1 = 2;
    $priority2 = 1;
}
elsif ($^O eq "dec_osf") {
    $priority1 = 20;
    $priority2 = 25;
}
elsif ($^O eq "hpux") {
    $priority1 = 17;
    $priority2 = 22;
}
elsif ($^O eq "irix") {
    $priority1 = 27;
    $priority2 = 33;
}
else {
    $priority1 = 45;
    $priority2 = 50;
}

$server_args =
    "-ORBdebuglevel $debug_level "
    ."-ORBendpoint iiop:// "
    .(PerlACE::is_vxworks_test() ? "" : "-ORBendpoint shmiop:// ");

$client_args =
    "-o file://$client_iorfile  "
    ."-a $priority1 -b $priority2 -e 1413566210 -f 0 -n $iterations";

$SV = $server->CreateProcess ("server", "-o $server_iorfile $server_args");

$CL = $client->CreateProcess ("client", $client_args);

print STDERR "\n********** MT Client Protocol & CLIENT_PROPAGATED combo Test\n\n";

# Redirect the output of the test run to a file, so that we can process it later.

open (OLDOUT, ">&STDOUT");
open (STDOUT, ">$data_file") or die "can't redirect stdout: $!";
open (OLDERR, ">&STDERR");
open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";

# just here to quiet warnings
$fh = \*OLDOUT;
$fh = \*OLDERR;

# Run server and client.
$SV->Spawn ();


if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    $server_status = $SV->TimedWait (1);
    if ($server_status == 2) {
        # Could not change priority levels so exit.

        # redirect STDOUT away from $data_file and set back to normal
        close (STDERR);
        close (STDOUT);
        open (STDOUT, ">&OLDOUT");
        open (STDERR, ">&OLDERR");

        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

close (STDERR);
close (STDOUT);
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

# Run a processing script on the test output.
print STDERR "\n********** Processing test output\n\n";

$errors = system ("perl process-output.pl $data_file $iterations $priority1 $priority2") >> 8;

if ($errors > 0) {
    $status = 1;

    if (!$quiet) {
        print STDERR "Errors Detected, printing output\n";
        if (open (DATA, "<$data_file")) {
            print STDERR "================================= Begin\n";
            print STDERR <DATA>;
            print STDERR "================================= End\n";
            close (DATA);
        }
        else {
            print STDERR "ERROR: Could not open $data_file\n";
        }
    }
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$server->DeleteFile($database);

# Clean up shmiop files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
