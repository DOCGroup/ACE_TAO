eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs a Load Balancing service demo.
# See README file for details about the demo.

unshift @INC, '../../../bin';
require Process;
require ACEutils;
require Uniqueid;

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 4;

# File used to pass load balancing service ior to its clients.
$iorfile = "lb.ior";

# Run the load balancing server.
sub load_balancing_server
{
  my $args = "@_"." -o $iorfile";
  my $prog = $EXEPREFIX."load_balancer".$EXE_EXT;

  unlink $iorfile;
  $LB = Process::Create ($prog, $args);

  if (ACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
      print STDERR "ERROR: File containing Load Balancing Service ior,".
          " <$iorfile>, cannot be found\n";
      $LB->Kill (); $LB->TimedWait (1);
      exit 1;
  }
}

# Run the identity server, which registers its objects with the load
# balancing server.
sub server
{
    my $args = "@_"." -i file://$iorfile ";
    my $prog = $EXEPREFIX."server".$EXE_EXT;

    if (ACE::waitforfile_timed ($iorfile, $sleeptime) == -1)
    {
        exit 1;
    }

    $S = Process::Create ($prog, $args);
    sleep ($sleeptime);
}

# Run client, which obtains references to Identity objects from the load balancing
# service, and invokes operations on them.
sub client
{
    my $args = "@_"." -i file://$iorfile -n 10";
    my $prog = $EXEPREFIX."client".$EXE_EXT;

    $CL = Process::Create ($prog, $args);

    $client = $CL->TimedWait (60);
    if ($client == -1) {
        print STDERR "ERROR: client timedout\n";
        $CL->Kill (); $CL->TimedWait (1);
    }
}

# Data for each test we will run, i.e., client command-line options
# and test description.
@client_opts = ("", "-r");

@test_heading = ("Client using Round Robin Object Group (10 iterations): \n\n",
                 "Client using Random Object Group (10 iterations): \n\n");

print STDERR "\n    Starting Load Balancing Server and Identity Server \n\n";
load_balancing_server ("");
server ("");

# Run tests, i.e., run client with different command line options.
$test_number = 0;
foreach $o (@client_opts)
{
    print STDERR "\n     ".$test_heading[$test_number];
    client ($o);

    $test_number++;
}

# Clean up.
$LB->Terminate (); $lserver = $LB->TimedWait (5);
if ($lserver == -1) {
    print STDERR "ERROR: load balancing server timedout\n";
    $LB->Kill (); $LB->TimedWait (1);
}
$S->Terminate (); $server = $S->TimedWait (5);
if ($server == -1) {
    print STDERR "ERROR: identity server timedout\n";
    $S->Kill (); $S->TimedWait (1);
}

# @@ Capture any exit status from the processes.
exit 0;
