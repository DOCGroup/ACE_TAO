eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

# Do not use environment variables here since not all platforms use ACE_ROOT
use lib "../../../../../bin";
use PerlACE::Run_Test;
use English;

$status = 0;

$iorfile = PerlACE::LocalFile ("cubit.ior");
$iiop_lite_conf = PerlACE::LocalFile ("iiop_lite$PerlACE::svcconf_ext");
$uiop_lite_conf = PerlACE::LocalFile ("uiop_lite$PerlACE::svcconf_ext");
$server_shmiop_conf = PerlACE::LocalFile ("server_shmiop$PerlACE::svcconf_ext");

$svnsflags = " -f $iorfile";
$clnsflags = " -f $iorfile";
$clflags = "";
$svflags = "";
$quietflag = " -q ";
$giopliteflag = 0;

###############################################################################
# Parse the arguments

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-h] [-n num] [-debug] [-verbose]\n";
        print "\n";
        print "-h                  -- prints this information\n";
        print "-n num              -- client uses <num> iterations\n";
        print "-debug              -- sets the debug flag for both client and "
                                      . "server\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-debug") {
        $clflags .= " -d ";
        $svflags .= " -d ";
    }
    elsif ($ARGV[$i] eq "-n") {
        $clflags .= " -n $ARGV[$i + 1] ";
        $i++;
    }
    elsif ($ARGV[$i] eq "-verbose") {
        $quietflag = "";
    }
    else {
        print STDERR "ERROR: Unknown Option: ".$ARGV[$i]."\n";
    }
}

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client");

###############################################################################
# run_test_helper

sub run_test_helper ()
{
    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill ();
        exit 1;
    }

    my $client = $CL->SpawnWaitKill (120);
    my $server = $SV->WaitKill (30);

    unlink $iorfile;

    if ($server != 0) {
        print STDERR "ERROR: server error status $server\n";
        $status = 1;
    }

    if ($client != 0) {
        print STDERR "ERROR: client error status $client\n";
        $status = 1;
    }
}

###############################################################################

if (! (-x $SV->Executable () && -x $CL->Executable)) {
    print STDERR "ERROR: server and/or client missing or not executable!\n";
    exit 1;
}

unlink $iorfile;

print STDERR "============================================================\n";
print STDERR "Running IDL_Cubit with the default ORB protocol.\n\n";

$SV->Arguments ($svflags . $svnsflags);
$CL->Arguments ($clflags . $clnsflags . $quietflag . " -x ");

run_test_helper ();

if ($OSNAME ne "MSWin32") {
    print STDERR "============================================================\n";
    print STDERR "Running IDL_Cubit with the UIOP protocol.\n\n";

    $SV->Arguments ($svflags . $svnsflags . " -ORBEndpoint uiop:// ");
    $CL->Arguments ($clflags . $clnsflags . $quietflag . " -x ");

    run_test_helper ();

    print STDERR "============================================================\n";
    print STDERR "Running IDL_Cubit with the SHMIOP protocol.\n\n";

    $SV->Arguments ($svflags . $svnsflags
                    . " -ORBEndpoint shmiop:// -ORBSvcconf $server_shmiop_conf ");
    $CL->Arguments ($clflags . $clnsflags . $quietflag . " -x ");

    run_test_helper ();

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

}

exit $status;
