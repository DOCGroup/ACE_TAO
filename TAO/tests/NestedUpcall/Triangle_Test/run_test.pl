eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$ior1file = "servera.ior";
$ior2file = "serverb.ior";

#  Make sure these aren't hanging around
unlink $ior1file;
unlink $ior2file;

$SV1 = Process::Create ($EXEPREFIX."server_A".$Process::EXE_EXT,
                        "-o $ior1file");

if (ACE::waitforfile_timed ($ior1file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$SV2 = Process::Create ($EXEPREFIX."server_B".$Process::EXE_EXT,
                        "-o $ior2file");

if (ACE::waitforfile_timed ($ior2file, 5) == -1) {
    print STDERR "ERROR: cannot find file <$ior1file>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$CL = Process::Create ($EXEPREFIX."initiator".$Process::EXE_EXT,
                       " -f $ior1file -g $ior2file");

$client = $CL->TimedWait (60);
if ($client == -1) {
    print STDERR "ERROR: client timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
}

$SV1->Terminate (); $server1 = $SV1->TimedWait (5);
if ($server1 == -1) {
    print STDERR "ERROR: cannot terminate server1\n";
    $SV1->Kill (); $SV1->TimedWait (1);
}

$SV2->Terminate (); $server1 = $SV2->TimedWait (5);
if ($server2 == -1) {
    print STDERR "ERROR: cannot terminate server2\n";
    $SV2->Kill (); $SV2->TimedWait (1);
}

# Clean up
unlink $ior1file;
unlink $ior2file;

if ($client == -1 || $server1 == -1 || $server2 == -1) {
    exit 1;
}

exit 0;
