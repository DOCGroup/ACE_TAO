eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";

ACE::checkForTarget($cwd);

print STDERR "********** RTCORBA Server Protocol Policy Unit Test\n";


sub server
{
  my $args = "@_ "."-o $iorfile";

  unlink $iorfile;
  $SV = Process::Create ($EXEPREFIX."server$EXE_EXT ", $args);

  if (ACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find IOR file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
  }
}

sub client
{
  my $args = " -k file://$iorfile -ORBdebuglevel 1";

  $CL = Process::Create ($EXEPREFIX."client$EXE_EXT ", $args);

  $client = $CL->TimedWait (60);
  if ($client == -1) {
      print STDERR "ERROR: client timedout\n";
      $CL->Kill (); $CL->TimedWait (1);
  }
}

# Configurations for all tests to be run.
@server_opts =
    ("-ORBendpoint iiop://",

     "-ORBsvcconf server_reverse.conf "
     ."-ORBEndpoint uiop:// -ORBendpoint shmiop:// -ORBendpoint iiop://",

     "-ORBsvcconf server_shmiop.conf -ORBendpoint shmiop://",

     "-ORBEndpoint uiop:// "
     ." -ORBendpoint shmiop:// -ORBendpoint iiop://"
     ." -p 1413566208",

     "-ORBEndpoint uiop:// "
     ." -ORBendpoint shmiop:// -ORBendpoint iiop://"
     ." -p 1413566210 ");

@comments = ("* ORB Default Server Protocol Policy Test\n          "
             ."(TAO's default behavior without config files): \n",

             "* ORB Default Server Protocol Policy Test\n          "
             ."(All TAO protocols are loaded, but in reverse order): \n",

             "* ORB Default Server Protocol Policy Test\n          "
            ."(Only SHMIOP loaded): \n",

             "* Overriding ORB Default Server Protocol Policy in the POA\n"
             ."          (POA Server Protocol set to UIOP only): \n",

             "* Overriding ORB Default Server Protocol Policy in the POA\n"
             ."          (POA Server Protocol set to SHMIOP only): \n");

# UIOP only available on Unix.  Substitute with alternative tests on Windows.
if ($^O eq "MSWin32")
{
    @server_opts =
        ("-ORBendpoint iiop://",

         "-ORBsvcconf server_reverse_nt.conf "
         ."-ORBEndpoint shmiop:// -ORBendpoint iiop://",

         "-ORBsvcconf server_shmiop.conf -ORBendpoint shmiop://",

         "-ORBendpoint shmiop:// -ORBendpoint iiop://"
         ." -p 0",

         "-ORBendpoint shmiop:// -ORBendpoint iiop://"
         ." -p 1413566210");

    $comments[3] =
        "Overriding ORB Default Server Protocol Policy in the POA "
            ."(POA Server Protocol set to IIOP only): \n";
}


$test_number = 0;

foreach $o (@server_opts)
{
    print STDERR "\n\n----------------------------------\n";
    print STDERR "        ".$comments[$test_number];

    server ($o);

    client ();

    $SV->Terminate ();
    $server = $SV->TimedWait (60);
    if ($server == -1) {
        print STDERR "ERROR: server timedout\n";
        $SV->Kill (); $SV->TimedWait (1);
    }

    $test_number++;
}

unlink $iorfile;
exit 0;
