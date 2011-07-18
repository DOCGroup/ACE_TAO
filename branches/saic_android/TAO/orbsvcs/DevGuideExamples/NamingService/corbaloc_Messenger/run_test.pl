# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
  &  eval 'exec perl -S $0 $argv:q'
  if 0;
  
use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;


$TARGETHOSTNAME = "localhost";
$def_port = 2809;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-ORBEndpoint iiop://$TARGETHOSTNAME:$def_port");
$NS->Spawn();

sleep(1);

# List of tests to test corbaloc URL syntax.
@corbaloc_servers = ( "-ORBDefaultInitRef corbaloc::$TARGETHOSTNAME",
                      "-ORBInitRef NameService=corbaloc::$TARGETHOSTNAME/NameService",
                      "-ORBInitRef NameService=corbaloc::$TARGETHOSTNAME:$def_port/NameService");


@servers_comments = ( "Using -ORBDefaultInitRef with default port $def_port and default transport IIOP: \n",
                      "Using -ORBInitRef with corbaloc URL: \n",
                      "Using -ORBInitRef with corbaloc URL and specified port: \n");

@corbaloc_clients = ( "corbaloc::$TARGETHOSTNAME/NameService",
                      "-ORBInitRef NameService=iiop://$TARGETHOSTNAME:$def_port/NameService corbaloc:rir:/NameService");

@clients_comments = ( "Using the URL parameter: \n");
                      "Using a corbaloc:rir form URL(must specify initial reference): \n",




$MessengerServer= "MessengerServer";
$MessengerClient= "MessengerClient";

$test_number = 0;

foreach $o (@corbaloc_servers) {  

  # Run messenger server for each test.  
  #print "Start $MessengerServer $o \n";
  $SR = new PerlACE::Process($MessengerServer, $o);
  $SR->Spawn();

  sleep(1);
  
  #print "Start $MessengerClient \n";
  $CL = new PerlACE::Process($MessengerClient, "-ORBDefaultInitRef iiop://$TARGETHOSTNAME:$def_port");
  $test_number++;

  if ($CL->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $SR->Kill();
   $NS->Kill();
   exit 1;
  }
  
  print "======================================\n";
  print "Finish Test $test_number: $servers_comments[$test_number] \n";
  print "  $MessengerServer $o\n";
  print "======================================\n\n";

  $SR->Kill(1);
}


#print "Start $MessengerServer \n";
$SR = new PerlACE::Process($MessengerServer, "-ORBDefaultInitRef iiop://$TARGETHOSTNAME:$def_port");
$SR->Spawn();

sleep(1);

$i = 0;
foreach $o (@corbaloc_clients) {
  
  # Run the client for each test.  
  #print "Start $MessengerClient $o \n";
  $CL = new PerlACE::Process($MessengerClient, $o);


  if ($CL->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $SR->Kill();
   $NS->Kill();
   exit 1;
  }

  $test_number++;
  print "======================================\n";
  print "Finish Test $test_number: $clients_comments[$i]\n";
  print "             $MessengerClient $o";
  print "\n======================================\n\n";

  $i ++;
}


# clean up 

$SR->Kill();
$NS->Kill();

exit 0;
