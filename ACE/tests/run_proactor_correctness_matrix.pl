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
my $log_dir = "$script_dir/log";
my $matrix_root = "$log_dir/proactor_matrix";
my $run_id = $ENV{RUN_ID} || strftime('%Y%m%d-%H%M%S', localtime);
my $run_dir = "$matrix_root/$run_id";
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

my $timeout_secs = value_or_default($ENV{TIMEOUT_SECS}, 180);
my $base_port = value_or_default($ENV{BASE_PORT}, 20000);
my $include_default = value_or_default($ENV{INCLUDE_DEFAULT}, 0);
my $include_ipv6 = value_or_default($ENV{INCLUDE_IPV6}, 'auto');
my $fail_fast = value_or_default($ENV{FAIL_FAST}, 0);
my $run_network_udp = value_or_default($ENV{RUN_NETWORK_UDP}, 1);
my $expected_fail_backends = value_or_default($ENV{EXPECTED_FAIL_BACKENDS}, '');

my @requested_tests;
my @requested_backends;
my $list_only = 0;
my $help = 0;

# Print command-line usage and supported environment variables.
sub usage {
  print <<'EOF';
Usage: ./run_proactor_correctness_matrix.pl [options]

Options:
  --test <name>       Run only the named test case. May be repeated.
  --backend <name>    Run only the named backend. May be repeated.
  --list              Print the resolved test/backend matrix and exit.
  --help              Show this help.

Environment:
  BASE_PORT=20000         Starting port for tests that accept -p.
  TIMEOUT_SECS=180        Per-test timeout.
  RUN_ID=<label>          Output directory label under log/proactor_matrix.
  INCLUDE_DEFAULT=0|1     Include the shared "default" backend selection.
  INCLUDE_IPV6=auto|0|1   Auto-detect loopback IPv6 support, force skip, or force run.
  RUN_NETWORK_UDP=0|1     Include the UDP correctness variant of
                          Proactor_Network_Performance_Test.
  FAIL_FAST=0|1           Stop on the first failing matrix entry.
  EXPECTED_FAIL_BACKENDS  Space-separated backends whose nonzero exits
                          should be recorded as expected failures.
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

# Return non-zero if ACE/ace/config.h defines the requested macro.
sub ace_config_has_define {
  my ($name) = @_;
  my $path = "$ace_root/ace/config.h";
  return 0 if !-e $path;

  my %visited;
  my @pending = ($path);

  while (@pending) {
    my $current = pop @pending;
    next if !defined $current || $visited{$current}++;

    open my $fh, '<', $current or next;
    my $current_dir = dirname($current);
    while (my $line = <$fh>) {
      if ($line =~ /^\s*#\s*define\s+\Q$name\E(?:\s+|$)/) {
        close $fh;
        return 1;
      }

      if ($line =~ /^\s*#\s*include\s+"([^"]+)"/) {
        my $include = "$current_dir/$1";
        push @pending, abs_path($include) || $include;
      }
    }
    close $fh;
  }

  return 0;
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
  @backends = ('default', @backends) if $include_default;
  return @backends;
}

# Resolve a test binary path, including the Windows .exe suffix.
sub resolve_test_binary {
  my ($path) = @_;
  return $path if -e $path;
  return "$path.exe" if $is_windows && -e "$path.exe";
  return $path;
}

# Return non-zero if failures for this backend should be treated as expected.
sub backend_is_expected_fail {
  my ($backend) = @_;
  return $expected_fail_backends =~ /(?:^|\s)\Q$backend\E(?:\s|$)/ ? 1 : 0;
}

# Return non-zero if the test uses a listen or connect port.
sub test_needs_port {
  my ($test_name) = @_;
  return ($test_name eq 'Proactor_Network_Performance_Test'
       || $test_name eq 'Proactor_Test'
       || $test_name eq 'Proactor_Test_IPV6'
       || $test_name eq 'Proactor_UDP_Test'
       || $test_name eq 'Proactor_Scatter_Gather_Test') ? 1 : 0;
}

# Return non-zero if the case requires ACE IPv6 support.
sub case_requires_ipv6 {
  my ($case) = @_;
  return $case->{test_name} eq 'Proactor_Test_IPV6' ? 1 : 0;
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

Getopt::Long::Configure('no_auto_abbrev');
my $ok = GetOptions(
  'test=s@' => \@requested_tests,
  'backend=s@' => \@requested_backends,
  'list' => \$list_only,
  'help' => \$help,
);

if (!$ok || $help) {
  usage();
  exit($ok ? 0 : 2);
}

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

my @all_cases = (
  { test_name => 'Proactor_Contract_Test', variant => '', args => '' },
  { test_name => 'Proactor_File_Test', variant => '', args => '' },
  {
    test_name => 'Proactor_Network_Performance_Test',
    variant => 'tcp',
    args => '-n 8 -m 32 -b 512 -w 4 -T 2',
  },
  { test_name => 'Proactor_Scatter_Gather_Test', variant => '', args => '' },
  { test_name => 'Proactor_Stress_Test', variant => '', args => '' },
  { test_name => 'Proactor_Test', variant => '', args => '-n 2 -c 4 -x 16384' },
  { test_name => 'Proactor_Test_IPV6', variant => '', args => '-n 2 -c 4 -x 16384' },
  { test_name => 'Proactor_Timer_Test', variant => '', args => '' },
  { test_name => 'Proactor_UDP_Test', variant => '', args => '-n 2 -c 4 -x 16384' },
);

if ($run_network_udp) {
  push @all_cases, {
    test_name => 'Proactor_Network_Performance_Test',
    variant => 'udp',
    args => '-u -n 8 -m 32 -b 512 -w 4 -T 2',
  };
}

my @cases;
if (!@requested_tests) {
  @cases = @all_cases;
} else {
  for my $case (@all_cases) {
    my $case_label = $case->{test_name};
    $case_label .= ':' . $case->{variant} if $case->{variant} ne '';
    if (contains_value($case->{test_name}, @requested_tests)
        || contains_value($case_label, @requested_tests)) {
      push @cases, $case;
    }
  }
}

if (!@cases) {
  print STDERR "error: no test cases selected\n";
  exit 2;
}

require_file(
  "$ace_root/ace/config.h",
  "missing $ace_root/ace/config.h; configure ACE before running the matrix",
);
if (!$is_windows) {
  require_file(
    "$ace_root/include/makeinclude/platform_macros.GNU",
    "missing $ace_root/include/makeinclude/platform_macros.GNU; configure ACE before running the matrix",
  );
}

my $ace_ipv6_enabled = ace_config_has_define('ACE_HAS_IPV6') ? 1 : 0;
my $ipv6_loopback = ipv6_loopback_available() ? 1 : 0;
my $ipv6_enabled = 0;
if ($include_ipv6 eq 'auto') {
  $ipv6_enabled = ($ace_ipv6_enabled && $ipv6_loopback) ? 1 : 0;
} elsif ($include_ipv6 eq '0') {
  $ipv6_enabled = 0;
} elsif ($include_ipv6 eq '1') {
  $ipv6_enabled = 1;
} else {
  print STDERR "error: INCLUDE_IPV6 must be auto, 0, or 1\n";
  exit 2;
}

my $ipv6_skip_reason = '';
if (!$ipv6_enabled) {
  if ($include_ipv6 eq '0') {
    $ipv6_skip_reason = 'IPv6 disabled by INCLUDE_IPV6=0';
  } elsif (!$ace_ipv6_enabled) {
    $ipv6_skip_reason = 'ACE IPv6 support is disabled in ace/config.h';
  } elsif (!$ipv6_loopback) {
    $ipv6_skip_reason = 'IPv6 loopback ::1 is unavailable';
  } else {
    $ipv6_skip_reason = 'IPv6 is unavailable';
  }
}

make_path($log_dir, $run_dir);

if ($list_only) {
  print "run_dir=$run_dir\n";
  print 'backends=' . join(' ', @backends) . "\n";
  for my $case (@cases) {
    my $case_label = $case->{test_name};
    $case_label .= ':' . $case->{variant} if $case->{variant} ne '';
    my $args = $case->{args} ne '' ? $case->{args} : '<none>';
    print "case $case_label args=$args";
    if (case_requires_ipv6($case) && !$ipv6_enabled) {
      print " [skipped: $ipv6_skip_reason]";
    }
    print "\n";
    for my $backend (@backends) {
      next if case_requires_ipv6($case) && !$ipv6_enabled;
      print "  backend $backend\n";
    }
  }
  exit 0;
}

my $pass_count = 0;
my $fail_count = 0;
my $skip_count = 0;
my $xfail_count = 0;
my $port_offset = 0;

# Execute one test/backend matrix entry and archive its logs.
sub run_case {
  my ($case, $backend) = @_;

  my $test_name = $case->{test_name};
  my $variant = $case->{variant};
  my $args_string = $case->{args};
  my $case_label = $test_name;
  $case_label .= ':' . $variant if $variant ne '';

  my $binary = resolve_test_binary("$script_dir/$test_name");
  my $native_log = "$log_dir/$test_name.log";
  my $archive_base = "$run_dir/$test_name";
  $archive_base .= ".$variant" if $variant ne '';
  $archive_base .= ".$backend";
  my $stdout_log = "$archive_base.stdout-stderr.log";
  my $archived_native_log = "$archive_base.log";

  require_file($binary, "missing $binary; build the Proactor tests before running the matrix");

  my @extra_args = $args_string ne '' ? shellwords($args_string) : ();
  if (test_needs_port($test_name)) {
    push @extra_args, ('-p', $base_port + $port_offset);
    ++$port_offset;
  }

  my @cmd = ($binary, '-t', $backend, @extra_args);

  unlink $native_log, $stdout_log, $archived_native_log;

  print "[RUN ] $case_label backend=$backend\n";
  print '       command:' . join('', map { ' ' . shell_quote($_) } @cmd) . "\n";

  my $rc = run_command_with_timeout(\@cmd, $stdout_log, $timeout_secs);

  if (-f $native_log) {
    copy($native_log, $archived_native_log)
      or die "copy $native_log -> $archived_native_log failed: $!";
  }

  if ($rc == 0) {
    ++$pass_count;
    print "[PASS] $case_label backend=$backend\n";
    return 1;
  }

  if (backend_is_expected_fail($backend)) {
    ++$xfail_count;
    print "[XFAIL] $case_label backend=$backend rc=$rc\n";
    print "        stdout/stderr: $stdout_log\n";
    print "        test log: $archived_native_log\n" if -f $archived_native_log;
    return 1;
  }

  ++$fail_count;
  print STDERR "[FAIL] $case_label backend=$backend rc=$rc\n";
  print STDERR "       stdout/stderr: $stdout_log\n";
  print STDERR "       test log: $archived_native_log\n" if -f $archived_native_log;
  return 0;
}

for my $case (@cases) {
  my $case_label = $case->{test_name};
  $case_label .= ':' . $case->{variant} if $case->{variant} ne '';

  if (case_requires_ipv6($case) && !$ipv6_enabled) {
    for my $backend (@backends) {
      ++$skip_count;
      print "[SKIP] $case_label backend=$backend ($ipv6_skip_reason)\n";
    }
    next;
  }

  for my $backend (@backends) {
    if (!run_case($case, $backend) && $fail_fast) {
      print STDERR "\nSummary: pass=$pass_count fail=$fail_count xfail=$xfail_count skip=$skip_count run_dir=$run_dir\n";
      exit 1;
    }
  }
}

print "\nSummary: pass=$pass_count fail=$fail_count xfail=$xfail_count skip=$skip_count run_dir=$run_dir\n";
exit($fail_count == 0 ? 0 : 1);
