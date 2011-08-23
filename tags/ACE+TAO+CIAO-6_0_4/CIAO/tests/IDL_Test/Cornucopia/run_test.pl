
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::Spec;

$status =0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$input_file1 = $server->LocalFile ("Cornucopia.doxygen");

# Compile the IDL
$SV = $server->CreateProcess ("/usr/bin/doxygen", "$input_file1");

$server_status1 = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status1 != 0) {
    print STDERR "ERROR: doxygen returned $server_status1 for $input_file1, should have failed\n";
    $status = 1;
}

exit $status;
