eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

sub count_matching_lines {
   my($file) = shift;
   my($pattern) = shift;
   my($count) = 0;
   open(FILE, $file);
   while(<FILE>) {
       ++$count if /$pattern/;
   }
   close FILE;
   return $count;
}

$status = 0;

my $sink = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $middle = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $source = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $sinkiorfilebase = "sink.ior";
my $sinkiorfile = $sink->LocalFile ($sinkiorfilebase);

my $middleiorfilebase = "middle.ior";
my $middleiorfile = $middle->LocalFile ($middleiorfilebase);

my $sinklogfilebase = "sink.log";
my $sinklogfile = $sink->LocalFile ($sinklogfilebase);

my $middlelogfilebase = "middle.log";
my $middlelogfile = $middle->LocalFile ($middlelogfilebase);

my $sourcelogfilebase = "source.log";
my $sourcelogfile = $source->LocalFile ($sourcelogfilebase);

$sink->DeleteFile ($sinkiorfilebase);
$sink->DeleteFile ($sinklogfilebase);

$middle->DeleteFile ($middleiorfilebase);
$middle->DeleteFile ($middlelogfilebase);

$source->DeleteFile ($sourcelogfilebase);

$SV = $sink->CreateProcess ("sink", "-o $sinkiorfile -orblogfile $sinklogfile -orbdebuglevel 9");
$MD = $middle->CreateProcess ("middle", "-o $middleiorfile -f $sinkiorfile -ORBSvcConf middle.conf -orblogfile $middlelogfile  -orbdebuglevel 9");
$CL = $source->CreateProcess ("source", "-f $middleiorfile -orblogfile $sourcelogfile  -orbdebuglevel 9");

$SV->Spawn ();

if ($sink->WaitForFileTimed ($sinkiorfilebase,
                             $sink->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find file <$sinkiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$MD->Spawn ();

if ($middle->WaitForFileTimed($middleiorfilebase,
                              $middle->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find file <$middleiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $MD->Kill (); $MD->TimedWait (1);
    exit 1;
}


$source_status = $CL->SpawnWaitKill ($source->ProcessStartWaitInterval ());

if ($source_status != 0) {
    print STDERR "ERROR: source returned $source_status\n";
    $status = 1;
}

$middle_status = $MD->WaitKill ($middle->ProcessStopWaitInterval ());
if ($middle_status != 0) {
    print STDERR "ERROR: middle returned $middle_status\n";
    $status = 1;
}

$sink_status = $SV->WaitKill ($sink->ProcessStopWaitInterval ());
if ($sink_status != 0) {
    print STDERR "ERROR: sink returned $sink_status\n";
    $status = 1;
}

if($status == 0){
    $connections = count_matching_lines($sinklogfile, "IIOP connection to peer");
    if ($connections != 1) {
        print STDERR "ERROR: Connections to sink should be 1: $connections\n";
        $status = 1;
    }
    else {
        print STDERR "OK: Found 1 connection to sink.\n";
   }
}

if($status == 0){
    $sink->DeleteFile ($sinkiorfilebase);
    $sink->DeleteFile ($sinklogfilebase);

    $middle->DeleteFile ($middleiorfilebase);
    $middle->DeleteFile ($middlelogfilebase);

    $source->DeleteFile ($sourcelogfilebase);
}

exit $status;
