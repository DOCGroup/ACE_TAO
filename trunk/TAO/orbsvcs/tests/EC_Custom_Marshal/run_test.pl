eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;
use Cwd;

$port = ACE::uniqueid () + 10001;  # This can't be 10000 on Chorus 4.0
$cwd = getcwd();
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

$NS_ior = "$cwd$DIR_SEPARATOR" . "NameService.ior";
$sleeptime = 5;
$status = 0;

$NS = Process::Create ($EXEPREFIX."..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT,
                       "-ORBNameServicePort $port -o $NS_ior ");

if (ACE::waitforfile_timed ($NS_ior, 5) == -1) {
  print STDERR "ERROR: waiting for naming service IOR file\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$ES = Process::Create ($EXEPREFIX."..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Event_Service".$DIR_SEPARATOR.
                       "Event_Service".$EXE_EXT,
		       "-ORBInitRef NameService=file://$NS_ior -t new");

sleep $sleeptime;

$C = Process::Create ($EXEPREFIX."ECM_Consumer".$EXE_EXT,
		      "-ORBInitRef NameService=file://$NS_ior");

sleep $sleeptime;

$S = Process::Create ($EXEPREFIX."ECM_Supplier".$EXE_EXT,
		      "-ORBInitRef NameService=file://$NS_ior");

if ($C->TimedWait (60) == -1) {
  $status = 1;
  print STDERR "ERROR: consumer timedout\n";
  $C->Kill (); $C->TimedWait (1);
}

if ($S->TimedWait (60) == -1) {
  $status =1;
  print STDERR "ERROR: supplier timedout\n";
  $S->Kill (); $S->TimedWait (1);
}

$NS->Terminate();
$ES->Terminate();
if ($NS->TimedWait (5) == -1 || $ES->TimedWait (5) == -1) {
  print STDERR "ERROR: couldn't terminate the services nicely\n";
  $NS->Kill (); $NS->TimedWait (1);
  $ES->Kill (); $ES->TimedWait (1);
  $status = 1;
}

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit $status;
