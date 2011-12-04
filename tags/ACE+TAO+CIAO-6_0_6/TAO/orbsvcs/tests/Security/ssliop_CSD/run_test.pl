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

my $options = "csd|ssliop|ssliop-csd";
my $ssliop_conf = "ssliop.conf";
my $csd_conf = "csd.conf";
my $ssliop_csd_conf = "ssliop-csd.conf";
my $client_conf = "client1.conf";

my $serv_opts = "-ORBSvcConf $ssliop_conf";
my $client_opts = "-ORBSvcConf $client_conf";

$args = @ARGV;
if ($args > 0) {
   if (($ARGV[0] =~ /\-h/) || ($ARGV[0] =~ /\-\?/)) {
      print $0. " $options\n";
    } elsif ($ARGV[0] =~ /ssliop\-csd/) {
      $serv_opts = "-ORBSvcConf $ssliop_csd_conf";
    } elsif ($ARGV[0] =~ /csd/) {
      $serv_opts = "-ORBSvcConf $csd_conf";
      $client_opts = "";
    } else {
      print "Using default ssliop configuration.\n";
    }
} else {
    print "Using default ssliop configuration.\n";
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$ENV{'SSL_CERT_FILE'} = "cacert.pem";
my $iorbase = "Messenger.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level -o $server_iorfile $serv_opts");
$CL = $client->CreateProcess ("MessengerClient", "-k file://$client_iorfile $client_opts");

print STDERR "\n\nSecurity Unaware Application Examples\n";
print STDERR     "-------------------------------------\n";
print STDERR     "Starting Messenger Server...\n\n";

print $SV->CommandLine(). "\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print $CL->CommandLine(). "\n";
print STDERR "\nStarting MessengerClient, example 3...\n\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$ENV{'SSL_CERT_FILE'} = "";

$server_status = $SV->Kill ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
