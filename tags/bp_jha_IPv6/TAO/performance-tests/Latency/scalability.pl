eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$client_conf="client.conf";
$server_conf="server.conf";
$iterations = 10000;

$iorfile = "test.ior";

print STDERR "================ Java server test\n";

unlink $iorfile;

$SV = Process::Create ("vbj",
		       "java_server",
                       " -D -OAid TPool"
                       ." -D -OAthreadMax 0"
                       ." -D -OAthreadMin 16");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

for ($i = 1; $i < 40; $i += 4) {
  $CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			 " -ORBSvcConf client.conf "
			 . " -k file://$iorfile "
			 . " -n $i -i $iterations -p 10 -x");

  # Each iteration should take 100 seconds
  $client = $CL->Wait ();
  if ($client == -1) {
    print STDERR "ERROR: client <$i> timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }
}

$server = $SV->Terminate (); $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

print STDERR "================ C++ server test\n";

unlink $iorfile;

$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBSvcConf $server_conf"
                       . " -o $iorfile -n 16");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

for ($i = 1; $i < 40; $i += 4) {
  $CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
			 " -ORBSvcConf client.conf "
			 . " -k file://$iorfile "
			 . " -n $i -i $iterations -p 10 -x");

  # Each iteration should take 100 seconds
  $client = $CL->Wait ();
  if ($client == -1) {
    print STDERR "ERROR: client <$i> timedout\n";
    $CL->Kill (); $CL->TimedWait (1);
  }
}

$server = $SV->Terminate (); $SV->TimedWait (10);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

exit 0;
