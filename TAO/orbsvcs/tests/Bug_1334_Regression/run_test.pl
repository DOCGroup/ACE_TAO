
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# The NameService IOR file
#
$nsiorfile = "ns.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);

# The client process
#
$CLI = $ns->CreateProcess ("client");

# Fire up the Name Service
#
$NS = $ns->CreateProcess ("../../Naming_Service/Naming_Service",
                          "-ORBDebugLevel $debug_level ".
                          "-o $ns_nsiorfile");
$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}


# Try the client with the name service ior in an environment variable of
# the form <service name>IOR.
# Expect it to succeed.
#
$ENV {'NameServiceIOR'} = "file://$ns_nsiorfile";
$CLI->Arguments("");
$CLI_status = $CLI->SpawnWaitKill ($ns->ProcessStartWaitInterval()+15);
if ($CLI_status != 0) {
    print STDERR "ERROR: Name Service not resolved by environment variable\n";
    $NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
    if ($NS_status != 0) {
        print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    }
    $ns->DeleteFile ($nsiorfile);
    exit 1;
}

# Now try it with a duff ior in the environment variable.
# Expect it to fail.
#
$ENV {'NameServiceIOR'} = "Banana";
$CLI->Arguments("");
$CLI_status = $CLI->SpawnWaitKill ($ns->ProcessStartWaitInterval()+15);
if ($CLI_status != 1) {
    print STDERR "ERROR: Name Service resolved with duff environment\n";
    $NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
    if ($NS_status != 0) {
        print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    }
    $ns->DeleteFile ($nsiorfile);
    exit 1;
}

# Now try with a duff ior in the environment variable but overridden by
# a command line parameter.
# Expect it to succeed.
#
$ENV {'NameServiceIOR'} = "Custard";
$CLI->Arguments("-ORBInitRef NameService=file://$ns_nsiorfile");
$CLI_status = $CLI->SpawnWaitKill ($ns->ProcessStartWaitInterval()+15);
if ($CLI_status != 0) {
    print STDERR "ERROR: Name Service not resolved by command line override\n";
    $NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
    if ($NS_status != 0) {
        print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    }
    $ns->DeleteFile ($nsiorfile);
    exit 1;
}

# Clean up and return
#
$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Closing Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);

exit $status;
