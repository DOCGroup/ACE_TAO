eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;
require Process;

$iorfile = "./ior";

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT");

ACE::waitforfile ($iorfile);

$status  = system ($EXEPREFIX."client$EXE_EXT -f $iorfile -ORBSkipServiceConfigOpen");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
