eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;

$iorfile = "ior";

$iorfile_1 = $iorfile."_1";
$iorfile_2 = $iorfile."_2";
$iorfile_3 = $iorfile."_3";
$iorfile_4 = $iorfile."_4";
$iorfile_5 = $iorfile."_5";
$iorfile_6 = $iorfile."_6";

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;
unlink $iorfile_4;
unlink $iorfile_5;
unlink $iorfile_6;

$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT");

# In this example all the files are written out at the same time. So  make a
# check only for the first file
if (ACE::waitforfile_timed ($iorfile_1, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

ACE::waitforfile ($iorfile_1);
$status  = Process::Create ($EXEPREFIX."client$Process::EXE_EXT  -a file://$iorfile_1 -b file://$iorfile_2 -c file://$iorfile_3 -d file://$iorfile_4");

if (ACE::waitforfile_timed ($iorfile_5,1) == 0) {
$status  = Process::Create ($EXEPREFIX."client$Process::EXE_EXT  -e file://$iorfile_5 -f file://$iorfile_6");
}

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;
unlink $iorfile_4;
unlink $iorfile_5;
unlink $iorfile_6;

$SV->Kill ();$SV->Wait ();

exit $client;
