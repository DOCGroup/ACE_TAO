eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$status = 0;

$server_reverse_conf_base = "server_reverse$PerlACE::svcconf_ext";
$server_reverse_conf = $server->LocalFile ($server_reverse_conf_base);

$server_iiop_uiop_conf_base = "server_iiop_uiop$PerlACE::svcconf_ext";
$server_iiop_uiop_conf = $server->LocalFile ($server_iiop_uiop_conf_base);

# Copy configuration files to the target.
if ($server->PutFile ($server_reverse_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$server_reverse_conf>\n";
    exit 1;
}
if ($server->PutFile ($server_iiop_uiop_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$server_iiop_uiop_conf>\n";
    exit 1;
}

# Configurations for all tests to be run.
@server_opts =
    ("-ORBSndSock 54321 -ORBendpoint iiop://",

     "-ORBsvcconf $server_reverse_conf "
     ."-ORBEndpoint shmiop:// -ORBEndpoint uiop:// -ORBendpoint iiop://",

     "-ORBRcvSock 12345 -ORBsvcconf $server_iiop_uiop_conf "
     ."-ORBEndpoint iiop:// -ORBEndpoint uiop:// "
     ."-p 1413566208");

@comments = ("* ORB Default Server Protocol Policy Test\n          "
             ."(TAO's default behavior without config files): \n",

             "* ORB Default Server Protocol Policy Test\n          "
             ."(All TAO protocols are loaded, but in reverse order): \n",

             "* Overriding ORB Default Server Protocol Policy in the POA\n"
             ."          (POA Server Protocol set to UIOP only): \n");



# UIOP only available on Unix.  Substitute with alternative tests on Windows.
if ($^O eq "MSWin32" || $^O eq "VMS") {
    $server_iiop_shmiop_conf_base = "server_iiop_shmiop$PerlACE::svcconf_ext";
    $server_iiop_shmiop_conf = $server->LocalFile ($server_iiop_shmiop_conf_base);

    $server_reverse_nt_conf_base = "server_reverse_nt$PerlACE::svcconf_ext";
    $server_reverse_nt_conf  = $server->LocalFile ($server_reverse_nt_conf_base);

    # Copy the configuration files
    if ($server->PutFile ($server_iiop_shmiop_conf_base) == -1) {
        print STDERR "ERROR: cannot set file <$server_iiop_shmiop_conf>\n";
        exit 1;
    }
    if ($server->PutFile ($server_reverse_nt_conf_base) == -1) {
        print STDERR "ERROR: cannot set file <$server_reverse_nt_conf>\n";
        exit 1;
    }

    @server_opts =
        ("-ORBSndSock 54321 -ORBendpoint iiop://",

         "-ORBsvcconf $server_reverse_nt_conf "
         ."-ORBEndpoint shmiop:// -ORBendpoint iiop://",

         "-ORBRcvSock 12345 -ORBsvcconf $server_iiop_shmiop_conf "
        ."-ORBEndpoint iiop:// -ORBEndpoint shmiop:// "
        ."-p 1413566210");

    $comments[2] =
         "* Overriding ORB Default Server Protocol Policy in the POA\n"
         ."          (POA Server Protocol set to SMHIOP only): \n";

}

print STDERR "********** RTCORBA Server Protocol Policy Unit Test\n";

$test_number = 0;

$SV = $server->CreateProcess ("server");

$CL = $client->CreateProcess ("client");

foreach $o (@server_opts) {
    print STDERR "\n\n----------------------------------\n";
    print STDERR "        ".$comments[$test_number];

    my $args = "$o -o $server_iorfile";

    $server->DeleteFile ($iorbase);

    $SV->Arguments ($args);

    $SV->Spawn ();

    if ($server->WaitForFileTimed ($iorbase,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    $CL->Arguments ("-k file://$client_iorfile -ORBdebuglevel 1");

    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $test_number++;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
