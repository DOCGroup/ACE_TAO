eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use File::stat;

$status = 0;
$debug = 0;

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sv = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $cl = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

# amount of delay between running the servers
$sleeptime = 10;

$nsiorfile = "ns.ior";
$inputfile = "test_input";

$inputfile = PerlACE::generate_test_file($inputfile, 102400);
my $cl_inputfile = $cl->LocalFile ($inputfile);

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $sv_nsiorfile = $sv->LocalFile ($nsiorfile);
my $cl_nsiorfile = $cl->LocalFile ($nsiorfile);
$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);

if ($cl->PutFile ($inputfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_inputfile>\n";
    exit 1;
}

for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print STDERR "\nusage:  run_test\n";

        print STDERR "\t-h shows options menu\n";

        print STDERR "\t-p: Transport protocols defaults to (";
        for $protocol (@protocols) {
            print STDERR "$protocol, ";
        }
        print STDERR ")\n";

        print STDERR "\t-d: Debug Level defaults to 0";

        print STDERR "\n";

        exit;
    }
    elsif ($ARGV[$i] eq "-p") {
        @protocols = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-d") {
        $debug = $ARGV[$i + 1];
        $i++;
    }
}

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");

print STDERR "Starting Naming Service\n";
$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sv->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sv_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($cl->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cl_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$outputfile = "TCP_output";

for $protocol (@protocols) {

    $outputfile = $protocol."_output";

    if ($protocol eq "RTP_UDP") {
        $protocol = "RTP/UDP";
    }

    my $sv_outputfile = $sv->LocalFile ($outputfile);
    $ns->DeleteFile ($outputfile);

    $SV = $sv->CreateProcess ("receiver",
                              "-ORBInitRef NameService=file://$sv_nsiorfile ".
                              "-ORBDebugLevel $debug ".
                              "-f $sv_outputfile");

    $CL = $cl->CreateProcess ("sender",
                              "-ORBInitRef NameService=file://$cl_nsiorfile ".
                              "-ORBDebugLevel $debug ".
                              "-p $protocol ".
                              "-f $cl_inputfile");

    print STDERR "Using ".$protocol."\n";
    print STDERR "Starting Receiver\n";

    $SV_status = $SV->Spawn ();
    if ($SV_status != 0) {
        print STDERR "ERROR: server returned $SV_status\n";
        $SV->Kill (); $SV->TimedWait (1);
        $NS->Kill (); $NS->TimedWait (1);
        exit 1;
    }
    sleep $sleeptime;

    print STDERR "Starting Sender\n";

    $CL_status = $CL->SpawnWaitKill ($cl->ProcessStartWaitInterval()+185);
    if ($CL_status != 0) {
        print STDERR "ERROR: ftp returned $CL_status\n";
        $status = 1;
    }

    $SV_status = $SV->TerminateWaitKill ($sv->ProcessStopWaitInterval()+185);
    if ($SV_status != 0) {
        print STDERR "ERROR: server returned $SV_status\n";
        $status = 1;
    }

    $ns->DeleteFile ($outputfile);
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval()+985);
if ($NS_status != 0) {
    print STDERR "ERROR: Naming Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$sv->DeleteFile ($nsiorfile);
$cl->DeleteFile ($nsiorfile);
$cl->DeleteFile ($inputfile);

exit $status;
