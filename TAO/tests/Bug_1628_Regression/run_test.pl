

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
my $tao_idl = "$ENV{ACE_ROOT}/bin/tao_idl";
if (exists $ENV{HOST_ROOT}) {
    $tao_idl = "$ENV{HOST_ROOT}/bin/tao_idl";
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# Generated code file names
$TestCaseC_i = "TestCaseC.i";

$server_TestCaseC_i = $server->LocalFile($TestCaseC_i);
$server->DeleteFile($TestCaseC_i);

$input_file = $server->LocalFile ("TestCase.idl");

# Compile the IDL
#

$SV = $server->CreateProcess ("$tao_idl", "-Ge 1 -Sc $input_file");

$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

if ($server->WaitForFileTimed ($TestCaseC_i,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_TestCaseC_i\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

# Test for the presence of Body_dup etc. Crude test assumes that if
# Body_dup is there, then all the Body_* methods are also there.
#
$found = 0 ;
open (I_FILE, "$server_TestCaseC_i") ;
while( <I_FILE> ){
   chomp ;
   if( $_ =~ /Body_dup/ ){
       $found = 1 ;
       last ;
   }
}
if( $found != 1 ){
    print STDERR "ERROR: Body_* methods not found\n" ;
    $status = 1 ;
}

$server->DeleteFile($TestCaseC_i);


exit $status;
