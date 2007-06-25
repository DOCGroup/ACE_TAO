eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$server_static_threads = 1;
$server_dynamic_threads = 0;
$status = 0;
$continuous = ($^O eq 'hpux');
$common_args = ($continuous ? "-ORBSvcConf continuous$PerlACE::svcconf_ext" : '');

@configurations =
  (
   {
    file => "ior_2",
    args => "-b 0",
    description => "Invoking methods on servant in thread lanes without bands",
   },
   {
    file => "ior_2",
    args => "-b 1",
    description => "Invoking methods on servant in thread lanes with bands",
   },
   {
    file => "ior_1",
    args => "-b 0",
    description => "Invoking methods on servant in thread pool without bands",
   },
   {
    file => "ior_1",
    args => "-b 1",
    description => "Invoking methods on servant in thread pool with bands",
   },
  );

sub run_test
  {
    for $test (@configurations)
      {
        unlink PerlACE::LocalFile($test->{file});
      }

    my @parms = @_;
    $arg = $parms[0];

    if (PerlACE::is_vxworks_test()) {
        $SV = new PerlACE::ProcessVX ("server", "$common_args -s $server_static_threads -d $server_dynamic_threads");
    }
    else {
        $SV = new PerlACE::Process ("server", "$common_args -s $server_static_threads -d $server_dynamic_threads");
    }

    $server = $SV->Spawn ();
    if ($server == -1)
      {
        exit $status;
      }

    for $test (@configurations)
      {
        if (PerlACE::waitforfile_timed (PerlACE::LocalFile($test->{file}),$PerlACE::wait_interval_for_process_creation ) == -1)
          {
            $server = $SV->TimedWait (1);
            if ($server == 2)
              {
                # Mark as no longer running to avoid errors on exit.
                $SV->{RUNNING} = 0;
                exit $status;
              }
            else
              {
                print STDERR "ERROR: cannot find ior file: $test->{file}\n";
                $status = 1;
                goto kill_server;
              }
          }
        print $test->{file}."\n";
      }

    $CL[$i] = new PerlACE::Process ("client", "$common_args $arg");
    $CL[$i]->Spawn ();

    $client = $CL[$i]->WaitKill (20);
    if ($client != 0)
      {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
        goto kill_server;
      }

  kill_server:

    $server = $SV->WaitKill (120);

    if ($server != 0)
      {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
      }

    for $test (@configurations)
      {
        unlink PerlACE::LocalFile($test->{file});
      }
  }

for $test (@configurations)
  {
    print STDERR "\n*************************************************************\n";
    print STDERR "$test->{description}\n";
    print STDERR "*************************************************************\n\n";

    my $file = PerlACE::LocalFile($test->{file});
    run_test ("-k file://$file $test->{args}");
  }

exit $status

