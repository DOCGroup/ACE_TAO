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

$notify_conf = "notify.conf";
if ($opt_n) {
    $notify_conf = $opt_n;
}

$high_path_conf = "high_path.conf";
if ($opt_s) {
    $high_path_conf = $opt_s;
}

$other_paths_conf = "other_paths.conf";
if ($opt_c) {
    $other_paths_conf = $opt_c;
}

my $naming_ior    = "naming.ior";
my $high_path_ior = "high_path.ior";

$test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

#uncomment it when test will be fixed
#@list = glob("*.dat");
#for $file (@list) {
#    $test->DeleteFile ($file);
#}

my $test_op_conf = $test->LocalFile ($other_paths_conf);
my $test_nm_ior  = $test->LocalFile ($naming_ior);
my $test_hp_ior  = $test->LocalFile ($high_path_ior);
$test->DeleteFile ($naming_ior);
$test->DeleteFile ($high_path_ior);

$T = $test->CreateProcess("../../../Driver/Notify_Tests_Driver",
                          "-ORBInitRef NameService=file://$test_nm_ior ".
                          "-IORinput file://$test_hp_ior ".
                          "-ORBSvcConf $test_op_conf -ORBdebuglevel $debug_level");

if ($test->WaitForFileTimed ($high_path_ior,
                             $test->ProcessStartWaitInterval() + 105) == -1) {
    print STDERR "ERROR: cannot find file <$test_hp_ior> to start\n";
    $T->Kill (); $T->TimedWait (1);
    exit 1;
}

$test->DeleteFile($other_paths_ior);

$status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 105);

if ($status != 0) {
    print STDERR "ERROR: Other_paths returned $status\n";
    exit 1;
}

if ($opt_o) {
    print STDERR "Results are not saved in $opt_o\n";
}

$test->DeleteFile($other_paths_ior);
$test->DeleteFile($naming_ior);
$test->DeleteFile($high_path_ior);

exit $status;
