eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;

$iorfile = "server.ior";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
		       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$Process::EXE_EXT ",
			" -k $iorfile");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Kill (); $SV->TimedWait (1);

unlink $iorfile;
unlink "test"; # created by the program

if ($client != 0) {
  exit 1;
}

exit 0;
