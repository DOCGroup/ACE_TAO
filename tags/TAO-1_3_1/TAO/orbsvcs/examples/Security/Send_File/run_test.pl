eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use Env (ACE_ROOT);

unshift @INC, "$ACE_ROOT/bin";
require ACEutils;

$status = 0;

# Set the SSL environment
$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$iorfile = "server.ior";
unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBSvcConf server_nopasswd" .
                       "$PerlACE::svcconf_ext " .
                       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -ORBSvcConf client_nopasswd" .
                       "$PerlACE::svcconf_ext " .
                       " -k file://$iorfile < client.cpp");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (15);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
