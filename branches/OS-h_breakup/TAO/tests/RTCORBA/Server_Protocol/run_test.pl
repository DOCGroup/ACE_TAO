eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;
$status = 0;

$server_reverse_conf    = PerlACE::LocalFile ("server_reverse.conf");
$server_iiop_conf       = PerlACE::LocalFile ("server_iiop.conf");
$server_uiop_conf       = PerlACE::LocalFile ("server_uiop.conf");
$server_shmiop_conf     = PerlACE::LocalFile ("server_shmiop.conf");
$server_reverse_nt_conf = PerlACE::LocalFile ("server_reverse_nt.conf");

# Configurations for all tests to be run.
@server_opts =
    ("-ORBendpoint iiop://",

     "-ORBsvcconf $server_reverse_conf "
     ."-ORBEndpoint uiop:// -ORBendpoint shmiop:// -ORBendpoint iiop://",

     "-ORBsvcconf $server_shmiop_conf -ORBendpoint shmiop://",

     "-ORBEndpoint uiop:// "
     ." -ORBsvcconf $server_uiop_conf -p 1413566208",

     " -ORBendpoint shmiop:// "
     ." -ORBsvcconf $server_shmiop_conf -p 1413566210");

@comments = ("* ORB Default Server Protocol Policy Test\n          "
             ."(TAO's default behavior without config files): \n",

             "* ORB Default Server Protocol Policy Test\n          "
             ."(All TAO protocols are loaded, but in reverse order): \n",

             "* ORB Default Server Protocol Policy Test\n          "
            ."(Only SHMIOP loaded): \n",

             "* Overriding ORB Default Server Protocol Policy in the POA\n"
             ."          (POA Server Protocol set to UIOP only): \n",

             "* Overriding ORB Default Server Protocol Policy in the POA\n"
             ."          (POA Server Protocol set to SHMIOP only): \n");

# UIOP only available on Unix.  Substitute with alternative tests on Windows.
if ($^O eq "MSWin32") {
    @server_opts =
        ("-ORBendpoint iiop://",

         "-ORBsvcconf $server_reverse_nt_conf "
         ."-ORBEndpoint shmiop:// -ORBendpoint iiop://",

         "-ORBsvcconf $server_shmiop_conf -ORBendpoint shmiop://",

         "-ORBendpoint iiop://"
         ." -ORBsvcconf $server_iiop_conf -p 0",

         "-ORBendpoint shmiop:// "
         ." -ORBsvcconf $server_shmiop_conf -p 1413566210");

    $comments[3] =
         "* Overriding ORB Default Server Protocol Policy in the POA\n"
         ."          (POA Server Protocol set to IIOP only): \n";

}

print STDERR "********** RTCORBA Server Protocol Policy Unit Test\n";

$test_number = 0;

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client");


foreach $o (@server_opts) {
    print STDERR "\n\n----------------------------------\n";
    print STDERR "        ".$comments[$test_number];

    my $args = "$o -o $iorfile";

    unlink $iorfile;

    $SV->Arguments ($args);

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
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

    $server = $SV->TerminateWaitKill (60);

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
