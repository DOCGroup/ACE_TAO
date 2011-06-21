eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs some Naming Service tests.
# It runs all the tests that will run with min CORBA.
# It starts all the servers and clients as necessary.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $server4 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

# Variables for command-line arguments to client and server
# executables.
my $naming_ior = "ns.ior";

my $server1_naming_ior = $server1->LocalFile ($naming_ior);
my $server2_naming_ior = $server2->LocalFile ($naming_ior);
my $server3_naming_ior = $server3->LocalFile ($naming_ior);
my $server4_naming_ior = $server4->LocalFile ($naming_ior);
my $client_naming_ior = $client->LocalFile ($naming_ior);

$server1->DeleteFile ($naming_ior);
$server2->DeleteFile ($naming_ior);
$server3->DeleteFile ($naming_ior);
$server4->DeleteFile ($naming_ior);
$client->DeleteFile ($naming_ior);


$NS = $server1->CreateProcess ("../../orbsvcs/Naming_Service/tao_cosnaming", "");
$LS = $server2->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nslist", "");
$AD = $server3->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsadd", "");
$DL = $server4->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsdel", "");
$CL = $client->CreateProcess ("../../orbsvcs/tests/Simple_Naming/client", "");

# We want the nslist and nsadd executables to be found exactly in the path
# given, without being modified by the value of -ExeSubDir.
# So, we tell their Process objects to ignore the setting of -ExeSubDir.

$LS->IgnoreExeSubDir (1);
$AD->IgnoreExeSubDir (1);
$DL->IgnoreExeSubDir (1);

print STDOUT "Executable for nslist is " . $LS->Executable () . "\n";

sub name_server
{
    $NS->Arguments("-o $server1_naming_ior -m 0 @_");

    $server1->DeleteFile ($naming_ior);
    $server2->DeleteFile ($naming_ior);
    $server3->DeleteFile ($naming_ior);
    $server4->DeleteFile ($naming_ior);
    $client->DeleteFile ($naming_ior);

    my $ret = $NS->Spawn ();
    if ($ret != 0) {
        print STDERR "ERROR: NameService spawn returned: $ret\n";
        exit 1;
    }

    if ($server1->WaitForFileTimed ($naming_ior,
                               $server1->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$naming_ior>\n";
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }

    if ($server1->GetFile ($naming_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server1_naming_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($server2->PutFile ($naming_ior) == -1) {
        print STDERR "ERROR: cannot set file <$server2_naming_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($server3->PutFile ($naming_ior) == -1) {
        print STDERR "ERROR: cannot set file <$server3_naming_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($server4->PutFile ($naming_ior) == -1) {
        print STDERR "ERROR: cannot set file <$server4_naming_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($client->PutFile ($naming_ior) == -1) {
        print STDERR "ERROR: cannot set file <$client_naming_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

sub client
{
    $CL->Arguments("@_" . " ");
    my $ret = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);
    if ($ret != 0) {
        print STDERR "ERROR: client returned $ret\n";
        $status = 1;
    }
}

sub nslist
{
    $LS->Arguments("-ORBInitRef NameService=file://$server2_naming_ior @_");
    my $ret = $LS->SpawnWaitKill ($server2->ProcessStartWaitInterval() + 45);
    if ($ret != 0) {
        print STDERR "ERROR: nslist returned $ret\n";
        $status = 1;
    }
}

sub nsadd
{
    $AD->Arguments("-ORBInitRef NameService=file://$server3_naming_ior @_");
    my $ret = $AD->SpawnWaitKill ($server3->ProcessStartWaitInterval() + 45);
    if ($ret != 0) {
        print STDERR "ERROR: nsadd returned $ret\n";
        $status = 1;
    }
}

sub nsdel
{
    $DL->Arguments("-ORBInitRef NameService=file://$server4_naming_ior @_");
    my $ret = $DL->SpawnWaitKill ($server4->ProcessStartWaitInterval() + 45);
    if ($ret != 0) {
        print STDERR "ERROR: nsdel returned $ret\n";
        $status = 1;
    }
}

name_server ();

print STDOUT "nslist of starting NS content\n";
nslist ();

client ("-ORBInitRef NameService=file://$client_naming_ior", "-t");

print STDOUT "nslist of NS content after client use\n";
nslist ();

print STDOUT "nsadd of a new context at level 2\n";
nsadd ("--newcontext --name level1_context/new_lvl2_context");

print STDOUT "nsadd of a new context at level 4\n";
nsadd ("--newcontext --name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context");

print STDOUT "nsadd of a new object at level 4\n";
nsadd ("--ior file://ns.ior --name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context/new_obj");

print STDOUT "nslist after adding NS content\n";
nslist ();

print STDOUT "nsdel of the object at level 4\n";
print STDOUT "Expected to warn about possibly orphaned naming context.\n";
nsdel ("--name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context/new_obj");

print STDOUT "nsdel of the naming context new_lvl4_context using --destroy\n";
nsdel ("--name level1_context/new_lvl2_context/autoadded_lvl3/new_lvl4_context --destroy");

print STDOUT "nslist of ending NS content\n";
nslist ();

$NS->TerminateWaitKill ($server1->ProcessStopWaitInterval());

$server1->DeleteFile ($naming_ior);
$server2->DeleteFile ($naming_ior);
$server3->DeleteFile ($naming_ior);
$server4->DeleteFile ($naming_ior);
$client->DeleteFile ($naming_ior);

exit $status;
