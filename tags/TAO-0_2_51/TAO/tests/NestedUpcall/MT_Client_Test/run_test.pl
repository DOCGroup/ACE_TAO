#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$server1_port = 0;
$server2_port = 0;
$ior1file = "server1.ior";
$ior2file = "server2.ior";

# Make sure the files are gone
unlink $ior1file;
unlink $ior2file;

$SV1 = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT, 
                        " -ORBport $server1_port -o $ior1file ".
			" -ORBobjrefstyle url");

$SV2 = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT, 
                        " -ORBport $server2_port -o $ior2file ".
			" -ORBobjrefstyle url");

ACE::waitforfile ($ior1file);
ACE::waitforfile ($ior2file);

$status = system ($EXEPREFIX."client".$Process::EXE_EXT.
		  " -ORBport $client_port ".
		  " -f $ior1file -g $ior2file -n 2 -i 10 -s 5");

$SV1->Kill (); $SV1->Wait ();
$SV2->Kill (); $SV2->Wait ();

#clean up
unlink $ior1file;
unlink $ior2file;

# @@ Capture any errors from the server too.
exit $status;
