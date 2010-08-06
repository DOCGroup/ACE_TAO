eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
$iorfilebase = "test.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");

unlink $iorfile;
$status = 0;

$server_reverse_conf     = PerlACE::LocalFile ("server_reverse$PerlACE::svcconf_ext");
$server_iiop_conf        = PerlACE::LocalFile ("server_iiop$PerlACE::svcconf_ext");
$server_uiop_conf        = PerlACE::LocalFile ("server_uiop$PerlACE::svcconf_ext");
$server_shmiop_conf      = PerlACE::LocalFile ("server_shmiop$PerlACE::svcconf_ext");
$server_iiop_uiop_conf   = PerlACE::LocalFile ("server_iiop_uiop$PerlACE::svcconf_ext");
$server_iiop_shmiop_conf = PerlACE::LocalFile ("server_iiop_shmiop$PerlACE::svcconf_ext");
$server_reverse_nt_conf  = PerlACE::LocalFile ("server_reverse_nt$PerlACE::svcconf_ext");

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

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server");
}
else {
    $SV = new PerlACE::Process ("server");
}
$CL = new PerlACE::Process ("client");

foreach $o (@server_opts) {
    print STDERR "\n\n----------------------------------\n";
    print STDERR "        ".$comments[$test_number];

    if (PerlACE::is_vxworks_test()) {
      my $args = "$o -o $iorfilebase";
    }
    else {
      my $args = "$o -o $iorfile";
    }

    unlink $iorfile;

    $SV->Arguments ($args);

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    $CL->Arguments ("-k file://$iorfile -ORBdebuglevel 1");

    $client = $CL->SpawnWaitKill (60);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
    }

    $server = $SV->WaitKill (60);

    if ($server != 0) {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    $test_number++;
}

unlink $iorfile;

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
