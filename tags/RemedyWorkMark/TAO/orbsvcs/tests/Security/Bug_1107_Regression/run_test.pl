eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';


foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

#Config files for server and client
my $server_conf = $server->LocalFile ("server" . $PerlACE::svcconf_ext);
my $client_conf = $client->LocalFile ("client" . $PerlACE::svcconf_ext);


$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile");
$CL = $client->CreateProcess ("client",
                              "-ORBSvcConf $client_conf " .
                              "-n " .
                              "-k file://$client_iorfile");


print STDERR "\n\n==== Running Bug_1107_Regression test\n";
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

## In order to test the following, we are running the client
## without the SSL_CERT_FILE set three times:
##
## The behavior we're observing is
##
## 1. Start the server normally.
## 2. Start the client without identifying the CA certificate file.
## 3. Server rejects the connection request and the client receives
##    "tlsv1 alert, unknown ca."
## 4. Start the client normally.
## 5. Server reports two identical errors from ACE_SSL
##    ACE_SSL () error code: 336462100 - error: 140E0114:SSL routines:
##    SSL_shutdown:uninitialized
## 6. Client receives a CORBA::TRANSIENT exception.
## 7. Start the client normally.
## 8. Test completes normally.
##
## In step 4, it isn't necessary to start the client normally.  The behavior
## is the same, including the server's eventual return to normal behavior,
## even if the client is started without defining the CA certifcate file.
for(my $i = 0; $i < 3; $i++) {
    print STDERR "\nRunning the client without SSL_CERT_FILE\n";
    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
}

print STDERR "\nRunning the client with SSL_CERT_FILE\n";

my $args = $CL->Arguments();
$args =~ s/\-n\s+//;
$CL->Arguments($args);
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

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
