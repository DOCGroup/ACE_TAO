eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ( '../TestInf');
PerlACE::add_lib_path ( '../TestServant');

# 5 Minute server timeout.
my $server_timeout_secs = 300;

my $status = 0;

my $iorfname_prefix        = "servant";
my $num_servants           = 1;
my $num_orb_threads        = 1;
my $num_remote_clients     = 1;
my $num_csd_threads        = 1;
my $num_collocated_clients = 0;
my $num_loops              = 100;
my $use_csd                = 1;
my $scenario_id            = "UnsetScenarioId";
my $trial_id               = 1;

my $i;
my $j;
my @iorfile;

my $ARGC = @ARGV;

if ($ARGC > 0)
{
  if ($ARGC > 3)
  {
    print STDERR "ERROR: Too many command-line arguments for $0.\n";
    exit 1;
  }

  if ($ARGC < 2)
  {
    print STDERR "ERROR: Too few command-line arguments for $0.\n";
    exit 1;
  }

  if ($ARGC > 2)
  {
      $num_loops = $ARGV[2];
  }

  $scenario_id = $ARGV[0];
  $trial_id    = $ARGV[1];

  my $subtest = $scenario_id;

  if ($subtest =~ /^x_(.+)$/)
    {
      $subtest = $1;
      $use_csd = 0;
    }

  if ($subtest eq 'remote')
  {
    $num_remote_clients = 40;
  }
  elsif ($subtest eq 'collocated')
  {
    $num_remote_clients = 0;
    $num_collocated_clients = 10;
  }
  elsif ($subtest eq 'remote_orbthreads')
  {
    $num_orb_threads = 5;
    $num_remote_clients = 40;
  }
  elsif ($subtest eq 'remote_servants')
  {
    $num_servants = 5;
    $num_remote_clients = 40;
  }
  elsif ($subtest eq 'remote_csdthreads')
  {
    $num_csd_threads = 5;
    $num_servants = 5;
    $num_remote_clients = 40;
  }
  elsif ($subtest eq 'remote_big')
  {
    $num_csd_threads = 5;
    $num_servants = 10;
    $num_orb_threads = 4;
    $num_remote_clients = 40;
  }
  elsif ($subtest eq 'collocated_big')
  {
    $num_csd_threads = 5;
    $num_servants = 10;
    $num_remote_clients = 0;
    $num_collocated_clients = 40;
  }
  elsif ($subtest eq 'big')
  {
    $num_csd_threads = 5;
    $num_servants = 10;
    $num_orb_threads = 4;
    $num_remote_clients = 40;
    $num_collocated_clients = 40;
  }
  elsif ($subtest eq 'remote_huge')
  {
    $num_csd_threads = 5;
    $num_servants = 10;
    $num_orb_threads = 4;
    $num_remote_clients = 400;
    $num_loops = $num_loops / 10;
    # 15 minute server timeout
    $server_timeout_secs = 1800;
  }
  elsif ($subtest eq 'collocated_huge')
  {
    $num_csd_threads = 20;
    $num_servants = 10;
    $num_remote_clients = 0;
    $num_collocated_clients = 40;
  }
  elsif ($subtest eq 'usage')
  {
    print STDOUT "Usage: $0 [<subtest>]\n" .
                 "\n" .
                 "Supported <subtest> values:\n" .
                 "\n" .
                 "\tremote\n" .
                 "\tcollocated\n" .
                 "\tremote_orbthreads\n" .
                 "\tremote_servants\n" .
                 "\tremote_csdthreads\n" .
                 "\tremote_big\n" .
                 "\tcollocated_big\n" .
                 "\tbig\n" .
                 "\tremote_huge\n" .
                 "\tusage\n" .
                 "\n";
    exit 0;
  }
  else
  {
    print STDERR "ERROR: invalid subtest argument for $0: $subtest\n";
    exit 1;
  }
}

#Delete old ior files.
for ($i = 0; $i < $num_servants; $i++) {
  my $servant_id = sprintf("%02d", ($i + 1));
  $iorfile[$i] = PerlACE::LocalFile($iorfname_prefix . "_$servant_id.ior");
  unlink $iorfile[$i];
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server_main",
                           "-p $iorfname_prefix "        .
                           "-s $num_servants "           .
                           "-n $num_csd_threads "        .
                           "-t $num_orb_threads "        .
                           "-r $num_remote_clients "     .
                           "-c $num_collocated_clients " .
                           "-l $num_loops " .
                           "-u $use_csd " .
                           "-x $scenario_id " .
                           "-z $trial_id");
}
else {
    $SV = new PerlACE::Process("server_main",
                           "-p $iorfname_prefix "        .
                           "-s $num_servants "           .
                           "-n $num_csd_threads "        .
                           "-t $num_orb_threads "        .
                           "-r $num_remote_clients "     .
                           "-c $num_collocated_clients " .
                           "-l $num_loops " .
                           "-u $use_csd " .
                           "-x $scenario_id " .
                           "-z $trial_id");

}


$SV->Spawn();

# Wait for the servant ior files created by server.
for ($i = 0; $i < $num_servants; $i++) {
  if (PerlACE::waitforfile_timed
                       ($iorfile[$i],
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile[$i]>\n";
    $SV->Kill();
    $SV->TimedWait(1);
    exit 1;
  }
}

my $count = 0;

for ($i = 0; $i < $num_remote_clients; $i++)
{
  $client_id = $i+1;

  # Round-robin assignment of servants to clients.
  $j = $i % $num_servants;

  $CLS[$i] = new PerlACE::Process("client_main",
                                  "-i file://$iorfile[$j] ".
                                  "-n $client_id " .
                                  "-l $num_loops");

  $CLS[$i]->Spawn();
}

for ($i = 0; $i < $num_remote_clients; $i++)
{
  $client = $CLS[$i]->WaitKill(600);

  if ($client != 0)
  {
    print STDERR "ERROR: client $i returned $client\n";
    $status = 1;
  }
}


$server = $SV->WaitKill(600);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

#Delete ior files generated by this run.
for ($i = 0; $i < $num_servants; $i++) {
  unlink $iorfile[$i];
}

exit $status;
