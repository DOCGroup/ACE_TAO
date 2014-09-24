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
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 4 failed\n";

my $iorbase = "ior";

@iors = ($iorbase . "_1",
         $iorbase . "_2",
         $iorbase . "_3",
         $iorbase . "_4",
         $iorbase . "_5",
         $iorbase . "_6");


my $server_iorfile1 = $server->LocalFile ($iors[0]);
my $server_iorfile2 = $server->LocalFile ($iors[1]);
my $server_iorfile3 = $server->LocalFile ($iors[2]);
my $server_iorfile4 = $server->LocalFile ($iors[3]);
my $server_iorfile5 = $server->LocalFile ($iors[4]);
my $server_iorfile6 = $server->LocalFile ($iors[5]);

my $client1_iorfile1 = $client1->LocalFile ($iors[0]);
my $client1_iorfile2 = $client1->LocalFile ($iors[1]);
my $client1_iorfile3 = $client1->LocalFile ($iors[2]);
my $client1_iorfile4 = $client1->LocalFile ($iors[3]);

my $client2_iorfile5 = $client2->LocalFile ($iors[4]);
my $client2_iorfile6 = $client2->LocalFile ($iors[5]);

# Unlink all ior files
foreach $i (@iors){
    $server->DeleteFile ($i);
    if ($i == 'ior_5' or $i == 'ior_6'){
        $client2->DeleteFile ($i);
    }
    else{
        $client1->DeleteFile ($i);
    }
}

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level " .
                              "-a $server_iorfile1 " .
                              "-b $server_iorfile2 " .
                              "-c $server_iorfile3 " .
                              "-d $server_iorfile4 " .
                              "-e $server_iorfile5 " .
                              "-f $server_iorfile6");

$CL1 = $client1->CreateProcess ("client", "-ORBdebuglevel $debug_level " .
                              "-a $client1_iorfile1 " .
                              "-b $client1_iorfile2 " .
                              "-c $client1_iorfile3 " .
                              "-d $client1_iorfile4");

$CL2 = $client2->CreateProcess ("client", "-ORBdebuglevel $debug_level " .
                              "-e $client2_iorfile5 " .
                              "-f $client2_iorfile6");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

# In this example all the files are written out at the same time. So  make a
# check only for the first file

if ($server->WaitForFileTimed ($iors[5],
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile6>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

foreach $i (@iors){
    if ($server->GetFile ($i) == -1) {
        print STDERR "ERROR: cannot retrieve file <$i>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    if ($i == 'ior_5' or $i == 'ior_6'){
        if ($client2->PutFile ($i) == -1) {
            print STDERR "ERROR: cannot set file <$i>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }

    }
    else{
        if ($client1->PutFile ($i) == -1) {
            print STDERR "ERROR: cannot set file <$i>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }
    }
}

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

# Unlink all ior files
foreach $i (@iors){
    $server->DeleteFile ($i);
    if ($i == 'ior_5' or $i == 'ior_6'){
        $client2->DeleteFile ($i);
    }
    else{
        $client1->DeleteFile ($i);
    }
}

exit $status;
