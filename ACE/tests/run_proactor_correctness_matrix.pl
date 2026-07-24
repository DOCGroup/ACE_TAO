eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use strict;
use warnings;

use Cwd qw(abs_path);
use File::Basename qw(dirname);
use File::Copy qw(copy);
use File::Path qw(make_path);
use Getopt::Long qw(GetOptions);
use POSIX qw(strftime);
use lib dirname(abs_path($0));
use Proactor_Test_Common qw(
  ace_config_has_define
  build_has_define
  contains_value
  has_io_uring
  ipv6_loopback_available
  require_file
  resolve_test_binary
  run_command_with_timeout
  shell_quote
  value_or_default
);
use Text::ParseWords qw(shellwords);

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
my @test_wrapper = defined $ENV{TEST_WRAPPER} && $ENV{TEST_WRAPPER} ne ''
  ? shellwords($ENV{TEST_WRAPPER})
  : ();

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
  TEST_WRAPPER=<command>  Prefix each test command, for example
                          TEST_WRAPPER="setarch x86_64 -R".
EOF
}

# Resolve the list of Proactor backends expected on this platform.
sub candidate_backends {
  my %build_args = (
    ace_root => $ace_root,
    is_windows => $is_windows,
    has_io_uring => has_io_uring(ace_root => $ace_root, is_windows => $is_windows),
  );

  my @backends;
  if ($is_windows) {
    @backends = qw(win32);
  } elsif (build_has_define('ACE_HAS_AIO_CALLS', %build_args)) {
    push @backends, 'aiocb';
    push @backends, 'sig' if build_has_define('ACE_HAS_POSIX_REALTIME_SIGNALS', %build_args);
    push @backends, 'sun' if build_has_define('sun', %build_args);
    push @backends, 'cb' if !build_has_define('ACE_HAS_BROKEN_SIGEVENT_STRUCT', %build_args);
    push @backends, 'uring' if build_has_define('ACE_HAS_IO_URING', %build_args);
  }
  @backends = ('default', @backends) if $include_default;
  return @backends;
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

my $ace_ipv6_enabled = ace_config_has_define('ACE_HAS_IPV6', ace_root => $ace_root) ? 1 : 0;
my $ipv6_loopback = ipv6_loopback_available(is_windows => $is_windows) ? 1 : 0;
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
  print 'test_wrapper=' . join(' ', @test_wrapper) . "\n" if @test_wrapper;
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

  my $binary = resolve_test_binary("$script_dir/$test_name", is_windows => $is_windows);
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
  my @run_cmd = (@test_wrapper, @cmd);

  unlink $native_log, $stdout_log, $archived_native_log;

  print "[RUN ] $case_label backend=$backend\n";
  print '       command:' . join('', map { ' ' . shell_quote($_) } @run_cmd) . "\n";

  my $rc = run_command_with_timeout(
    \@run_cmd,
    $stdout_log,
    $timeout_secs,
    is_windows => $is_windows,
    script_dir => $script_dir,
  );

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
