eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use strict;
use warnings;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $status = 0;
my $server = PerlACE::TestTarget::create_target (1)
  || die "Create target 1 failed\n";
my $ior_base = "chapter_test_$$.ior";
my $db_base = "chapter_test_$$.db";
my $assets_base = "chapter_test_$$.assets";
my $ior_file = $server->LocalFile ($ior_base);
my $db_file = $server->LocalFile ($db_base);
my $assets_file = $server->LocalFile ($assets_base);

for my $file ($db_file, $assets_file)
  {
    open my $handle, '>', $file or die "Cannot create $file: $!\n";
    close $handle or die "Cannot close $file: $!\n";
  }

$server->SetEnv ('CCS_DB', $db_file);
$server->SetEnv ('CCS_ASSETS', $assets_file);
$server->DeleteFile ($ior_base);

my $server_process = $server->CreateProcess ('server', "-o $ior_file");
my $server_started = 0;
if ($server_process->Spawn () != 0)
  {
    print STDERR "ERROR: cannot start server\n";
    $status = 1;
  }
else
  {
    $server_started = 1;
  }

if ($server_started && $status == 0
    && $server->WaitForFileTimed ($ior_base,
                                  $server->ProcessStartWaitInterval ()) == -1)
  {
    print STDERR "ERROR: timed out waiting for IOR file <$ior_file>\n";
    $status = 1;
  }
elsif ($server_started && $status == 0)
  {
    my $client_process = $server->CreateProcess ('client', "file://$ior_file");
    my $client_status = $client_process->SpawnWaitKill (60);
    if ($client_status != 0)
      {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
      }
  }

if ($server_started)
  {
    my $server_status = $server_process->TerminateWaitKill (5);
    if ($server_status != 0)
      {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
      }
  }

$server->DeleteFile ($ior_base);
$server->DeleteFile ($db_base);
$server->DeleteFile ($assets_base);
exit $status;
