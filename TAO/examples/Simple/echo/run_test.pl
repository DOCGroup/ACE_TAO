#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";
require ACEutils;
require Process;


$SV = Process::Create ("server$Process::EXE_EXT", " ");
sleep ($ACE::sleeptime);
$status = system ("client$Process::EXE_EXT  -x");

$SV->Kill (); $SV->Wait ();

exit $status;
