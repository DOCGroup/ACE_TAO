     eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
  &  eval 'exec perl -S $0 $argv:q'
  if 0;

# $Id$
# -*- perl -*-

# This is a Perl Script that runs the corbaloc: style URL tests.
# It starts the servers and client.

use lib "../../../bin";

use PerlACE::Run_Test;

$TARGETHOSTNAME = "localhost";

$status = 0;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 8;

@iorfile = ("ns1.ior",
            "ns2.ior",
            "ns2.ior" );

$def_port = 2809;
@ns_orb_port=( 12000 + PerlACE::uniqueid (),
               $def_port);

# List of tests to test corbaloc URL syntax.
@corbaloc_tests = ("corbaloc:rir:/NameService",
         "corbaloc:rir:/",
         "corbaloc:iiop:$TARGETHOSTNAME:$ns_orb_port[0]/NameService",
         "corbaloc::$TARGETHOSTNAME:$ns_orb_port[0]/NameService",
         "corbaloc::1.0\@$TARGETHOSTNAME:$ns_orb_port[0]/NameService",
	 "corbaloc::$TARGETHOSTNAME/NameService",
         "corbaloc::1.1\@$TARGETHOSTNAME:$ns_orb_port[0]/NameService",
         "corbaloc::1.2\@$TARGETHOSTNAME:$ns_orb_port[0]/NameService");


@comments = ("Using RIR protocol with the object key specified: \n",
             "Using RIR protocol without specifying the object key: \n(empty object key defaults to \"NameService\"\n",
             "Using IIOP protocol with the hostname, port specified: \n",
             "Using IIOP protocol, with protocol unspecified, hostname specified, port specified: \n",
             "Using the default protocol (iiop) with the hostname, port specified: \n",
             "Using IIOP protocol with protocol unspecified, hostname specified, port unspecified\n(should default to $def_port): \n");

$test_number = 0;

# Run the naming service, server. Run the client for each of the test. 
# Client uses ior in a file to bootstrap to the server.

@SR = (new PerlACE::Process ("server"),
       new PerlACE::Process ("server"),
       new PerlACE::Process ("server"));
@SR_NAME=( "STATUS", "STATUS1", "STATUS");

$CL = new PerlACE::Process ("client");
$CN_CL = new PerlACE::Process ("corbaname_client");

# We run two Naming_Service processes.  One on an arbitrary port,
# and one on the corbaloc: default port of 2809
@NS = (new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service"),
       new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service"));

for($i=0; $i <= $#NS; $i++){
  # Make sure the files are gone, so we can wait on them.
  unlink $iorfile[$i];
  print "Spawning Naming Service on port $ns_orb_port[$i]......\n";
  $NS[$i]->Arguments (" -ORBEndpoint iiop://$TARGETHOSTNAME:$ns_orb_port[$i]"
                      ." -o $iorfile[$i] -m 0");
  
  $NS[$i]->Spawn ();
  
  if (PerlACE::waitforfile_timed ($iorfile[$i], $sleeptime) == -1) {
    print STDERR "ERROR: cannot find IOR file <$iorfile[$i]>\n";
    $NS[$i]->Kill (); 
    exit 1;
  }
}

sleep 2;

for($i=0; $i <= $#SR; $i++){
  print "Spawning server with name $SR_NAME[$i]\n";
  $SR[$i]->Arguments ("-ORBInitRef NameService=file://$iorfile[$i] $SR_NAME[$i]");
  $SR[$i]->Spawn ();
}

sleep 5;


print "\n======= Running corbaloc: tests\n";
foreach $o (@corbaloc_tests) {  
  print STDERR "\n".$test_number.".  ".$comments[$test_number];
  print STDERR "             $o\n";
  
  
  $CL->Arguments ("$o -ORBInitRef NameService=file://$iorfile[0]");
  $client = $CL->SpawnWaitKill (60);
  
  if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
  }
  
  $test_number++;
}



print "\n======= Running corbaname: tests\n";
@corbaname_tests = ("corbaname:iiop:$TARGETHOSTNAME:$ns_orb_port[0]#$SR_NAME[0]",
                    "corbaname::$TARGETHOSTNAME:$ns_orb_port[0]#$SR_NAME[0]",

                    "corbaname::$TARGETHOSTNAME#$SR_NAME[1]",
                    "corbaname::$TARGETHOSTNAME:$ns_orb_port[1]#$SR_NAME[1]");

foreach $o ( @corbaname_tests ){
  print STDERR "\n$test_number:  $o\n";
  $CN_CL->Arguments ("$o $corbaname_tests[$i]");
  $client = $CN_CL->SpawnWaitKill (60);


  $test_number++;
}


for($i=0; $i <= $#NS; $i++){
$nameservice = $NS[$i]->TerminateWaitKill (10); 

if ($nameservice != 0) {
  print STDERR "ERROR: name server $i returned $nameservice\n";
  $status = 1;
}
}


for($i=0; $i <= $#SR; $i++){
$server = $SR[$i]->TerminateWaitKill (10);
if ($server != 0) {
  print STDERR "ERROR: server $i returned $server\n";
  $status = 1;
}
}

exit $status;
