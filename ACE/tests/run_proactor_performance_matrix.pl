eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use strict;
use warnings;

use Cwd qw(abs_path);
use Encode qw(encode);
use File::Basename qw(dirname);
use File::Copy qw(copy);
use File::Path qw(make_path);
use Getopt::Long qw(GetOptions);
use MIME::Base64 qw(encode_base64);
use POSIX qw(WNOHANG strftime);
use Text::ParseWords qw(shellwords);
use Time::HiRes qw(sleep time);

# Return a default value when the requested setting is undefined.
sub value_or_default {
  my ($value, $default) = @_;
  return defined $value ? $value : $default;
}

my $script_dir = abs_path(dirname($0));
my $ace_root = abs_path("$script_dir/..");
my $log_root = "$script_dir/log/proactor_performance";
my $run_id = $ENV{RUN_ID} || strftime('%Y%m%d-%H%M%S', localtime);
my $run_dir = "$log_root/$run_id";
my $results_tsv = "$run_dir/results.tsv";
my $summary_txt = "$run_dir/summary.txt";
my $is_windows = ($^O eq 'MSWin32') ? 1 : 0;

$ENV{ACE_ROOT} = $ace_root;
if ($is_windows) {
  $ENV{PATH} = join(
    ';',
    grep { defined $_ && $_ ne '' }
      ("$ace_root/lib", $script_dir, $ENV{PATH})
  );
} else {
  $ENV{LD_LIBRARY_PATH} = join(
    ':',
    grep { defined $_ && $_ ne '' }
      ("$ace_root/lib", $script_dir, $ENV{LD_LIBRARY_PATH})
  );
}

my $network_timeout_secs = value_or_default($ENV{TIMEOUT_SECS_NETWORK}, 300);
my $stress_timeout_secs = value_or_default($ENV{TIMEOUT_SECS_STRESS}, 120);
my $base_port = value_or_default($ENV{BASE_PORT}, 26000);
my $fail_fast = value_or_default($ENV{FAIL_FAST}, 0);
my $include_ipv6 = value_or_default($ENV{INCLUDE_IPV6}, 'auto');
my $include_experimental_udp = value_or_default($ENV{INCLUDE_EXPERIMENTAL_UDP}, 0);
my @test_wrapper = defined $ENV{TEST_WRAPPER} && $ENV{TEST_WRAPPER} ne ''
  ? shellwords($ENV{TEST_WRAPPER})
  : ();

my @requested_scenarios;
my @requested_backends;
my $list_only = 0;
my $help = 0;

my @scenario_order;
my %scenario_type;
my %scenario_args;
my %scenario_desc;
my @skipped_notes;
my @rows;
my $overall_rc = 0;
my $port_offset = 0;

my @column_order = qw(
  benchmark scenario backend status description transport family sessions
  messages payload depth threads stress_mode elapsed_sec send_mib_per_sec
  recv_mib_per_sec completions_per_sec avg_write_us avg_read_us
  avg_completion_us udp_loss_pct udp_lost callbacks callbacks_per_sec
  schedule_failures stdout_log native_log udp_rcvbuf_req udp_sndbuf_req
  udp_rcvbuf_actual udp_sndbuf_actual
);

# Print command-line usage and supported environment variables.
sub usage {
  print <<'EOF';
Usage: ./run_proactor_performance_matrix.pl [options]

Options:
  --scenario <name>   Run only the named scenario. May be repeated.
  --backend <name>    Run only the named backend. May be repeated.
  --list              Print the resolved scenario/backend matrix and exit.
  --help              Show this help.

Environment:
  RUN_ID=<label>            Output directory label under log/proactor_performance.
  BASE_PORT=26000           Starting port for network scenarios.
  TIMEOUT_SECS_NETWORK=300  Per-scenario timeout for network runs.
  TIMEOUT_SECS_STRESS=120   Per-scenario timeout for stress runs.
  FAIL_FAST=0|1             Stop after the first failed scenario.
  INCLUDE_IPV6=auto|0|1     Auto-detect loopback IPv6 support, force skip, or force run.
  INCLUDE_EXPERIMENTAL_UDP=0|1
                            Include UDP overload scenarios that may
                            intentionally fail the benchmark's
                            progress-timeout validation.
  TEST_WRAPPER=<command>     Prefix each benchmark command, for example
                            TEST_WRAPPER="setarch x86_64 -R".
EOF
}

