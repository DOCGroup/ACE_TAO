eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "/tmp";
use lib "../../../bin";
require ACEutils;
require Process;

$status = 0;
$iorfile = "chapter_test.ior";
unlink $iorfile;



#Create the Server and wait for the ior file
$server = Process::Create ("exec server$EXE_EXT >$iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: timedout waiting for file <$iorfile>\n";
  $server->Kill (); $server->TimedWait (1);
  exit 1;
}

#Open and load the IOR to a variable
open(ior_handle, "$iorfile");
$ior_content = <ior_handle>;


#Create the Client
$client = Process::Create($EXEPREFIX."client$EXE_EXT", "$ior_content");


#Wait for the Client to finish
if ($client->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $client->Kill (); $client->TimedWait (1);
}


#Gracefully kill the Server
$server->Terminate (); 
if ($server->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate the server\n";
  $server->Kill (); $server->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
