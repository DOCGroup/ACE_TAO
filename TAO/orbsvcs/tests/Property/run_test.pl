eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

$tao_root = $ENV{TAO_ROOT};
# This is a Perl script that runs the Naming Service, client and servers

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

# amount of delay between running the servers

$sleeptime = 6;
$status = 0;

# variables for parameters

$nsport = 20000 + uniqueid ();
sub name_server
{
    my $args = "-ORBnameserviceport $nsport";
    my $prog = "$tao_root/orbsvcs/Naming_Service/Naming_Service"
        .$EXE_EXT;
    print ("\nNaming_Service: $prog$EXE_EXT $args\n");
    $NS = Process::Create ($prog, $args);
}


sub server
{
    my $args = "-ORBnameserviceport $nsport";
    print ("\nServer: server$EXE_EXT $args\n");
    $SV = Process::Create ($EXEPREFIX."server$EXE_EXT", $args);
}


sub client
{
    my $args = "-ORBnameserviceport $nsport";
    print ("\nclient: client $args\n");
    $CL = Process::Create ($EXEPREFIX."client$EXE_EXT", $args);
}

name_server ();
sleep $sleeptime;

server ();
sleep $sleeptime;

client ();
sleep $sleeptime;


if ($CL->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $CL->Kill (); $CL->TimedWait (1);
}

$NS->Kill (); $NS->TimedWait (1);
$SV->Kill (); $NS->TimedWait (1);

exit $status;

