eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
my $ACE_ROOT = $ENV{ACE_ROOT};

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Cwd;
use Sys::Hostname;
use File::Copy;

use strict;
use POSIX "sys_wait_h";

my $cwd = getcwd();

if (!defined $ACE_ROOT) {
    print "Error: ACE_ROOT not defined.\n";
    return 1;
}

# this matches the orbendpoint used in start_all.conf
my $imr_corbaloc = "corbaloc::localhost:9999";
my $imr_initref = "-orbinitref ImplRepoService=$imr_corbaloc/ImplRepoService";

my $imr_start_file = PerlACE::LocalFile ("imr_status");
my $server_start_file = PerlACE::LocalFile ("server_status");

my $start_conf = PerlACE::LocalFile ("start_all.conf");

my $COMB = new PerlACE::Process (PerlACE::LocalFile ("combined_service"));
my $combined_ior = PerlACE::LocalFile ("combined.ior");
my $cs_initref = "-orbinitref ServiceConfig=file://$combined_ior";

my $SERV = new PerlACE::Process (PerlACE::LocalFile ("test_server"));
my $test_server = $SERV->Executable ();

my $IMRUTIL = new PerlACE::Process("$ACE_ROOT/bin/tao_imr");

my $CLI = new PerlACE::Process ('test');
my $test_initref = "-orbinitref Test=$imr_corbaloc";

my $hostname = hostname();

my $rundate = POSIX::strftime("%Y_%m_%d_%H_%M", localtime);
my $output_file = PerlACE::LocalFile ("run_test_$rundate");

my $status = 0;

###############################################################################

sub restore_output
{
    # Restore output facilities.
    close (STDERR);
    close (STDOUT);
    open (STDOUT, ">&OLDOUT");
    open (STDERR, ">&OLDERR");
}

sub redirect_output
{
    open (OLDOUT, ">&STDOUT");
    open (STDOUT, ">$output_file") or die "can't redirect stdout: $!";
    open (OLDERR, ">&STDERR");
    open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";
}

sub print_output
{
  open( TEST_OUTPUT, "< $output_file" ) or die "Can't open $output_file : $!";
  while( <TEST_OUTPUT> ) {
    print;
  }
  close (TEST_OUTPUT);
}

sub cleanup
{
  # Retrieve spawned process id from the log file.
  my @pids = ();
  
  if (! open (TEST_OUTPUT, "<$output_file"))
  {
      print STDERR "ERROR: Could not open $output_file\n";
      return -1;
  }
  
  my $i = 0;
  
  while (<TEST_OUTPUT>)
  {
      if (m/ImR Activator: Successfully started/)
      {
          chomp $_;
          my @words = split (/=/, $_);
          push(@pids, $words[1]); 
          ++ $i;         
      }
  }
  close (TEST_OUTPUT);
  
  # Kill the MessengerService processes spawed by activator 
  # if they are still running.
  my $size = @pids;
  
  for (my $i = 0; $i < $size; ++$i)
  {
      if (kill ('KILL', $pids[$i]) == 1)
       {
         print STDERR "ERROR: Killed the test server (pid=$pids[$i]) " .
           "that was still running after IMR shutdown. \n";
         $status = 1;
       }
  }
}


sub do_test
{
    unlink $combined_ior;

    my $start_time = time();
print STDERR "Starting comb -f $start_conf\n";
    # First we start all the servers, including the test server
    $COMB->Arguments("-f $start_conf");
    my $ret = $COMB->Spawn();
    if ($ret != 0) {
        print "ERROR : spawning combined service.\n";
        return $ret;
    }

    ## Wait a little bit for everything to get started
    sleep(2);
print STDERR "Starting test_server -orbuseimr 1 $imr_initref\n";

    $SERV->Arguments("-orbuseimr 1 $imr_initref");
    my $ret = $SERV->Spawn();
    if ($ret != 0) {
        print "ERROR : spawning test server.\n";
        return $ret;
    }

    ## Wait a little bit for everything to get started
    sleep(2);
print STDERR "Starting client\n";

    # The client should pass the simple test
    $CLI->Arguments("$test_initref/TestObject1");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 1.\n";
        return $ret;
    }

    # The client should pass the simple test again, this time with obj #2
    $CLI->Arguments("$test_initref/TestObject2");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 2.\n";
        return $ret;
    }
print STDERR "Starting imr\n";

    # The server was autoregistered without any start information. We
    # need to update the registration with a command line so that the
    # activator can be used to re-launch it.
    $IMRUTIL->Arguments("$imr_initref  update TestObject1 -l $hostname -c \"$test_server -orbuseimr 1 $imr_initref\"");
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Updating TestObject1 cmdline.\n";
        return $ret;
    }

    # The new command line should now be registered, but only for TestObject1
    $IMRUTIL->Arguments("$imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        return $ret;
    }

    # Now we can kill the server.
    $IMRUTIL->Arguments("$imr_initref shutdown TestObject1");
    $ret = $IMRUTIL->SpawnWaitKill(10);
    if ($ret != 0) {
        print "ERROR : Shutting down test server.\n";
        return $ret;
    }
       
    # Both TestObject1 and TestObject2 should now show up as "not running"
    $IMRUTIL->Arguments("$imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        return $ret;
    }
print STDERR "Starting client\n";

    # The client should pass the simple test again, because the Activator will
    # restart test_server
    $CLI->Arguments("$test_initref/TestObject1");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 3.\n";
        return $ret;
    }

    # The client should pass the simple test again with obj #2, because both
    # are started by the server
    $CLI->Arguments("$test_initref/TestObject2");
    $ret = $CLI->SpawnWaitKill(5);
    if ($ret != 0) {
        print "ERROR : spawning test client 4.\n";
        return $ret;
    }

    # We have to kill the server, since $SERV most likely does
    # not refer to the actual process anymore since a new server
    # was started by the ImR Activator.
    $IMRUTIL->Arguments("$imr_initref shutdown TestObject1");
    $ret = $IMRUTIL->SpawnWaitKill(10);
    if ($ret != 0) {
        print "ERROR : Shutting down test server.\n";
        return $ret;
    }

    $ret = $COMB->TerminateWaitKill(10);
    if ($ret != 0) {
        print "ERROR : Terminating combined service.\n";
        return $ret;
    }

    $ret = $SERV->TerminateWaitKill(5);
    if ($ret != 0) {
        print "ERROR : Terminating test server.\n";
        return $ret;
    }

    unlink $combined_ior;

    return $ret;
}


$status  = 0;

redirect_output();

if (do_test () == -1) {
    $status = 1;
}

cleanup ();

restore_output();

print_output ();

unlink $output_file;

# Regardless of the return value, ensure that the processes
# are terminated before exiting
$CLI->Kill();
$COMB->Kill();
$SERV->Kill();

exit $status;

