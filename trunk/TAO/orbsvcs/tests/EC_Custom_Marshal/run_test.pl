eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 5;

$NS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT.
                       " -o $NS_ior ");
sleep $sleeptime;

$ES = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Event_Service".$DIR_SEPARATOR.
                       "Event_Service".$EXE_EXT);

$C = Process::Create ($EXEPREFIX."ECM_Consumer".$EXE_EXT);

sleep $sleeptime;

$S = Process::Create ($EXEPREFIX."ECM_Supplier".$EXE_EXT);

$C->Wait ();
$S->Wait ();

$NS->Kill (); $NS->Wait ();
$ES->Kill (); $ES->Wait ();

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit 0;
