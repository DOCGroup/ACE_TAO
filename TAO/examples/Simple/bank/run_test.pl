#$Id $
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";
require ACEutils;
require Process;


$SV = Process::Create ("server$Process::EXE_EXT", " ");

sleep(10);
$status  = system ("client$Process::EXE_EXT  -x -b 100 -y Vishal -z Kachroo");
#sleep($ACE::sleeptime);

$SV->Kill (); $SV->Wait ();

exit $status;



