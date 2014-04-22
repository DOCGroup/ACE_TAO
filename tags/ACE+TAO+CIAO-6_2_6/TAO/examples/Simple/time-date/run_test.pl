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
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "ior"; # please note that conf file is defined in *.conf file
my $client_conffile = $client->LocalFile ("client"."$PerlACE::svcconf_ext");
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

# Generate svc.conf file for server
my $server_conf = "svc.conf";
$server->LocalFile ($server_conf);

die "Error: Cannot create svc.conf file" if not open(FH, ">$server_conf");
print FH "# Define a special Reactor for single-threaded configurations. " .
            " Refer to README for details. \n";
print FH "dynamic Resource_Factory Service_Object * Time_Date:_make_My_Resource_Factory()\n";
print FH "# Dynamically configure the ORB into the application process.\n";
print FH "dynamic ORB Service_Object * Time_Date:_make_DLL_ORB() \"dummy\"\n";
print FH "# Once the ORB is configured, dynamically configure the Time_Date service.\n";
print FH "dynamic Time_Date_Servant Service_Object * Time_Date:_make_Time_Date_Servant() " .
         "\"dummy -n ORB -o $server_iorfile\"\n";
close(FH);

if ($server->PutFile ($server_conf) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf\n";
    exit 1;
}

$SV = $server->CreateProcess ("server", "");
$CL = $client->CreateProcess ("client", "-f $client_iorfile -x -ORBSvcConf $client_conffile");
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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($server_conf);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
