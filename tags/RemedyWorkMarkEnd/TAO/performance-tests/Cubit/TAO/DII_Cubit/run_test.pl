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

my $iorbase = "cubit.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$client_flags = "";
$server_flags = "";

###############################################################################
# Parse the arguments

for (my $i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print "run_test [-h] [-n num] [-debug] [-orblite]\n";
        print "\n";
        print "-h                  -- prints this information\n";
        print "-n num              -- client uses <num> iterations\n";
        print "-debug              -- sets the debug flag for both client and server\n";
        print "-orblite            -- Use the lite version of the orb";
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
    elsif ($ARGV[$i] eq "-orblite") {
        $client_flags .= " -ORBgioplite ";
        $server_flags .= " -ORBgioplite ";
    }
    else {
        print STDERR "ERROR: Unknown Option: ".$ARGV[$i]."\n";
    }
}

###############################################################################
$SV = $server->CreateProcess ("../IDL_Cubit/server", "-ORBdebuglevel $debug_level ".
                                                     "-f $server_iorfile $server_flags");
$CL = $client->CreateProcess ("client", "-f $client_iorfile $client_flags -x");

if (! (-x $SV->Executable () && -x $CL->Executable)) {
    print STDERR "ERROR: server and/or client missing or not executable!\n";
    exit 1;
}

# Make sure the file is gone, so we can wait on it.
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

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
