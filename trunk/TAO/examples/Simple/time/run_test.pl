eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;

$iorfile = "time.ior";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT", "-o $iorfile ");

ACE::waitforfile ($iorfile);

$status  = system ($EXEPREFIX."client$EXE_EXT -f $iorfile -x");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
