#$Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;

$client_port = 0;
$server1_port = 0;
$server2_port = 0;
$ior1file = "theior1";
$ior2file = "theior2";
$sleeptime = 3;

$SV1 = Process::Create ("server_A".$Process::EXE_EXT, 
                        "-ORBport $server1_port -o $ior1file -ORBobjrefstyle url");

$SV2 = Process::Create ("server_B".$Process::EXE_EXT, 
                        "-ORBport $server2_port -o $ior2file -ORBobjrefstyle url");

sleep ($sleeptime);

system ("initiator".$Process::EXE_EXT." -ORBport $client_port -f $ior1file -g $ior2file");

$SV1->Kill ();
$SV2->Kill ();

if ($^O eq "MSWin32")
{
  system ("del ".$ior1file);
  system ("del ".$ior2file);
}
else
{
  system ("rm ".$ior1file);
  system ("rm ".$ior2file);
}
