eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("supplier.ior");
$notify_ior = PerlACE::LocalFile ("notify.ior");
$notify_out = PerlACE::LocalFile ("notify.out");

$notify_nothreads_conf = PerlACE::LocalFile ("notify_nothreads.conf");
$notify_mtdispatching_conf = PerlACE::LocalFile ("notify_mtdispatching.conf");
$notify_mtsource_conf = PerlACE::LocalFile ("notify_mtsource.conf");
$notify_mtlistener_conf = PerlACE::LocalFile ("notify_mtlistener.conf");

$status = 0;

sub check_output {
  my($file)   = shift;
  my($tag)    = shift;
  my($count)  = shift;
  my($status) = 0;

  if (open(IN, $file)) {
    my(%thread_ids) = ();
    while(<IN>) {
      my($line) = $_;
      if ($line =~ /Notify\s+\(\d+\|(\d+)\)\s*\-\s*.*$tag/) {
        $thread_ids{$1} = 1;
      }
    }
    close(IN);

    my(@ids) = keys %thread_ids;
    if ($#ids + 1 == $count) {
      $status = 1;
    }
  }
  return $status;
}


unlink $notify_ior;
unlink $notify_out;

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port");
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service");
$notify_args = "-ORBInitRef NameService=iioploc://" .
               "localhost:$port/NameService " .
               "-IORoutput $notify_ior " .
               "-ORBDebugLevel 1";

$STS = new PerlACE::Process ("Structured_Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService");
$STC = new PerlACE::Process ("Structured_Consumer");

$client_args = "-ORBInitRef NameService=iioploc://localhost:" .
               "$port/NameService";

open(STDERR, ">$notify_out");
my($old_fd) = select(STDERR);
$| = 1;
select($old_fd);

$TS->Arguments($notify_args .
               " -ORBSvcConf $notify_mtdispatching_conf");

$NS->Spawn ();
$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notify_ior, 20) == -1) {
    print STDOUT "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

print STDOUT "********* Running multi-threaded dispatching test " .
             "*******\n";

unlink $ior;
$STS->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
    print STDOUT "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
}

if ($status == 0) {
  $STC->Arguments($client_args);
  $client = $STC->SpawnWaitKill (60);
  print STDOUT "\n";

  $STS->Kill ();
  if ($client != 0) {
    print STDOUT "ERROR: Structured_Consumer did not run properly\n";
    $status = 1;
  }

  close(STDERR);
  $mt_ok = check_output($notify_out, "dispatching event", 2);

  if ($mt_ok && $status == 0) {
    $TS->Kill ();

    unlink $notify_ior;
    $TS->Arguments($notify_args .
                   " -ORBSvcConf $notify_mtsource_conf");

    unlink $notify_out;
    open(STDERR, ">$notify_out");
    my($old_fd) = select(STDERR);
    $| = 1;
    select($old_fd);
    $TS->Spawn ();

    if (PerlACE::waitforfile_timed ($notify_ior, 20) == -1) {
        print STDOUT "ERROR: waiting for the notify service to start\n";
        $TS->Kill ();
        $NS->Kill ();
        exit 1;
    }

    print STDOUT "********* Running multi-threaded consumer filter " .
                 "evaluation *******\n";

    unlink $ior;
    $STS->Spawn ();

    if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
        print STDOUT "ERROR: waiting for the supplier to start\n";
        $STS->Kill ();
        $TS->Kill ();
        $NS->Kill ();
        $status = 1;
    }

    if ($status == 0) {
      $STC->Arguments($client_args . " -c 2 -f");
      $client = $STC->SpawnWaitKill (60);
      print STDOUT "\n";

      $STS->Kill ();
      if ($client != 0) {
        print STDOUT "ERROR: Structured_Consumer did not run properly\n";
        $status = 1;
      }

      close(STDERR);
      $mt_ok = check_output($notify_out, "do_match", 2);
    }
    if ($mt_ok && $status == 0) {
      $TS->Kill ();

      unlink $notify_ior;
      $TS->Arguments($notify_args .
                     " -ORBSvcConf $notify_mtlistener_conf");

      unlink $notify_out;
      open(STDERR, ">$notify_out");
      my($old_fd) = select(STDERR);
      $| = 1;
      select($old_fd);
      $TS->Spawn ();

      if (PerlACE::waitforfile_timed ($notify_ior, 20) == -1) {
          print STDOUT "ERROR: waiting for the notify service to start\n";
          $TS->Kill ();
          $NS->Kill ();
          exit 1;
      }

      print STDOUT "********* Running multi-threaded supplier filter " .
                   "evaluation *******\n";

      unlink $ior;
      $STS->Arguments($STS->Arguments() . " -c 10 -f");
      $STS->Spawn ();

      if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
          print STDOUT "ERROR: waiting for the supplier to start\n";
          $STS->Kill ();
          $TS->Kill ();
          $NS->Kill ();
          $status = 1;
      }

      if ($status == 0) {
        $STC->Arguments($client_args . " -c 1 -e 60");
        $client = $STC->SpawnWaitKill (60);
        print STDOUT "\n";

        $STS->Kill ();
        if ($client != 0) {
          print STDOUT "ERROR: Structured_Consumer did not run properly\n";
          $status = 1;
        }

        close(STDERR);
        $mt_ok = check_output($notify_out, "Evaluating listener filter", 2);
        if (!$mt_ok) {
          $status = 1;
        }
      }
      if ($mt_ok && $status == 0) {
        $TS->Kill ();

        unlink $notify_ior;
        $TS->Arguments($notify_args .
                       " -ORBSvcConf $notify_nothreads_conf");

        unlink $notify_out;
        open(STDERR, ">$notify_out");
        my($old_fd) = select(STDERR);
        $| = 1;
        select($old_fd);
        $TS->Spawn ();

        if (PerlACE::waitforfile_timed ($notify_ior, 20) == -1) {
            print STDOUT "ERROR: waiting for the notify service to start\n";
            $TS->Kill ();
            $NS->Kill ();
            exit 1;
        }

        print STDOUT "********* Running single-threaded notification " .
                     "service *******\n";

        unlink $ior;
        $STS->Arguments($STS->Arguments() . " -c 10 -f");
        $STS->Spawn ();

        if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
            print STDOUT "ERROR: waiting for the supplier to start\n";
            $STS->Kill ();
            $TS->Kill ();
            $NS->Kill ();
            $status = 1;
        }

        if ($status == 0) {
          $STC->Arguments($client_args . " -c 2 -f -e 48");
          $client = $STC->SpawnWaitKill (60);
          print STDOUT "\n";

          $STS->Kill ();
          if ($client != 0) {
            print STDOUT "ERROR: Structured_Consumer did not run properly\n";
            $status = 1;
          }

          close(STDERR);
          $mt_ok = check_output($notify_out, "", 1);
          if (!$mt_ok) {
            $status = 1;
          }
        }
      }
    }
  }
}


close(STDERR);

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notify_ior;
unlink $notify_out;

exit $status;
