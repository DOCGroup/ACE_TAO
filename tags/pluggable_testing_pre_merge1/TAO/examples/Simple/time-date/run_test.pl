eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;
require Process;

$iorfile = "grid.ior";

$SV = Process::Create ($EXEPREFIX."server$Process::EXE_EXT", "-o $iorfile ");

ACE::waitforfile ($iorfile);

$status  = system ($EXEPREFIX."client$Process::EXE_EXT -f $iorfile -x -w 12 -h 12 -p 2 -q 2 -v 2345");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
