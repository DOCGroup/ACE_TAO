# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$ior1file = "servera.ior";
$ior2file = "serverb.ior";

#  Make sure these aren't hanging around
unlink $ior1file;
unlink $ior2file;

$SV1 = Process::Create ($EXEPREFIX."server_A".$Process::EXE_EXT, 
                        "-o $ior1file -ORBobjrefstyle url");

$SV2 = Process::Create ($EXEPREFIX."server_B".$Process::EXE_EXT, 
                        "-o $ior2file -ORBobjrefstyle url");

ACE::waitforfile ($ior1file);
ACE::waitforfile ($ior2file);

$status = system ($EXEPREFIX."initiator".$Process::EXE_EXT.
		  " -f $ior1file -g $ior2file");

$SV1->Kill (); $SV1->Wait ();
$SV2->Kill (); $SV2->Wait ();

# Clean up
unlink $ior1file;
unlink $ior2file;

# @@ Capture any errors from the server too.
exit $status;
