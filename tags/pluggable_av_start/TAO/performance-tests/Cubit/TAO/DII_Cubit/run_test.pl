eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require Process;

$iorfile = "theior";
$sleeptime = 5;

$SV = Process::Create ("..$DIR_SEPARATOR"
                       ."IDL_Cubit".$DIR_SEPARATOR.
                       "server".$Process::EXE_EXT,
                       " -o $iorfile");

sleep $sleeptime;

$status = system ("client".$Process::EXE_EXT.
                  " -f $iorfile -x");

# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
