eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$client_conf="$cwd$DIR_SEPARATOR" . "muxed.conf";
$client_process="simple_client";
$debug_level='5';
$threads='2';
$iterations='1';

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
  elsif ($ARGV[$i] eq '-mux') {
    $client_conf = "muxed.conf";
  }
  elsif ($ARGV[$i] eq '-debug') {
    $debug_level = '1';
  }
  elsif ($ARGV[$i] eq '-exclusive') {
    $client_conf = 'exclusive.conf';
  }
}


$iorfile = "$cwd$DIR_SEPARATOR" . "server.ior";
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
			  " -ORBdebuglevel $debug_level"
			  . "-d -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."$client_process$EXE_EXT ",
			  " -ORBsvcconf $client_conf "
			  . "-ORBdebuglevel $debug_level"
			  . " -k file://$iorfile "
			  . " -i $iterations -x -d");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0 || $client != 0) {
   exit 1;
}
 
exit 0;
 
