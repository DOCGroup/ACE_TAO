# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

# amount of delay between running the servers

$sleeptime = 3;

# variables for parameters

$nsmport = 10000 + uniqueid ();

sub name_server
{
  my $args = "-ORBnameserviceport $nsmport";
  my $prog = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."Naming_Service".$DIR_SEPARATOR.
             "Naming_Service".$Process::EXE_EXT;

  $NS = Process::Create ($prog, $args);
}

sub client
{
  my $args = "-ORBnameserviceport $nsmport";
  my $prog = "client".$Process::EXE_EXT;
    
  system ($prog." ".$args);
}

name_server ();
sleep $sleeptime;
client ();

$NS->Kill ();

# @@ Capture any exit status from the processes.
exit 0;
