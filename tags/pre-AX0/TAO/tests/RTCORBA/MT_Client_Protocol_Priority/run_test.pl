eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$quiet = 0;

# check for -q flag
if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

# Test parameters.
$iorfile = PerlACE::LocalFile ("test.ior");
$data_file = PerlACE::LocalFile ("test_run.data");

$debug_level = 1;
$iterations = 50;
$priority1 = 65;
$priority2 = 70;

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

# Clean up leftovers from previous runs.
unlink $iorfile;
unlink $data_file;

$server_conf = PerlACE::LocalFile ("server.conf");

$server_args =
    "-o $iorfile -ORBdebuglevel $debug_level -ORBsvcconf $server_conf "
    ."-ORBendpoint iiop:// -ORBendpoint shmiop:// ";

$client_args =
    "-o file://$iorfile  "
    ."-a $priority1 -b $priority2 -e 1413566210 -f 0 -n $iterations";

$SV = new PerlACE::Process ("server", $server_args);
$CL = new PerlACE::Process ("client", $client_args);

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

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

close (STDERR);
close (STDOUT);
open (STDOUT, ">&OLDOUT");
open (STDERR, ">&OLDERR");

unlink $iorfile;

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

unlink $iorfile;
unlink $data_file;

# Clean up shmiop files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
