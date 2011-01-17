eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";

require ACEutils;
require Process;

$status = 0;

# amount of delay between starting servers and client
$sleeptime = 15;



sub name_server
{
  my $prog = "..$DIR_SEPARATOR".
             "..$DIR_SEPARATOR".
             "..$DIR_SEPARATOR".
             "orbsvcs$DIR_SEPARATOR".
             "Naming_Service$DIR_SEPARATOR".
             "tao_cosnaming$EXE_EXT";
  $NS = Process::Create ($prog,"");
}



# create naming service, server, client
name_server ();
sleep $sleeptime;

$server = Process::Create ($EXEPREFIX."server".$EXE_EXT, "");
sleep $sleeptime;

$client = Process::Create($EXEPREFIX."client".$EXE_EXT, "");



# wait for client to finish
if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimedWait (1);
}


# gracefully kill the server
$server->Terminate ();
if ($server->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate the server\n";
  $server->Kill (); $server->TimedWait (1);
  $status = 1;
}

# gracefully kill the Naming Service
$NS->Terminate ();
if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate the server\n";
  $NS->Kill (); $server->TimedWait (1);
  $status = 1;
}

exit $status;




