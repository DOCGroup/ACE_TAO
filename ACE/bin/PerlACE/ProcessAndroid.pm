#! /usr/bin/perl
# $Id$

package PerlACE::ProcessAndroid;

use POSIX "sys_wait_h";
use File::Basename;
use File::Spec;
use FileHandle;
use Cwd;

use strict;

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    $self->{RUNNING} = 0;
    $self->{IGNOREEXESUBDIR} = 1;
    $self->{IGNOREHOSTROOT} = 0;
    $self->{PROCESS} = undef;
    $self->{EXECUTABLE} = shift;

    # Only set argument when they are really supplied via the
    # CreateProcess call. If the weren't supplied, an error like
    # Process_Android::HASH (0x...) is generated.
    if (@_ == 2) {
        $self->{ARGUMENTS} = shift;
    }
    else {
        $self->{ARGUMENTS} = "";
    }
    $self->{FSROOT} = $ENV{'ANDROID_FS_ROOT'};

    bless ($self, $class);

    # copy the test executable to the target at forehand
    $self->copy_executable ();

    return $self;
}

sub Arguments
{
    my $self = shift;

    if (@_ != 0) {
        $self->{ARGUMENTS} = shift;
    }

    return $self->{ARGUMENTS};
}

sub Executable
{
    my $self = shift;

    if (@_ != 0) {
        $self->{EXECUTABLE} = shift;
    }

    my $executable = $self->{EXECUTABLE};

    # If the target's config has a different ACE_ROOT, rebase the executable
    # from $ACE_ROOT to the target's root.
    if (defined $self->{TARGET} &&
          $self->{TARGET}->ACE_ROOT() ne $ENV{'ACE_ROOT'}) {
        $executable = PerlACE::rebase_path ($executable,
                                            $ENV{'ACE_ROOT'},
                                            $self->{TARGET}->ACE_ROOT());
    }

    if ($self->{IGNOREHOSTROOT} == 0) {
        if (PerlACE::is_vxworks_test()) {
            $executable = PerlACE::VX_HostFile ($executable);
        }
    }

    if ($self->{IGNOREEXESUBDIR}) {
        return $executable;
    }

    my $basename = basename ($executable);
    my $dirname = dirname ($executable).'/';

    my $subdir = $PerlACE::Process::ExeSubDir;
    if (defined $self->{TARGET} && defined $self->{TARGET}->{EXE_SUBDIR}) {
        $subdir = $self->{TARGET}->{EXE_SUBDIR};
    }

    $executable = $dirname . $subdir . $basename;

    return $executable;
}

sub Wait ($)
{
    my $self = shift;
    my $timeout = shift;
    if (!defined $timeout || $timeout < 0) {
        waitpid ($self->{PROCESS}, 0);
    } else {
        return TimedWait($self, $timeout);
    }
}

sub TimedWait ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
        $timeout *= $PerlACE::Process::WAIT_DELAY_FACTOR;
    }

    if (defined $ENV{'ACE_TEST_VERBOSE'} && $self->{PROCESS} > 0) {
        print STDERR "Wait $timeout to finish executable $self->{PROCESS}. ";
        print STDERR "RUNNING: $self->{RUNNING}\n";
    }
    while ($timeout-- != 0 && $self->{PROCESS} > 0 && $self->{RUNNING} == 1) {
        my $pid = waitpid ($self->{PROCESS}, WNOHANG);
        if ($pid == $self->{PROCESS}) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Wait for process <$self->{PROCESS}> ended. $timeout seconds left.\n";
            }
            return 0;
        }
        sleep 1;
    }

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Exit TimedWait with Process: $self->{PROCESS}, Running: $self->{RUNNING}\n";
    }
    if ($self->{PROCESS} == 0 || $self->{RUNNING} == 0) {
        return 0;
    }
    return -1;
}

sub Kill ()
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Killing process <$self->{PROCESS}>\n";
        }
        # killing the adb process, not the actual test executable.
        kill (1, $self->{PROCESS});

        my $pid = waitpid ($self->{PROCESS}, WNOHANG);
        if ($pid == -1) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Process <$self->{PROCESS}> already ended\n";
            }
        }
        elsif ($pid == $self->{PROCESS}) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Process <$self->{PROCESS}> ended\n";
            }
        }
    }

    $self->{RUNNING} = 0;
}

sub WaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    my $pid = waitpid ($self->{PROCESS}, WNOHANG);
    if ($pid == $self->{PROCESS}) {
        $self->{RUNNING} = 0;
    }

    if ($self->{RUNNING} == 0) {
        return 0;
    }

    my $status = $self->TimedWait ($timeout);

    if ($status == -1) {
        print STDERR "ERROR: $self->{EXECUTABLE} timedout\n";
        $self->Kill ();
    }

    $self->{RUNNING} = 0;

    return $status;
}


