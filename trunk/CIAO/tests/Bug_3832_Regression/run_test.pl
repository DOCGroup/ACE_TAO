# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

$DANCE_ROOT = "$ENV{'DANCE_ROOT'}";

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status =0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# Generated code file names
my $TestCaseC_i = "test.log";

$server_TestCaseC_i = $server->LocalFile($TestCaseC_i);
$server->DeleteFile($TestCaseC_i);

$ENV {'CIAO_LOG_LEVEL'} = 10;
$ENV {'CIAO_LOG_FILE'} = 'test.log';

my $node_app = $server->GetArchDir("$DANCE_ROOT/bin/") . "dance_locality_manager";

$SV = $server->CreateProcess ("$node_app", "-u c7a07968-7ece-11df-8000-67c6697351ff");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: dance_locality_manager returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($TestCaseC_i,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_TestCaseC_i\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->Kill ($server->ProcessStopWaitInterval());

# Test for the presence of LM_ERROR
$found = 0 ;
open (I_FILE, "$server_TestCaseC_i") ;
while( <I_FILE> ){
   chomp ;
   if( $_ =~ /LM_ERROR/ ){
       $found = 1 ;
       last ;
   }
}

if( $found != 1 ){
  print STDERR "ERROR: No logging found\n";
  $status = 1 ;
} else {
  print STDERR "Found logging\n";
}

$server->DeleteFile($TestCaseC_i);

exit $status;
