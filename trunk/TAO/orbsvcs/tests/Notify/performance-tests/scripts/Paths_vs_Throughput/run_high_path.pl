eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Copy;
use Getopt::Std;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# -n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h) {
    $opt_h = 0; #to disable fuxx.pl warning only
    print STDERR "-n notify.conf -s high_path.conf -c other_paths.conf -o output_dir -h\n";
    exit 0;
}

my $notify_ior    = "notify.ior";
my $naming_ior    = "naming.ior";
my $high_path_ior = "high_path.ior";

my $other_paths_conf = "other_paths.conf";
my $high_path_conf   = "high_path.conf";
my $notify_conf      = "notify.conf";

if ($opt_n) {
    $notify_conf = $opt_n;
}

if ($opt_s) {
    $high_path_conf = $opt_s;
}

if ($opt_c) {
    $other_paths_conf = $opt_c;
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $test_notify_ior   = $test->LocalFile ($notify_ior);
my $test_naming_ior   = $test->LocalFile ($naming_ior);
my $test_highpath_ior = $test->LocalFile ($high_path_ior);
$test->DeleteFile($notify_ior);
$test->DeleteFile($naming_ior);
$test->DeleteFile($high_path_ior);

my $test_highpath_conf = $test->LocalFile ($high_path_conf);

#uncomment it when test will be fixed
#@datfiles = glob("*.dat");
#for $file (@datfiles) {
#    $test->DeleteFile($file);
#}

$T = $test->CreateProcess ("../../../Driver/Notify_Tests_Driver",
                           "-ORBInitRef NameService=file://$test_naming_ior ".
                           "-IORoutput $test_highpath_ior ".
                           "-ORBSvcConf $test_highpath_conf");

if ($test->WaitForFileTimed ($notify_ior,
                             $test->ProcessStartWaitInterval() + 105) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    exit 1;
}

$test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 105);

if ($test_status != 0) {
    print STDERR "ERROR: test returned $test_status\n";
    exit 1;
}

if ($opt_o) {
    print STDERR "Results are not saved in $opt_o\n";
}

$test->DeleteFile($notify_ior);
$test->DeleteFile($naming_ior);
$test->DeleteFile($high_path_ior);

exit $status;
