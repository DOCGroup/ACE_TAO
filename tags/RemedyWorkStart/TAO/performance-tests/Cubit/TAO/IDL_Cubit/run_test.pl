eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use English;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "cubit.ior";
my $server_shmiop_conf = $server->LocalFile ("server_shmiop$PerlACE::svcconf_ext");

my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

# The first three lines above let this script run without specifying the
# full path to perl, as long as it is in the user's PATH.
# Taken from perlrun man page.

$client_flags = "";
$server_flags = "";
$quiet_flag = " -q ";
$shmiop = 1;

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
        print "-noshmiop           -- don't run SHMIOP test\n";
        exit;
    }
    elsif ($ARGV[$i] eq "-debug") {
        $client_flags .= " -d ";
        $server_flags .= " -d ";
    }
    elsif ($ARGV[$i] eq "-n") {
        $client_flags .= " -n $ARGV[$i + 1] ";
        $i++;
    }
    elsif ($ARGV[$i] eq "-verbose") {
        $quiet_flag = "";
    }
    elsif ($ARGV[$i] eq "-noshmiop") {
        $shmiop = 0;
    }
    else {
        print STDERR "ERROR: Unknown Option: ".$ARGV[$i]."\n";
    }
}

$SV = $server->CreateProcess ("server", "");
$CL = $client->CreateProcess ("client", "-f $client_iorfile $client_flags $quiet_flag -x");

###############################################################################
# run_test_helper

sub run_test_helper ()
{
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

    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 105);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 20);

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
}

###############################################################################
my $server_stdarg = "-ORBdebuglevel $debug_level -f $server_iorfile $server_flags";

if (! (-x $SV->Executable () && -x $CL->Executable)) {
    print STDERR "ERROR: server and/or client missing or not executable!\n";
    exit 1;
}

print STDERR "============================================================\n";
print STDERR "Running IDL_Cubit with the default ORB protocol.\n\n";

$SV->Arguments ($server_stdarg);

run_test_helper ();

if ($OSNAME ne "MSWin32" && $OSNAME ne "VMS") {
    print STDERR "============================================================\n";
    print STDERR "Running IDL_Cubit with the UIOP protocol.\n\n";

    $SV->Arguments ("$server_stdarg -ORBEndpoint uiop:// ");

    run_test_helper ();
}

if ($shmiop) {
    print STDERR "============================================================\n";
    print STDERR "Running IDL_Cubit with the SHMIOP protocol.\n\n";

    $SV->Arguments ("$server_stdarg -ORBEndpoint shmiop:// -ORBSvcconf $server_shmiop_conf ");

    run_test_helper ();
}

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
