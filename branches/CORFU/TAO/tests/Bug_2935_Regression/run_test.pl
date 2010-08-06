eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

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
$sinkiorfilebase = "sink.ior";
$sinkiorfile = PerlACE::LocalFile ("$sinkiorfilebase");
$middleiorfile = PerlACE::LocalFile ("middle.ior");
$sinklogfilebase = "sink.log";
$sinklogfile = PerlACE::LocalFile ("$sinklogfilebase");
$middlelogfile = PerlACE::LocalFile ("middle.log");
$sourcelogfile = PerlACE::LocalFile ("source.log");

unlink $sinkiorfile;
unlink $middleiorfile;
unlink $sinklogfile;
unlink $middlelogfile;
unlink $sourcelogfile;

if (PerlACE::is_vxworks_test()) {
$SV = new PerlACE::ProcessVX ("sink", "-o $sinkiorfilebase -orblogfile $sinklogfilebase -orbdebuglevel 9");
}
else {
$SV = new PerlACE::Process ("sink", "-o $sinkiorfile -orblogfile $sinklogfile  -orbdebuglevel 9");
}
$MD = new PerlACE::Process ("middle", "-o $middleiorfile -f $sinkiorfile -ORBSvcConf middle.conf -orblogfile $middlelogfile  -orbdebuglevel 9");
$CL = new PerlACE::Process ("source", "-f $middleiorfile -orblogfile $sourcelogfile  -orbdebuglevel 9");

#print $SV->CommandLine . "\n";
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($sinkiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find file <$sinkiorfile>\n";
    $SV->Kill ();
    exit 1;
}

#print $MD->CommandLine . "\n";
$MD->Spawn ();

if (PerlACE::waitforfile_timed ($middleiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find file <$middleiorfile>\n";
    $SV->Kill ();
    exit 1;
}


#print $CL->CommandLine . "\n";
$source = $CL->SpawnWaitKill (60);

if ($source != 0) {
    print STDERR "ERROR: source returned $source\n";
    $status = 1;
}

$middle = $MD->WaitKill (15);
if ($middle != 0) {
    print STDERR "ERROR: middle returned $middle\n";
    $status = 1;
}

$sink = $SV->WaitKill (15);
if ($sink != 0) {
    print STDERR "ERROR: sink returned $sink\n";
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
  unlink $sinkiorfile;
  unlink $middleiorfile;
  unlink $sinklogfile;
  unlink $middlelogfile;
  unlink $sourcelogfile;
}

exit $status;
