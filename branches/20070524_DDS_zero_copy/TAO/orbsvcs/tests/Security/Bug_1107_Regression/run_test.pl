eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$


use lib  "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");

unlink $file;

$status = 0;

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = new PerlACE::Process ("server",
			    "-o $file -ORBSvcConf server$PerlACE::svcconf_ext");
$CL = new PerlACE::Process ("client",
			    "-n -ORBSvcConf client$PerlACE::svcconf_ext -k file://$file");

print STDERR "\n\n==== Running Bug_1107_Regression test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

## In order to test the following, we are running the client
## without the SSL_CERT_FILE set three times:
##
## The behavior we're observing is
##
## 1. Start the server normally.
## 2. Start the client without identifying the CA certificate file.
## 3. Server rejects the connection request and the client receives
##    "tlsv1 alert, unknown ca."
## 4. Start the client normally.
## 5. Server reports two identical errors from ACE_SSL
##    ACE_SSL () error code: 336462100 - error: 140E0114:SSL routines: 
##    SSL_shutdown:uninitialized
## 6. Client receives a CORBA::TRANSIENT exception.
## 7. Start the client normally.
## 8. Test completes normally.
##    
## In step 4, it isn't necessary to start the client normally.  The behavior
## is the same, including the server's eventual return to normal behavior,
## even if the client is started without defining the CA certifcate file.
for(my $i = 0; $i < 3; $i++) {
  print STDERR "\nRunning the client without SSL_CERT_FILE\n";
  $client = $CL->SpawnWaitKill (60);

  if ($client != 0) {
      print STDERR "ERROR: client returned $client\n";
      $status = 1;
      last;
  }
}


print STDERR "\nRunning the client with SSL_CERT_FILE\n";

my $args = $CL->Arguments();
$args =~ s/\-n\s+//;
$CL->Arguments($args);
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file;

exit $status;