# Exit with an error if a required file is missing.
sub require_file {
  my ($path, $message) = @_;
  if (!-e $path) {
    print STDERR "error: $message\n";
    exit 1;
  }
}

# Return non-zero if the requested value appears in the list.
sub contains_value {
  my ($needle, @values) = @_;
  for my $value (@values) {
    return 1 if $value eq $needle;
  }
  return 0;
}

# Quote an argument for a POSIX shell command line.
sub shell_quote {
  my ($value) = @_;
  return "''" if !defined $value || $value eq '';
  return $value if $value =~ /\A[-+A-Za-z0-9_.,\/:=]+\z/;
  $value =~ s/'/'\\''/g;
  return "'$value'";
}

# Quote an argument for embedding in a PowerShell command line.
sub powershell_quote {
  my ($value) = @_;
  $value = '' if !defined $value;
  $value =~ s/'/''/g;
  return "'$value'";
}

my $queried_build_macros = 0;
my %build_macros;

# Return non-zero if the current ACE build enables io_uring support.
sub has_io_uring {
  return 0 if $is_windows;
  if (-e "$ace_root/ace/config.h") {
    open my $fh, '<', "$ace_root/ace/config.h" or return 0;
    while (<$fh>) {
      return 1 if /^\s*#\s*define\s+ACE_HAS_IO_URING/;
    }
    close $fh;
  }
  if (-e "$ace_root/include/makeinclude/platform_macros.GNU") {
    open my $fh, '<', "$ace_root/include/makeinclude/platform_macros.GNU" or return 0;
    while (<$fh>) {
      return 1 if /^\s*uring\s*=\s*1(?:\s|$)/;
    }
    close $fh;
  }
  return 0;
}

# Query the C++ preprocessor for the active ACE build macros.
sub query_build_macros {
  return %build_macros if $queried_build_macros;
  $queried_build_macros = 1;

  return %build_macros if $is_windows;

  my $cxx = value_or_default($ENV{CXX}, '');
  $cxx = 'g++' if $cxx eq '';
  my @cmd = shellwords($cxx);
  push @cmd, shellwords($ENV{CPPFLAGS}) if defined $ENV{CPPFLAGS};
  push @cmd, ('-dM', '-E', "-I$ace_root", '-D_GNU_SOURCE');
  push @cmd, '-DACE_HAS_IO_URING' if has_io_uring();
  push @cmd, ('-include', 'ace/config-all.h', '-x', 'c++', '/dev/null');

  my $fh;
  if (!open $fh, '-|', @cmd) {
    print STDERR "warning: unable to query ACE build macros: $!\n";
    return %build_macros;
  }

  while (my $line = <$fh>) {
    if ($line =~ /^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)\b/) {
      $build_macros{$1} = 1;
    }
  }

  if (!close $fh) {
    print STDERR "warning: ACE build macro query failed; backend availability may be incomplete\n";
    %build_macros = ();
  }

  return %build_macros;
}

# Return non-zero if the queried build macros include the named define.
sub build_has_define {
  my ($name) = @_;
  my %macros = query_build_macros();
  return exists $macros{$name} ? 1 : 0;
}

# Resolve the list of Proactor backends expected on this platform.
sub candidate_backends {
  my @backends;
  if ($is_windows) {
    @backends = qw(win32);
  } elsif (build_has_define('ACE_HAS_AIO_CALLS')) {
    push @backends, 'aiocb';
    push @backends, 'sig' if build_has_define('ACE_HAS_POSIX_REALTIME_SIGNALS');
    push @backends, 'sun' if build_has_define('sun');
    push @backends, 'cb' if !build_has_define('ACE_HAS_BROKEN_SIGEVENT_STRUCT');
    push @backends, 'uring' if build_has_define('ACE_HAS_IO_URING');
  }
  return @backends;
}

# Resolve a test binary path, including the Windows .exe suffix.
sub resolve_test_binary {
  my ($path) = @_;
  return $path if -e $path;
  return "$path.exe" if $is_windows && -e "$path.exe";
  return $path;
}

# Translate Perl wait status values into process exit codes.
sub interpret_wait_status {
  my ($status) = @_;
  return 255 if !defined $status;
  return 128 + ($status & 127) if ($status & 127);
  return $status >> 8;
}

