package Proactor_Test_Common;

use strict;
use warnings;

use Cwd qw(abs_path);
use Encode qw(encode);
use Exporter qw(import);
use File::Basename qw(dirname);
use MIME::Base64 qw(encode_base64);
use POSIX qw(WNOHANG);
use Text::ParseWords qw(shellwords);
use Time::HiRes qw(sleep time);

our @EXPORT_OK = qw(
  value_or_default
  require_file
  contains_value
  shell_quote
  powershell_quote
  has_io_uring
  query_build_macros
  build_has_define
  ace_config_has_define
  resolve_test_binary
  interpret_wait_status
  run_command_with_timeout_windows
  run_command_with_timeout
  ipv6_loopback_available
);

my %build_macro_cache;

# Return a default value when the requested setting is undefined.
sub value_or_default {
  my ($value, $default) = @_;
  return defined $value ? $value : $default;
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

# Return non-zero if the current ACE build enables io_uring support.
sub has_io_uring {
  my (%args) = @_;
  my $ace_root = $args{ace_root};
  my $is_windows = $args{is_windows};

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
  my (%args) = @_;
  my $ace_root = $args{ace_root};
  my $is_windows = $args{is_windows};
  my $has_io_uring = $args{has_io_uring};

  my $cache_key = join "\0",
    value_or_default($ace_root, ''),
    $is_windows ? 1 : 0,
    value_or_default($ENV{CXX}, ''),
    value_or_default($ENV{CPPFLAGS}, ''),
    $has_io_uring ? 1 : 0;
  return %{ $build_macro_cache{$cache_key} } if exists $build_macro_cache{$cache_key};

  my %build_macros;
  if ($is_windows) {
    $build_macro_cache{$cache_key} = \%build_macros;
    return %build_macros;
  }

  my $cxx = value_or_default($ENV{CXX}, '');
  $cxx = 'g++' if $cxx eq '';
  my @cmd = shellwords($cxx);
  push @cmd, shellwords($ENV{CPPFLAGS}) if defined $ENV{CPPFLAGS};
  push @cmd, ('-dM', '-E', "-I$ace_root", '-D_GNU_SOURCE');
  push @cmd, '-DACE_HAS_IO_URING' if $has_io_uring;
  push @cmd, ('-include', 'ace/config-all.h', '-x', 'c++', '/dev/null');

  my $fh;
  if (!open $fh, '-|', @cmd) {
    print STDERR "warning: unable to query ACE build macros: $!\n";
    $build_macro_cache{$cache_key} = \%build_macros;
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

  $build_macro_cache{$cache_key} = \%build_macros;
  return %build_macros;
}

# Return non-zero if the queried build macros include the named define.
sub build_has_define {
  my ($name, %args) = @_;
  my %macros = query_build_macros(%args);
  return exists $macros{$name} ? 1 : 0;
}

# Return non-zero if ACE/ace/config.h defines the requested macro.
sub ace_config_has_define {
  my ($name, %args) = @_;
  my $ace_root = $args{ace_root};
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

# Resolve a test binary path, including the Windows .exe suffix.
sub resolve_test_binary {
  my ($path, %args) = @_;
  my $is_windows = $args{is_windows};
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
  my ($cmd_ref, $stdout_log, $timeout, %args) = @_;
  my $script_dir = $args{script_dir};

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
  my ($cmd_ref, $stdout_log, $timeout, %args) = @_;
  my $is_windows = $args{is_windows};
  my $script_dir = $args{script_dir};

  if ($is_windows) {
    return run_command_with_timeout_windows(
      $cmd_ref,
      $stdout_log,
      $timeout,
      script_dir => $script_dir,
    );
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

# Return non-zero if IPv6 loopback is available on this host.
sub ipv6_loopback_available {
  my (%args) = @_;
  my $is_windows = $args{is_windows};

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

1;
