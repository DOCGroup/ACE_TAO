# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 5;

$NS = Process::Create ("..".$DIR_SEPARATOR.
		       "..".$DIR_SEPARATOR.
		       "Naming_Service".$DIR_SEPARATOR.
		       "Naming_Service".$Process::EXE_EXT.
		       " -o $NS_ior ");

sleep $sleeptime;

$T = Process::Create ($EXEPREFIX."ECT_Throughput".$Process::EXE_EXT);

sleep $sleeptime;

$T->Wait ();

$NS->Kill (); $NS->Wait ();

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit 0;
