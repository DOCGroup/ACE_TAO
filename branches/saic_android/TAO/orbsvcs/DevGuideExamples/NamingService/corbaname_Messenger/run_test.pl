# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
  &  eval 'exec perl -S $0 $argv:q'
  if 0;
  
use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;


$TARGETHOSTNAME = "localhost";
$def_port = 2809;
$nsior = "ns.ior";

# start Naming Service
unlink($nsior);
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-ORBEndpoint iiop://$TARGETHOSTNAME:$def_port -o $nsior");
$NS->Spawn();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
  print STDERR "ERROR: cannot find IOR file <$nsior>\n";
  $NS->Kill ();
  exit 1;     
}

# start the server
print "Start Messenger Server \n";
$SR =  new PerlACE::Process("MessengerServer", 
  "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:$def_port/NameService");
$SR->Spawn();
sleep(2);


@corbaname_clients = ("corbaname:iiop:$TARGETHOSTNAME:2809#example/Messenger",
                      "corbaname::$TARGETHOSTNAME:2809#example/Messenger",
                      "corbaname::$TARGETHOSTNAME#example/Messenger",
                      "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:2809/NameService corbaname:rir:#example/Messenger");

@clients_comments = ( "Using a corbaname that does not require -ORBDefaultInitRef: \n",
                      "Using defaults to iiop protocol: \n",
                      "Using default protocol (iiop) and default port ($def_port): \n",
                      "Using Resolve Initial Reference form(must tell ORB where NS is located with -ORBInitRef)\n");

# Run the client for each of the corbaname test. 
$test_number = 0;
foreach $o (@corbaname_clients) {   
  
  $CL = new PerlACE::Process("MessengerClient", $o);

  if ($CL->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $SR->Kill();
   $NS->Kill();
   exit 1;
  }
   
  $test_number++;

  print "======================================\n";
  print "Finish Test $test_number: $clients_comments[$test_number]\n";
  print "             $CL_NAME $o\n";
  print "======================================\n\n";

}

# clean up 
$SR->Kill ();
$NS->Kill ();

exit 0;
