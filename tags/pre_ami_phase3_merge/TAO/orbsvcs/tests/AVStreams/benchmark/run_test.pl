eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require Process;
require ACEutils;

# amount of delay between running the servers

$sleeptime = 6;
$status = 0;
local $nsior = "ns.ior";

# variables for parameters

sub name_server
{
    my $args = "-o $nsior";
    my $prog = "..".$DIR_SEPARATOR
      ."..".$DIR_SEPARATOR
	."..".$DIR_SEPARATOR
	  ."Naming_Service".$DIR_SEPARATOR
	."Naming_Service ".$EXE_EXT;
    print ("\nNaming_Service $args\n");
    $NS = Process::Create ($prog, $args);
    if (ACE::waitforfile_timed ($nsior, 5) == -1) {
      print STDERR "ERROR: cannot find naming service IOR file\n";
      $NS->Kill (); $NS->TimedWait (1);
      exit 1;
    }
}


sub server
{
    my $args = "-ORBNameServiceIOR file://$nsior";
    print ("\nServer: $args\n");
    $SV = Process::Create (".".$DIR_SEPARATOR."server".$EXE_EXT, $args);
}


sub client
{
    my $args = "-ORBNameServiceIOR file://$nsior";
    print ("\nclient: $args\n");
    $CL = Process::Create (".".$DIR_SEPARATOR."client".$EXE_EXT, $args);
}

name_server ();

server ();

sleep $sleeptime;

client ();

if ($CL->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Terminate (); if ($SV->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate server\n";
  $SV->Kill (); $SV->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}
  
$NS->Terminate (); if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate naming service\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}
  
exit $status;
