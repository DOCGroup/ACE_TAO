eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

use strict;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# Variables for command-line arguments to client and server
# executables.
my $iorfile = PerlACE::LocalFile ("ns.ior");
my $NS = new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service");
my $CL = new PerlACE::Process ("../../orbsvcs/tests/Simple_Naming/client");
my $LS = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/nslist");
my $AD = new PerlACE::Process ("$ENV{ACE_ROOT}/bin/nsadd");
my $status = 0;

# We want the nslist and nsadd executables to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell their Process objects to ignore the setting of -ExeSubDir.

$LS->IgnoreExeSubDir (1);
$AD->IgnoreExeSubDir (1);

print STDOUT "Executable for nslist is " . $LS->Executable () . "\n";

sub name_server
{
    $NS->Arguments("-o $iorfile -m 0 @_");
    unlink $iorfile;

    my $ret = $NS->Spawn ();
    if ($ret != 0) {
        print STDERR "ERROR: NameService spawn returned: $ret\n";
        exit 1;
    }

    if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
        $NS->Kill ();
        exit 1;
    }
}

sub client
{
    $CL->Arguments("@_" . " ");
    my $ret = $CL->SpawnWaitKill (60);
    if ($ret != 0) {
        print STDERR "ERROR: client returned $ret\n";
        $status = 1;
    }
}

sub nslist
{
    $LS->Arguments("-ORBInitRef NameService=file://$iorfile @_");
    my $ret = $LS->SpawnWaitKill (60);
    if ($ret != 0) {
        print STDERR "ERROR: nslist returned $ret\n";
        $status = 1;
    }
}

sub nsadd
{
    $AD->Arguments("-ORBInitRef NameService=file://$iorfile @_");
    my $ret = $AD->SpawnWaitKill (60);
    if ($ret != 0) {
        print STDERR "ERROR: nsadd returned $ret\n";
        $status = 1;
    }
}

name_server ();

print STDOUT "nslist of starting NS content\n";
nslist ();

client ("-ORBInitRef NameService=file://$iorfile", "-t");

print STDOUT "nslist of NS content after client use\n";
nslist ();

print STDOUT "nsadd of a new context at level 2\n";
nsadd ("--newcontext --name level1_context/new_lvl2_context");

print STDOUT "nsadd of a new context at level 4\n";
nsadd ("--newcontext --name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context");

print STDOUT "nsadd of a new object at level 4\n";
nsadd ("--ior file://ns.ior --name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context/new_obj");

print STDOUT "nslist of ending NS content\n";
nslist ();

$NS->TerminateWaitKill (5);

unlink $iorfile;

exit $status;
