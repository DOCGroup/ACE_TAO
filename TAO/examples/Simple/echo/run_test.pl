eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
require ACEutils;
require Process;

$iorfile = "echo.ior";

$SV = Process::Create ("server$EXE_EXT", "-o $iorfile ");

sleep ($ACE::sleeptime);
$status = system ("client$EXE_EXT -f $iorfile -x");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
