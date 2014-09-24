eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $mapping1 = "mapping_file1.dat";
my $server_mapping1 = $server->LocalFile ($mapping1);
my $mapping2 = "mapping_file2.dat";
my $server_mapping2 = $server->LocalFile ($mapping2);
my $svc_conf1 = "orb1_svc.conf";
my $server_svc_conf1 = $server->LocalFile ($svc_conf1);
my $svc_conf2 = "orb2_svc.conf";
my $server_svc_conf2 = $server->LocalFile ($svc_conf2);
$server->DeleteFile($mapping1);
$server->DeleteFile($mapping2);

# copy the configuation files
if ($server->PutFile ($svc_conf1) == -1) {
    print STDERR "ERROR: cannot set file <$server_svc_conf1>\n";
    return 1;
}
if ($server->PutFile ($svc_conf2) == -1) {
    print STDERR "ERROR: cannot set file <$server_svc_conf2>\n";
    return 1;
}

$SV = $server->CreateProcess ("server",
    "-a \"AAA -ORBdebuglevel $debug_level -ORBSvcConf $server_svc_conf1\" " .
    "-b \"BBB -ORBdebuglevel $debug_level -ORBGestalt LOCAL -ORBSvcConf $server_svc_conf2\" " .
    "-y $server_mapping1 " .
    "-z $server_mapping2");
$server_status = $SV->SpawnWaitKill ($server->ProcessStartWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->GetFile ($mapping1) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_mapping1>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($mapping2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_mapping2>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if (-s $mapping1 == -s $mapping2) {
    print STDERR "ERROR: <$server_mapping1> and <$server_mapping2> are identical\n";
    $status = 1;
}

$server->DeleteFile($mapping1);
$server->DeleteFile($mapping2);

exit $status;
