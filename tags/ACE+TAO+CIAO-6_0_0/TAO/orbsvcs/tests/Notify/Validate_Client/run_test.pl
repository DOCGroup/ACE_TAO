eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$conf = $PerlACE::svcconf_ext;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$iorbase = "notify.ior";
$notify_conf = "notify" . $conf;

my $server_iorfile = $server->LocalFile ($iorbase);

my $persistent_file_xml = "persistency.notif.xml";
my $persistent_file_000 = "persistency.notif.000";

my $server_conf = $server->LocalFile ($notify_conf);

my $server_persistent_file = $server->LocalFile ($persistent_file_xml);
my $persistent_file = $client->LocalFile ($persistent_file_xml);

$server->DeleteFile($iorbase);
$client->DeleteFile ($persistent_file_xml);
$client->DeleteFile ($persistent_file_000);

$SV = $server->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                              "-ORBdebuglevel $debug_level " .
                              "-NoNameSvc " .
                              "-IORoutput $server_iorfile " .
                              "-ORBSvcConf $server_conf");

$CL = $client->CreateProcess ("proxy_dummy", "-o $persistent_file");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

if ($client->WaitForFileTimed ($persistent_file_xml,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$persistent_file_xml>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

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

sleep($server->ProcessStartWaitInterval / 2);

$found = 0 ;
open (I_FILE, "$persistent_file") ;
while( <I_FILE> ){

   chomp ;
   if( $_ =~ /proxy/ ){
       $found = 1 ;
       last ;
   }
}
if( $found == 1 ){
    print STDERR "ERROR: validate consumer/supplier test failed.\n" ;
    $status = 1 ;
}
else{
    print STDOUT "validate consumer/supplier test succeeded.\n";
}

$SV->Kill ();
$CL->Kill ();

$server->DeleteFile($iorbase);
$client->DeleteFile ($persistent_file_xml);
$client->DeleteFile ($persistent_file_000);

exit $status;