# Run a command with a timeout using PowerShell on Windows.
sub run_command_with_timeout_windows {
  my ($cmd_ref, $stdout_log, $timeout) = @_;

  my $stderr_log = "$stdout_log.stderr";
  unlink $stdout_log, $stderr_log;

  my $arg_list = join(', ', map { powershell_quote($_) } @$cmd_ref[1 .. $#$cmd_ref]);
  my $ps = join "\n",
    "\$ErrorActionPreference = 'Stop'",
    "\$ProgressPreference = 'SilentlyContinue'",
    '$typeDef = "using System.Runtime.InteropServices; public static class CodexWindowsErrorMode { [DllImport(""kernel32.dll"")] public static extern uint SetErrorMode(uint mode); }"',
    'Add-Type -TypeDefinition $typeDef | Out-Null',
    '[CodexWindowsErrorMode]::SetErrorMode(0x0001 -bor 0x0002 -bor 0x8000) | Out-Null',
    '$stdoutLog = ' . powershell_quote($stdout_log),
    '$stderrLog = ' . powershell_quote($stderr_log),
    '$workingDir = ' . powershell_quote($script_dir),
    '$timeout = ' . int($timeout),
    '$process = Start-Process -FilePath ' . powershell_quote($cmd_ref->[0])
      . ' -ArgumentList @(' . $arg_list . ')'
      . ' -WorkingDirectory $workingDir'
      . ' -RedirectStandardOutput $stdoutLog'
      . ' -RedirectStandardError $stderrLog'
      . ' -PassThru',
    'try {',
    '  Wait-Process -Id $process.Id -Timeout $timeout -ErrorAction Stop',
    '  $process.Refresh()',
    '  $rc = [int]([uint32]$process.ExitCode)',
    '} catch {',
    '  Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue',
    '  $rc = 124',
    '}',
    'if (Test-Path -LiteralPath $stderrLog) {',
    '  if ((Get-Item -LiteralPath $stderrLog).Length -gt 0) {',
    '    Get-Content -LiteralPath $stderrLog | Add-Content -LiteralPath $stdoutLog',
    '  }',
    '  Remove-Item -LiteralPath $stderrLog -Force -ErrorAction SilentlyContinue',
    '}',
    'exit $rc';
  my $encoded_ps = encode_base64(encode('UTF-16LE', $ps), '');

  system('powershell.exe',
         '-NoProfile',
         '-ExecutionPolicy',
         'Bypass',
         '-EncodedCommand',
         $encoded_ps);

  return interpret_wait_status($?);
}

# Run a command with a timeout on the current platform.
sub run_command_with_timeout {
  my ($cmd_ref, $stdout_log, $timeout) = @_;

  if ($is_windows) {
    return run_command_with_timeout_windows($cmd_ref, $stdout_log, $timeout);
  }

  my $pid = fork();
  if (!defined $pid) {
    die "fork failed: $!";
  }

  if ($pid == 0) {
    chdir $script_dir or do {
      print STDERR "failed to chdir to $script_dir: $!\n";
      exit 127;
    };
    open STDOUT, '>', $stdout_log or do {
      print STDERR "failed to open $stdout_log: $!\n";
      exit 127;
    };
    open STDERR, '>&', \*STDOUT or exit 127;
    exec { $cmd_ref->[0] } @$cmd_ref or do {
      print STDERR "exec failed for $cmd_ref->[0]: $!\n";
      exit 127;
    };
  }

  my $deadline = time() + $timeout;
  while (1) {
    my $wait_pid = waitpid($pid, WNOHANG);
    if ($wait_pid == $pid) {
      return interpret_wait_status($?);
    }
    if ($wait_pid == -1) {
      return 255;
    }

    if (time() >= $deadline) {
      kill 'TERM', $pid;
      my $grace_deadline = time() + 2;
      while (time() < $grace_deadline) {
        my $grace_pid = waitpid($pid, WNOHANG);
        return interpret_wait_status($?) if $grace_pid == $pid;
        last if $grace_pid == -1;
        sleep 0.1;
      }
      kill 'KILL', $pid;
      waitpid($pid, 0);
      return 124;
    }

    sleep 0.1;
  }
}

# Register one benchmark scenario in the execution order.
sub add_scenario {
  my ($name, $type, $args, $desc) = @_;
  push @scenario_order, $name;
  $scenario_type{$name} = $type;
  $scenario_args{$name} = $args;
  $scenario_desc{$name} = $desc;
}

# Convert an optional numeric field to a comparable number.
sub num_value {
  my ($value) = @_;
  return 0 if !defined $value || $value eq '';
  return $value + 0;
}

# Append a result row to the in-memory table and TSV output.
sub append_result_row {
  my (%row) = @_;
  push @rows, \%row;

  open my $fh, '>>', $results_tsv or die "open $results_tsv failed: $!";
  print {$fh} join("\t", map { defined $row{$_} ? $row{$_} : '' } @column_order), "\n";
  close $fh or die "close $results_tsv failed: $!";
}

# Record a skipped benchmark row.
sub record_skip {
  my ($benchmark, $scenario, $backend, $message) = @_;
  append_result_row(
    benchmark => $benchmark,
    scenario => $scenario,
    backend => $backend,
    status => 'skip',
    description => $scenario_desc{$scenario},
    stdout_log => $message,
  );
}

# Record a failed benchmark row.
sub record_failure {
  my ($benchmark, $scenario, $backend, $stdout_log, $native_log) = @_;
  append_result_row(
    benchmark => $benchmark,
    scenario => $scenario,
    backend => $backend,
    status => 'fail',
    description => $scenario_desc{$scenario},
    stdout_log => $stdout_log,
    native_log => $native_log,
  );
}

# Record a successful network benchmark row.
sub record_network_success {
  my ($scenario, $backend, $fields, $stdout_log, $native_log) = @_;
  append_result_row(
    benchmark => 'network',
    scenario => $scenario,
    backend => $backend,
    status => 'pass',
    description => $scenario_desc{$scenario},
    transport => $fields->{transport},
    family => $fields->{family},
    sessions => $fields->{sessions},
    messages => $fields->{messages},
    payload => $fields->{payload},
    depth => $fields->{depth},
    threads => $fields->{threads},
    elapsed_sec => $fields->{elapsed_sec},
    send_mib_per_sec => $fields->{send_mib_per_sec},
    recv_mib_per_sec => $fields->{recv_mib_per_sec},
    completions_per_sec => $fields->{completions_per_sec},
    avg_write_us => $fields->{avg_write_us},
    avg_read_us => $fields->{avg_read_us},
    avg_completion_us => $fields->{avg_completion_us},
    udp_loss_pct => $fields->{udp_loss_pct},
    udp_lost => $fields->{udp_lost},
    stdout_log => $stdout_log,
    native_log => $native_log,
    udp_rcvbuf_req => $fields->{udp_rcvbuf_req},
    udp_sndbuf_req => $fields->{udp_sndbuf_req},
    udp_rcvbuf_actual => $fields->{udp_rcvbuf_actual},
    udp_sndbuf_actual => $fields->{udp_sndbuf_actual},
  );
}

# Record a successful stress benchmark row.
sub record_stress_success {
  my ($scenario, $backend, $fields, $stdout_log, $native_log) = @_;
  append_result_row(
    benchmark => 'stress',
    scenario => $scenario,
    backend => $backend,
    status => 'pass',
    description => $scenario_desc{$scenario},
    stress_mode => $fields->{mode},
    elapsed_sec => $fields->{elapsed_sec},
    callbacks => $fields->{callbacks},
    callbacks_per_sec => $fields->{callbacks_per_sec},
    schedule_failures => $fields->{schedule_failures},
    stdout_log => $stdout_log,
    native_log => $native_log,
  );
}

# Parse PERF_RESULT key-value pairs from one output line.
sub parse_perf_fields {
  my ($line) = @_;
  my %fields;
  while ($line =~ /\b([A-Za-z0-9_]+)=([^\s]+)/g) {
    $fields{$1} = $2;
  }
  return \%fields;
}

# Extract PERF_RESULT lines for the requested benchmark from a log.
sub extract_perf_lines {
  my ($native_log, $benchmark) = @_;
  return () if !-f $native_log;

  open my $fh, '<', $native_log or die "open $native_log failed: $!";
  my @lines;
  while (my $line = <$fh>) {
    next if $line !~ /PERF_RESULT/;
    next if $line !~ /\bbenchmark=\Q$benchmark\E\b/;
    $line =~ s/^.*PERF_RESULT /PERF_RESULT /;
    chomp $line;
    push @lines, $line;
  }
  close $fh or die "close $native_log failed: $!";
  return @lines;
}

# Return non-zero if IPv6 loopback is available on this host.
sub ipv6_loopback_available {
  if ($is_windows) {
    my $status = system('ping', '-n', '1', '::1');
    return $status == 0 ? 1 : 0;
  }

  my $path = '/proc/net/if_inet6';
  return 0 if !-r $path;

  open my $fh, '<', $path or return 0;
  while (my $line = <$fh>) {
    if ($line =~ /^00000000000000000000000000000001\s+\S+\s+\S+\s+\S+\s+\S+\s+lo\b/) {
      close $fh;
      return 1;
    }
  }
  close $fh;
  return 0;
}

# Generate and print the final benchmark summary report.
sub generate_summary {
  open my $fh, '>', $summary_txt or die "open $summary_txt failed: $!";

  my $total = scalar @rows;
  my $pass = scalar grep { value_or_default($_->{status}, '') eq 'pass' } @rows;
  my $fail = scalar grep { value_or_default($_->{status}, '') eq 'fail' } @rows;
  my $skip = scalar grep { value_or_default($_->{status}, '') eq 'skip' } @rows;

  print {$fh} "Run Directory: $run_dir\n";
  print {$fh} "Results TSV: $results_tsv\n";
  print {$fh} "Latency note: avg_write_us and avg_completion_us are wall-clock service-time proxies, not ping-pong RTT.\n";
  print {$fh} "UDP buffer note: Linux commonly reports doubled SO_RCVBUF/SO_SNDBUF values via getsockopt; compare granted values consistently within this run.\n\n";
  print {$fh} "Status: total=$total pass=$pass fail=$fail skip=$skip\n";

  if (@skipped_notes) {
    print {$fh} "\nSkipped Notes:\n";
    print {$fh} "  $_\n" for @skipped_notes;
  }

  print {$fh} "\nStress Timing:\n";
  print {$fh} "mode\tbackend\telapsed_sec\tcallbacks_per_sec\tschedule_failures\n";
  my @stress_rows = sort {
    value_or_default($a->{stress_mode}, '') cmp value_or_default($b->{stress_mode}, '')
      || num_value($a->{elapsed_sec}) <=> num_value($b->{elapsed_sec})
  } grep {
    value_or_default($_->{benchmark}, '') eq 'stress'
      && value_or_default($_->{status}, '') eq 'pass'
  } @rows;
  for my $row (@stress_rows) {
    print {$fh} join(
      "\t",
      map { defined $row->{$_} ? $row->{$_} : '' }
        qw(stress_mode backend elapsed_sec callbacks_per_sec schedule_failures)
    ), "\n";
  }

  for my $scenario (@scenario_order) {
    next if value_or_default($scenario_type{$scenario}, '') ne 'network';

    print {$fh} "\nScenario: $scenario\n";
    print {$fh} "Description: $scenario_desc{$scenario}\n";
    print {$fh} "backend\tstatus\tfamily\ttransport\telapsed_sec\trecv_mib_per_sec\tavg_write_us\tavg_completion_us\tudp_loss_pct\tudp_rcvbuf_actual\tudp_sndbuf_actual\n";

    my @scenario_rows = sort {
      value_or_default($a->{status}, '') cmp value_or_default($b->{status}, '')
        || num_value($b->{recv_mib_per_sec}) <=> num_value($a->{recv_mib_per_sec})
    } grep {
      value_or_default($_->{benchmark}, '') eq 'network'
        && value_or_default($_->{scenario}, '') eq $scenario
    } @rows;

    my ($best_throughput, $best_latency);
    for my $row (@scenario_rows) {
      print {$fh} join(
        "\t",
        map { defined $row->{$_} ? $row->{$_} : '' }
          qw(
            backend status family transport elapsed_sec recv_mib_per_sec
            avg_write_us avg_completion_us udp_loss_pct udp_rcvbuf_actual
            udp_sndbuf_actual
          )
      ), "\n";

      next if value_or_default($row->{status}, '') ne 'pass';
      if (!defined $best_throughput
          || num_value($row->{recv_mib_per_sec}) > num_value($best_throughput->{recv_mib_per_sec})) {
        $best_throughput = $row;
      }
      if (value_or_default($row->{avg_write_us}, '') ne ''
          && (!defined $best_latency
              || num_value($row->{avg_write_us}) < num_value($best_latency->{avg_write_us}))) {
        $best_latency = $row;
      }
    }

    if (defined $best_throughput) {
      printf {$fh} "Throughput leader: %s (recv %.6f MiB/s)\n",
        $best_throughput->{backend},
        num_value($best_throughput->{recv_mib_per_sec});
    }
    if (defined $best_latency) {
      printf {$fh} "Latency-proxy leader: %s (avg_write %.3f us)\n",
        $best_latency->{backend},
        num_value($best_latency->{avg_write_us});
    }
  }

  close $fh or die "close $summary_txt failed: $!";

  open my $readback, '<', $summary_txt or die "open $summary_txt failed: $!";
  while (my $line = <$readback>) {
    print $line;
  }
  close $readback or die "close $summary_txt failed: $!";
}

Getopt::Long::Configure('no_auto_abbrev');
my $ok = GetOptions(
  'scenario=s@' => \@requested_scenarios,
  'backend=s@' => \@requested_backends,
  'list' => \$list_only,
  'help' => \$help,
);

if (!$ok || $help) {
  usage();
  exit($ok ? 0 : 2);
}

require_file(
  "$ace_root/ace/config.h",
  "missing $ace_root/ace/config.h; build ACE before running the performance matrix",
);
if (!$is_windows) {
  require_file(
    "$ace_root/include/makeinclude/platform_macros.GNU",
    "missing $ace_root/include/makeinclude/platform_macros.GNU; build ACE before running the performance matrix",
  );
}
require_file(
  resolve_test_binary("$script_dir/Proactor_Stress_Test"),
  "missing $script_dir/Proactor_Stress_Test; build the Proactor tests before running the matrix",
);
require_file(
  resolve_test_binary("$script_dir/Proactor_Network_Performance_Test"),
  "missing $script_dir/Proactor_Network_Performance_Test; build the Proactor tests before running the matrix",
);

my @backends = candidate_backends();
if (@requested_backends) {
  my @filtered_backends;
  for my $backend (@requested_backends) {
    if (!contains_value($backend, @backends)) {
      print STDERR "error: unsupported backend selection: $backend\n";
      exit 2;
    }
    push @filtered_backends, $backend;
  }
  @backends = @filtered_backends;
}

if (!@backends) {
  print STDERR "error: backend resolution produced empty matrix\n";
  exit 2;
}

my $ipv6_enabled = 0;
if ($include_ipv6 eq 'auto') {
  $ipv6_enabled = ipv6_loopback_available() ? 1 : 0;
} elsif ($include_ipv6 eq '0') {
  $ipv6_enabled = 0;
} elsif ($include_ipv6 eq '1') {
  $ipv6_enabled = 1;
} else {
  print STDERR "error: INCLUDE_IPV6 must be auto, 0, or 1\n";
  exit 2;
}

add_scenario('stress_recursive', 'stress', '',
  'Recursive timer dispatch stress; lower elapsed is better.');
add_scenario('tcp_1x1_light_v4', 'network', '-4 -n 1 -m 50000 -b 64 -w 1 -T 1',
  'TCP IPv4 one-to-one, single-thread, small payload latency proxy.');
add_scenario('tcp_1x16_moderate_st_v4', 'network', '-4 -n 16 -m 4096 -b 512 -w 4 -T 1',
  'TCP IPv4 many sessions on one proactor thread.');
add_scenario('tcp_1x16_moderate_mt_v4', 'network', '-4 -n 16 -m 4096 -b 512 -w 4 -T 4',
  'TCP IPv4 same moderate workload with four proactor threads.');
add_scenario('tcp_16x16_heavy_mt_v4', 'network', '-4 -n 16 -m 2048 -b 8192 -w 16 -T 8',
  'TCP IPv4 heavier bulk-transfer workload with deeper write pipelining.');
add_scenario('udp_1x1_light_v4', 'network', '-u -4 -n 1 -m 50000 -b 64 -w 1 -T 1',
  'UDP IPv4 one-to-one, single-thread, small datagrams latency proxy.');
add_scenario('udp_1x16_moderate_mt_buf4m_v4', 'network',
  '-u -4 -n 16 -m 4096 -b 512 -w 4 -T 4 -R 4194304 -S 4194304',
  'UDP IPv4 moderate multi-threaded workload with explicit 4 MiB socket buffers.');
add_scenario('udp_32x32_heavy_mt_buf4m_v4', 'network',
  '-u -4 -n 32 -m 2048 -b 1400 -w 8 -T 8 -R 4194304 -S 4194304',
  'UDP IPv4 heavier datagram workload with explicit 4 MiB socket buffers.');

if ($include_experimental_udp) {
  add_scenario('udp_1x16_moderate_st_v4', 'network', '-u -4 -n 16 -m 4096 -b 512 -w 4 -T 1',
    'UDP IPv4 many sessions on one proactor thread.');
  add_scenario('udp_1x16_moderate_mt_v4', 'network', '-u -4 -n 16 -m 4096 -b 512 -w 4 -T 4',
    'UDP IPv4 same moderate workload with four proactor threads.');
  add_scenario('udp_1x16_moderate_mt_buf1m_v4', 'network',
    '-u -4 -n 16 -m 4096 -b 512 -w 4 -T 4 -R 1048576 -S 1048576',
    'UDP IPv4 moderate multi-threaded workload with explicit 1 MiB socket buffers.');
  add_scenario('udp_32x32_heavy_mt_v4', 'network', '-u -4 -n 32 -m 2048 -b 1400 -w 8 -T 8',
    'UDP IPv4 heavier datagram throughput workload near MTU-sized payloads.');
  add_scenario('udp_32x32_heavy_mt_buf1m_v4', 'network',
    '-u -4 -n 32 -m 2048 -b 1400 -w 8 -T 8 -R 1048576 -S 1048576',
    'UDP IPv4 heavier datagram workload with explicit 1 MiB socket buffers.');
} else {
  push @skipped_notes,
    'Experimental UDP overload scenarios omitted by default; set INCLUDE_EXPERIMENTAL_UDP=1 to run rows that may stall/fail when socket buffers are too small.';
}

if ($ipv6_enabled) {
  add_scenario('tcp_ipv6_moderate_mt_v6', 'network', '-6 -n 8 -m 4096 -b 512 -w 4 -T 4',
    'TCP IPv6 moderate workload on loopback.');
  add_scenario('udp_ipv6_moderate_mt_buf4m_v6', 'network',
    '-u -6 -n 8 -m 4096 -b 512 -w 4 -T 4 -R 4194304 -S 4194304',
    'UDP IPv6 moderate workload on loopback with explicit 4 MiB socket buffers.');
  if ($include_experimental_udp) {
    add_scenario('udp_ipv6_moderate_mt_v6', 'network', '-u -6 -n 8 -m 4096 -b 512 -w 4 -T 4',
      'UDP IPv6 moderate workload on loopback.');
  }
} else {
  push @skipped_notes,
    'IPv6 scenarios skipped because loopback ::1 was not detected or IPv6 was disabled.';
}

if (@requested_scenarios) {
  my @filtered_scenarios;
  for my $scenario (@requested_scenarios) {
    if (!contains_value($scenario, @scenario_order)) {
      print STDERR "error: unsupported scenario selection: $scenario\n";
      exit 2;
    }
    push @filtered_scenarios, $scenario;
  }
  @scenario_order = @filtered_scenarios;
}

make_path($run_dir);
open my $results_fh, '>', $results_tsv or die "open $results_tsv failed: $!";
print {$results_fh} join("\t", @column_order), "\n";
close $results_fh or die "close $results_tsv failed: $!";

if ($list_only) {
  print "run_dir=$run_dir\n";
  print 'backends=' . join(' ', @backends) . "\n";
  print 'test_wrapper=' . join(' ', @test_wrapper) . "\n" if @test_wrapper;
  print "ipv6_enabled=$ipv6_enabled\n";
  for my $scenario (@scenario_order) {
    my $args = $scenario_args{$scenario} ne '' ? $scenario_args{$scenario} : '<none>';
    print "scenario $scenario type=$scenario_type{$scenario} args=$args\n";
  }
  exit 0;
}

# Run one Proactor_Stress_Test backend instance and record its metrics.
sub run_stress_case {
  my ($scenario, $backend) = @_;

  my $binary = resolve_test_binary("$script_dir/Proactor_Stress_Test");
  my $native_log = "$script_dir/log/Proactor_Stress_Test.log";
  my $stdout_log = "$run_dir/Proactor_Stress_Test.$backend.stdout-stderr.log";
  my $archived_native_log = "$run_dir/Proactor_Stress_Test.$backend.log";

  unlink $native_log, $stdout_log, $archived_native_log;

  my @cmd = ($binary, '-t', $backend);
  my @run_cmd = (@test_wrapper, @cmd);
  print "[RUN ] $scenario backend=$backend\n";
  print '       command:' . join('', map { ' ' . shell_quote($_) } @run_cmd) . "\n";

  my $rc = run_command_with_timeout(\@run_cmd, $stdout_log, $stress_timeout_secs);

  if (-f $native_log) {
    copy($native_log, $archived_native_log)
      or die "copy $native_log -> $archived_native_log failed: $!";
  }

  if ($rc == 0 && -f $archived_native_log) {
    open my $fh, '<', $archived_native_log;
    my $content = do { local $/; <$fh> };
    close $fh;
    if ($content =~ /IPv6 is not supported by ACE on this platform/i) {
      print "[SKIP] $scenario backend=$backend (IPv6 not supported by ACE)\n";
      record_skip('stress', $scenario, $backend, 'IPv6 not supported by ACE');
      return;
    }
  }

  if ($rc != 0) {
    print "[FAIL] $scenario backend=$backend rc=$rc\n";
    record_failure('stress', $scenario, $backend, $stdout_log, $archived_native_log);
    $overall_rc = 1;
    return;
  }

  my @perf_lines = extract_perf_lines($archived_native_log, 'stress');
  if (!@perf_lines) {
    print "[FAIL] $scenario backend=$backend missing PERF_RESULT lines\n";
    record_failure('stress', $scenario, $backend, $stdout_log, $archived_native_log);
    $overall_rc = 1;
    return;
  }

  for my $line (@perf_lines) {
    record_stress_success(
      $scenario,
      $backend,
      parse_perf_fields($line),
      $stdout_log,
      $archived_native_log,
    );
  }

  print "[PASS] $scenario backend=$backend\n";
}

# Run one Proactor_Network_Performance_Test backend instance and record its metrics.
sub run_network_case {
  my ($scenario, $backend) = @_;

  my $binary = resolve_test_binary("$script_dir/Proactor_Network_Performance_Test");
  my $native_log = "$script_dir/log/Proactor_Network_Performance_Test.log";
  my $stdout_log = "$run_dir/Proactor_Network_Performance_Test.$scenario.$backend.stdout-stderr.log";
  my $archived_native_log = "$run_dir/Proactor_Network_Performance_Test.$scenario.$backend.log";
  my @extra_args = $scenario_args{$scenario} ne '' ? shellwords($scenario_args{$scenario}) : ();
  push @extra_args, ('-p', $base_port + $port_offset);
  ++$port_offset;

  unlink $native_log, $stdout_log, $archived_native_log;

  my @cmd = ($binary, '-t', $backend, @extra_args);
  my @run_cmd = (@test_wrapper, @cmd);
  print "[RUN ] $scenario backend=$backend\n";
  print '       command:' . join('', map { ' ' . shell_quote($_) } @run_cmd) . "\n";

  my $rc = run_command_with_timeout(\@run_cmd, $stdout_log, $network_timeout_secs);

  if (-f $native_log) {
    copy($native_log, $archived_native_log)
      or die "copy $native_log -> $archived_native_log failed: $!";
  }

  if ($rc == 0 && -f $archived_native_log) {
    open my $fh, '<', $archived_native_log;
    my $content = do { local $/; <$fh> };
    close $fh;
    if ($content =~ /IPv6 is not supported by ACE on this platform/i) {
      print "[SKIP] $scenario backend=$backend (IPv6 not supported by ACE)\n";
      record_skip('network', $scenario, $backend, 'IPv6 not supported by ACE');
      return;
    }
  }

  if ($rc != 0) {
    print "[FAIL] $scenario backend=$backend rc=$rc\n";
    record_failure('network', $scenario, $backend, $stdout_log, $archived_native_log);
    $overall_rc = 1;
    return;
  }

  my @perf_lines = extract_perf_lines($archived_native_log, 'network');
  if (!@perf_lines) {
    print "[FAIL] $scenario backend=$backend missing PERF_RESULT line\n";
    record_failure('network', $scenario, $backend, $stdout_log, $archived_native_log);
    $overall_rc = 1;
    return;
  }

  record_network_success(
    $scenario,
    $backend,
    parse_perf_fields($perf_lines[-1]),
    $stdout_log,
    $archived_native_log,
  );

  print "[PASS] $scenario backend=$backend\n";
}

for my $scenario (@scenario_order) {
  if (!$ipv6_enabled
      && value_or_default($scenario_args{$scenario}, '') =~ /(?:^|\s)-6(?:\s|$)/) {
    for my $backend (@backends) {
      print "[SKIP] $scenario backend=$backend (IPv6 loopback unavailable)\n";
      record_skip($scenario_type{$scenario}, $scenario, $backend, 'IPv6 loopback unavailable');
    }
    next;
  }

  for my $backend (@backends) {
    if (value_or_default($scenario_type{$scenario}, '') eq 'stress') {
      run_stress_case($scenario, $backend);
    } else {
      run_network_case($scenario, $backend);
    }

    if ($fail_fast && $overall_rc) {
      generate_summary();
      exit 1;
    }
  }
}

generate_summary();
exit($overall_rc ? 1 : 0);
