# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$tao_root = $ENV{TAO_ROOT};
# This is a Perl script that runs the Naming Service, client and servers

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

# amount of delay between running the servers

$sleeptime = 6;

# variables for parameters

$nsport = 20000 + uniqueid ();
sub name_server
{
    my $args = "-ORBnameserviceport $nsport";
    my $prog = "$tao_root/orbsvcs/Naming_Service/Naming_Service"
	.$Process::EXE_EXT;
    print ("\nNaming_Service: $prog$Process::EXE_EXT $args\n");
    $NS = Process::Create ($prog, $args);
}


sub server
{
    my $args = "-ORBnameserviceport $nsport";
    print ("\nServer: server$Process::EXE_EXT $args\n");
    $SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", $args);
}


sub client
{
    my $args = "-ORBnameserviceport $nsport";
    print ("\nclient: client $args\n");
    $CL = Process::Create ($EXEPREFIX."client$Process::EXE_EXT", $args);
}

name_server ();
sleep $sleeptime;

server ();
sleep $sleeptime;

client ();
sleep $sleeptime;

$NS->Kill ();
$SV->Kill ();

