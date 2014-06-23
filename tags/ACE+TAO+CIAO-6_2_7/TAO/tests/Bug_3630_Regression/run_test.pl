eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $srvconf = "TAO Conf File.conf";
my $srvconf2 = "TAO_Conf_File.conf";
my $server_svcconf = $server->LocalFile ($srvconf);
my $server_svcconf2 = $server->LocalFile ($srvconf2);

if ($server->PutFile ($srvconf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcconf>\n";
    exit 1;
}

if ($server->PutFile ($srvconf2) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcconf2>\n";
    exit 1;
}

my $SV = $server->CreateProcess ("server");

my $server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStartWaitInterval());
my $status = 0;
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
