eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require Process;
require ACEutils;
use Cwd;

# amount of delay between running the servers

$sleeptime = 6;
$status = 0;
$cwd = getcwd();
local $nsior = "$cwd$DIR_SEPARATOR" . "ns.ior";

for($i = 0; $i <= $#ARGV; $i++) {
  if ($ARGV[$i] eq '-chorus') {
    $i++;
    if (defined $ARGV[$i]) {
      $EXEPREFIX = "rsh $ARGV[$i] arun $cwd$DIR_SEPARATOR";
    }
    else {
      print STDERR "The -chorus option requires the hostname of the target\n";
      exit(1);
    }
  }
}

$name_server_prog = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."Naming_Service".$DIR_SEPARATOR."Naming_Service".$EXE_EXT;
$server_prog = $EXEPREFIX.".".$DIR_SEPARATOR."server".$EXE_EXT;
$client_prog = $EXEPREFIX.".".$DIR_SEPARATOR."client".$EXE_EXT;


# variables for parameters

sub name_server
{
    my $args = "-o $nsior";
    print ("\n$name_server_prog $args\n");
    $NS = Process::Create ($name_server_prog, $args);
    if (ACE::waitforfile_timed ($nsior, 5) == -1) {
      print STDERR "ERROR: cannot find naming service IOR file\n";
      $NS->Kill (); $NS->TimedWait (1);
      exit 1;
    }
}


sub server
{
    my $args = "-ORBInitRef NameService=file://$nsior";
    print ("\nServer: $args\n");
    $SV = Process::Create ($server_prog, $args);
}


sub client
{
    my $args = "-ORBInitRef NameService=file://$nsior";
    print ("\nclient: $args\n");
    $CL = Process::Create ($client_prog, $args);
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
