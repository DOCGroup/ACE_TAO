# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$tao_root = $ENV{TAO_ROOT};
# This is a Perl script that runs the Naming Service, client and servers

unshift @INC, '../../../../../bin';
require Process;
require Uniqueid;

# amount of delay between running the servers

$sleeptime = 6;

# variables for parameters

sub run_test
{
    my $args = "";
    print ("\nServer: server$Process::EXE_EXT $args\n");
    $SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", $args);

    my $args = "";
    print ("\nclient: client $args\n");
    $CL = Process::Create ($EXEPREFIX."client$Process::EXE_EXT", $args);
    
    $client = $CL->TimedWait (60);
    if ($client == -1) {
        print STDERR "ERROR: client timedout\n";
        $CL->Kill (); $CL->TimedWait (1);
    }

    $server = $SV->TimedWait (2);
    if ($server == -1) {
        print STDERR "ERROR: server timedout\n";
        $SV->Kill (); $SV->TimedWait (1);
    }
    
}

run_test;