# Do a Spawn and immediately WaitKill
#
sub SpawnWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->Spawn () == -1) {
        return -1;
    }
    my $result = 0;

    if ($self->{RUNNING} == 1) {
        $result = $self->WaitKill ($timeout);
    }

    $self->remove_executable ();

    return $result;
}

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";
        kill ('TERM', $self->{PROCESS});
    }

    return $self->WaitKill ($timeout);
}

sub IgnoreExeSubDir
{
    my $self = shift;

    if (@_ != 0) {
        $self->{IGNOREEXESUBDIR} = shift;
    }

    return $self->{IGNOREEXESUBDIR};
}

sub IgnoreHostRoot
{
    my $self = shift;

    if (@_ != 0) {
        $self->{IGNOREHOSTROOT} = shift;
    }

    return $self->{IGNOREHOSTROOT};
}

sub remove_executable ()
{
  my $self = shift;
  my $silent;

  if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
    $silent = "2> /dev/null"
  }

  my $fsroot_target = $ENV{'ANDROID_FS_ROOT'};
  my $program = $self->Executable ();
  my $test = basename ($program);
  my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

  my $cmd = $adb_process . ' shell rm ' . $fsroot_target . "/" . $test . " " . $silent;

  if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "Start to execute : $cmd\n";
  }
  system ( $cmd );
}

sub Spawn ()
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                     "> already running\n";
        return -1;
    }

    if (!defined $self->{EXECUTABLE}) {
        print STDERR "ERROR: Cannot Spawn: No executable specified\n";
        return -1;
    }

    if ($self->{IGNOREEXESUBDIR} == 0) {
        if (!-f $self->Executable ()) {
            print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                         "> not found\n";
            return -1;
        }
    }
    my $status = 0;

    my $fsroot_target = $ENV{'ANDROID_FS_ROOT'};
    my $exe = $self->Executable ();
    my $program = "$fsroot_target/$exe";

    my($test, $dir, $suffix) = fileparse($program);

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";
    my $cmd = $adb_process . ' shell "cd ' . $dir . ' && LD_LIBRARY_PATH=' . $fsroot_target . '/lib:$LD_LIBRARY_PATH ./' . $test . ' ' . $self->{ARGUMENTS} . '"';

    FORK: {
        if ($self->{PROCESS} = fork) {
            bless $self;
        }
        elsif (defined $self->{PROCESS}) {
            $self->{RUNNING} = 1;

            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Start to execute: $cmd\n";
            }
            exec ( $cmd );
            exit;
        }
        elsif ($! =~ /No more process/) {
            sleep 5;
            redo FORK;
        }
        else {
            print STDERR "ERROR: Can't fork <" . $cmd . ">: $!\n";
       }
    }

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Forked process started with ID <$self->{PROCESS}>\n";
    }
    $self->{RUNNING} = 1;

    return 0;
}

sub copy_executable ()
{
    my $self = shift;

    my $fsroot_target = $ENV{'ANDROID_FS_ROOT'};
    my $program = $self->Executable ();
    my $exe = "$fsroot_target/$program";

    $self->PutFile ("$program", $exe);

    if ($PerlACE::Static == 0) {
        my $vxtest_file = "$ENV{'ACE_ROOT'}/$program" . '.vxtest';
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Analyzing vxtest file <$vxtest_file>\n";
        }
        copy_vxtest_files ($self, $vxtest_file);
    }
}

sub copy_vxtest_files ()
{
    my $self = shift;
    my $vxtestfile = shift;
    my $fh = new FileHandle;

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Analyzing vxtest file <$vxtestfile>\n";
    }

    if (open ($fh, $vxtestfile)) {
        my $line1 = <$fh>;
        chomp $line1;
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Analyzing vxtest file: Found line $line1\n";
        }
        while(<$fh>) {
            $line1 = $_;
            chomp $line1;
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Analyzing vxtest file: Found line $line1\n";
            }

            $self->copy_library ($line1);
        }
    }
    close $fh;
}

sub copy_library ()
{
    my $self = shift;
    my $name = shift;

    my $cdir = getcwd ();

    chdir ("$ENV{'ACE_ROOT'}/lib");

    my $query = "lib$name.so";
    if (-e $query) {
        # MSM: Using exclude wildcards, like !(b*)
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Found $name in library directory\n";
        }
        my @files = glob ($query . '*');
        foreach my $file (@files) {
            $self->PutFile ($file, "$self->{FSROOT}/lib/$file");
        }
    }
    else {
        chdir ("$ENV{'ACE_ROOT'}/tests");

        if (-e $query) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Found $name in tests directory\n";
            }

            my @files = glob ($query . '*');
            foreach my $file (@files) {
                $self->PutFile ($file, "$self->{FSROOT}/lib/$file");
            }
        }
    }
    chdir ("$cdir");
}


sub PutFile ($)
{
    my $self = shift;
    my $src = shift;
    my $dest = shift;

    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = "2> /dev/null"
    }

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

    my $cmd = "$adb_process" . ' push '. "$src $dest $silent";

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "PutFile cmd: $cmd\n";
    }

    system ( $cmd );
    if ($? != 0) {
        return -1;
    }
    return 0;
}

1;
