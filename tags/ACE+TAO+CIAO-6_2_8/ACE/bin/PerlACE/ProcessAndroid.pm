#! /usr/bin/perl
# $Id$

package PerlACE::ProcessAndroid;

use POSIX "sys_wait_h";
use File::Basename;
use File::Spec;
use FileHandle;
use Cwd;
use Config;

use strict;

###############################################################################

###  Grab signal names

my @signame;

if (defined $Config{sig_name}) {
    my $i = 0;
    foreach my $name (split (' ', $Config{sig_name})) {
        $signame[$i] = $name;
        $i++;
    }
}
else {
    my $i;
    for ($i = 0; $i < 255; ++$i) {
        $signame[$i] = $i;
    }
}

###############################################################################

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    $self->{TARGET} = shift;
    $self->{RUNNING} = 0;
    $self->{IGNOREEXESUBDIR} = 1;
    $self->{IGNOREHOSTROOT} = 0;
    $self->{PROCESS} = undef;
    $self->{EXECUTABLE} = shift;

    # Only set argument when they are really supplied via the
    # CreateProcess call. If the weren't supplied, an error like
    # Process_Android::HASH (0x...) is generated.
    if (@_ == 1) {
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

sub DESTROY
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        print STDERR "ERROR: <", $self->{EXECUTABLE},
                     "> still running upon object destruction\n";
        $self->Kill ();
    }

    if (defined $self->{SCRIPTFILE}) {
        unlink $self->{SCRIPTFILE};
    }
    if (defined $self->{UNLINKLIST}) {
        foreach my $ul (@{$self->{UNLINKLIST}}) {
            unlink $ul;
        }
    }
}

###############################################################################

sub Arguments
{
    my $self = shift;

    if (@_ != 0) {
        $self->{ARGUMENTS} = shift;
        if (defined $self->{SCRIPTFILE}) {
          if (!defined $self->{UNLINKLIST}) {
              $self->{UNLINKLIST} = ();
          }
          push(@{$self->{UNLINKLIST}}, $self->{SCRIPTFILE});
          $self->{SCRIPTFILE} = undef;
        }
    }

    return $self->{ARGUMENTS};
}

sub Executable
{
    my $self = shift;

    if (@_ != 0) {
        $self->{EXECUTABLE} = shift;
        if (defined $self->{SCRIPTFILE}) {
            if (!defined $self->{UNLINKLIST}) {
                $self->{UNLINKLIST} = ();
            }
            push(@{$self->{UNLINKLIST}}, $self->{SCRIPTFILE});
            $self->{SCRIPTFILE} = undef;
        }
        # copy the (new) test executable to the target
        # previously scanned .vxtest files and detected libraries
        # will not be scanned/copied twice
        $self->copy_executable ();
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
        return $self->TimedWait($timeout);
    }
}

# really only for internal use
sub check_return_value ($)
{
    my $self = shift;
    my $rc = shift;

    if ($rc < 0x80) {
        return $rc;
    }

    # Remember Core dump flag
    my $dump = 0;

    if ($rc & 0x80) {
        $rc &= ~0x80;
        $dump = 1;
    }

    # check for ABRT, KILL or TERM
    if ($rc == 6 || $rc == 9 || $rc == 15) {
        return 0;
    }

    print STDERR "ERROR: <", $self->{EXECUTABLE},
                 "> exited with ";

    print STDERR "coredump from " if ($dump == 1);

    print STDERR "signal $rc : ", $signame[$rc], "\n";

    return 255;
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

    # Multiply with 10 because we wait a tenth of a second each time
    $timeout *= 10;

    while ($timeout-- != 0 && $self->{PROCESS} > 0 && $self->{RUNNING} == 1) {
        my $pid = waitpid ($self->{PROCESS}, WNOHANG);
        if ($pid != 0 && $? != -1) {
            if (defined $ENV{'ACE_TEST_VERBOSE'} && $self->{PROCESS} > 0) {
                print STDERR "Executable $self->{PROCESS} finished ($pid)\n";
            }
            last;
        }
        select(undef, undef, undef, 0.1);
    }

    # attempt to retrieve exitstatus from remote .rc file
    my $shell = $ENV{'ANDROID_SDK_ROOT'} . '/platform-tools/adb shell';
    my $rcfile = $self->{RCFILE};
    ## wait max 5 * $PerlACE::Process::WAIT_DELAY_FACTOR sec for RC file to appear
    my $start_tm = time ();
    my $max_wait = 5;
    if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
        $max_wait *= $PerlACE::Process::WAIT_DELAY_FACTOR;
    }
    my $rc = -255;
    while ((time() - $start_tm) < $max_wait) {
        select(undef, undef, undef, 0.2);
        $rc = int(`$shell 'if [ -e $rcfile -a -s $rcfile ] ; then cat $rcfile; rm -f $rcfile >/dev/null 2>&1; else echo -255; fi'`);
        if ($rc != -255) {
            return $self->check_return_value ($rc);
        }
    }

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Exit TimedWait with Process: $self->{PROCESS}, Running: $self->{RUNNING}\n";
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

    return $result;
}

sub TerminateWaitKill ($)
{
    my $self = shift;
    my $timeout = shift;

    if ($self->{RUNNING}) {
        print STDERR "INFO: $self->{EXECUTABLE} being killed.\n";

        my $killcmd = $ENV{'ANDROID_SDK_ROOT'} . '/platform-tools/adb shell "kill -s TERM ' . $self->{REMOTE_PID} . '"';
        system ($killcmd);
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
    my $program = "$fsroot_target/".basename($exe);

    my($test, $dir, $suffix) = fileparse($program);

    my $local_xdir = cwd ();

    if (!defined $self->{PIDFILE}) {
        $self->{PIDFILE} = "$fsroot_target/ace-".rand(time).".pid";
    }
    if (!defined $self->{RCFILE}) {
        $self->{RCFILE} = "$fsroot_target/ace-".rand(time).".rc";
    }
    if (!defined $self->{SCRIPTFILE}) {
        $self->{SCRIPTFILE} = "$local_xdir/run-".rand(time).".sh";
    }
    ## create scriptfile
    my $run_script =
        # "if [ ! -e /tmp/.acerun ]; then mkdir /tmp/.acerun; fi\n".
        "cd $fsroot_target\n".
        "export LD_LIBRARY_PATH=$fsroot_target/lib:.:\$LD_LIBRARY_PATH\n".
        "export PATH=\$PATH:$fsroot_target/lib:.\n".
        "export ACE_ROOT=$fsroot_target\n";

    if (defined $self->{TARGET} && defined $self->{TARGET}->{EXTRA_ENV}) {
      my $x_env_ref  = $self->{TARGET}->{EXTRA_ENV};
      while ( my ($env_key, $env_value) = each(%$x_env_ref) ) {
          $run_script .=
          "export $env_key=\"$env_value\"\n";
      }
    }
    $run_script .=
      "./$test $self->{ARGUMENTS} &\n";
    $run_script .=
      "MY_PID=\$!\n".
      "echo \$MY_PID > ".$self->{PIDFILE}."\n";
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        $run_script .=
          "echo INFO: Process started remote with pid [\$MY_PID]\n";
    }
    $run_script .=
      "wait \$MY_PID\n";
    $run_script .=
      "MY_RC=\$?\n".
      "echo \$MY_RC > ".$self->{RCFILE}."\n";
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        $run_script .=
          "echo INFO: Process [\$MY_PID] returned exit code [\$MY_RC]\n";
    }
    unless (open (RUN_SCRIPT, ">".$self->{SCRIPTFILE})) {
        print STDERR "ERROR: Cannot Spawn: <", $self->Executable (),
                      "> failed to create ",$self->{SCRIPTFILE},"\n";
        return -1;
    }
    print RUN_SCRIPT $run_script;
    close RUN_SCRIPT;

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "INFO: created run script [",$self->{SCRIPTFILE},"]\n", $run_script;
    }
    $self->PutFile ($self->{SCRIPTFILE}, "$fsroot_target/".basename($self->{SCRIPTFILE}));

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";
    my $cmd = $adb_process . ' shell "cd ' . $fsroot_target . ' && source ./' . basename($self->{SCRIPTFILE}) . '"';

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

    my $shell = $adb_process . ' shell';
    my $pidfile = $self->{PIDFILE};
    ## wait max 10 * $PerlACE::Process::WAIT_DELAY_FACTOR sec for pid file to appear
    my $start_tm = time ();
    my $max_wait = 10;
    if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
        $max_wait *= $PerlACE::Process::WAIT_DELAY_FACTOR;
    }
    my $rc = 1;
    while ((time() - $start_tm) < $max_wait) {
        select(undef, undef, undef, 0.2);
        $rc = int(`$shell 'if [ -e $pidfile -a -s $pidfile ] ; then cat $pidfile; rm -f $pidfile >/dev/null 2>&1; else echo 0; fi'`);
        if ($rc != 0) {
            $self->{REMOTE_PID} = $rc;
            last;
        }
    }
    if (!defined $self->{REMOTE_PID}) {
        print STDERR "ERROR: Remote command failed <" . $test . ' ' . $self->{ARGUMENTS} . ">: $! No PID found.\n";
        return -1;
    }

    $self->{RUNNING} = 1;

    return 0;
}

sub copy_executable ($)
{
    my $self = shift;

    my $fsroot_target = $ENV{'ANDROID_FS_ROOT'};
    my $program = $self->Executable ();
    # never copy program subdirectory if specified (like '../Generic_Servant/server')
    my $exe = "$fsroot_target/".basename($program);

    $self->{SHLIBS} = ();
    $self->{VXTESTS} = ();
    $self->{XLIBPATH} = ();

    push (@{$self->{XLIBPATH}}, '.');
    if (defined $self->{TARGET} && defined $self->{TARGET}->{LIBPATH}) {
        foreach my $libpath (split(/:|;/, $self->{TARGET}->{LIBPATH})) {
          push(@{$self->{XLIBPATH}}, $libpath);
        }
    }

    $self->PutFile ("$program", $exe);

    if ($PerlACE::Static == 0) {
        # collect libraries from .vxtest file
        $self->process_vxtest ($program.'.vxtest');
        # collect libraries from extra lib paths (might be dynamically loaded)
        $self->collect_extra_libs ();
        # collect any runtime lib dependencies specified
        $self->collect_runtime_libs ();
        # copy all collected libraries
        foreach my $lib (@{$self->{SHLIBS}}) {
            $self->PutFile ($lib, "$self->{FSROOT}/lib/".basename($lib));
        }
        # handle defined system libraries
        if (defined $self->{TARGET} && defined $self->{TARGET}->{SYSTEM_LIBS}) {
            $self->copy_system_libs ($self->{TARGET}->{SYSTEM_LIBS});
        }
    }
}

sub process_vxtest ($)
{
    my $self = shift;
    my $newvxtest = shift;

    foreach my $vxtest (@{$self->{VXTESTS}}) {
        if ($vxtest eq $newvxtest) {
            return;
        }
    }
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Processing vxtest file $newvxtest\n";
    }
    $self->collect_vxtest_libs ($newvxtest);
}

sub add_unique_lib ($)
{
    my $self = shift;
    my $newlib = shift;

    foreach my $lib (@{$self->{SHLIBS}}) {
        if ($lib eq $newlib) {
            return 0;
        }
    }
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Collecting library $newlib\n";
    }
    push(@{$self->{SHLIBS}}, $newlib);
    return 1;
}

sub collect_vxtest_libs ()
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

            $self->collect_vxtest_lib ($line1);
        }
    }
    close $fh;
}

sub collect_vxtest_lib ($)
{
    my $self = shift;
    my $name = shift;
    my $query;
    my @libpaths = ("$ENV{'ACE_ROOT'}/lib");

    push (@libpaths, @{$self->{XLIBPATH}});

    foreach my $libpath (@libpaths) {
      $query = "$libpath/lib$name.so";
      if (-e $query) {
          if (defined $ENV{'ACE_TEST_VERBOSE'}) {
              print STDERR "Found $name in library directory $libpath\n";
          }
          # look for versioned and non-versioned filenames
          my @files = glob ($query . '*');
          foreach my $file (@files) {
              $self->add_unique_lib ($file);
          }
      }
    }
}

sub collect_extra_libs ()
{
    my $self = shift;
    # treat current dir as extra libpath
    my @libpaths = (@{$self->{XLIBPATH}});
    my $query;
    my $vxtest;

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Inspecting libpaths (@libpaths) for extra libraries\n";
    }

    foreach my $libpath (@libpaths) {
      # look for versioned and non-versioned libraries
      $query = "$libpath/lib*.so*";
      my @files = glob ($query);
      foreach my $file (@files) {
          if ($self->add_unique_lib ($file) == 1) {
              # check for possible .vxtest for new lib dependency
              $vxtest = basename ($file);
              $vxtest =~ s/^lib(.*)[\.]so([\.].*)?$/\1/;
              if (-e "$libpath/$vxtest.vxtest") {
                  # process .vxtest file if not yet processed before
                  $self->process_vxtest ("$libpath/$vxtest.vxtest");
              }
          }
      }
    }
}

sub collect_runtime_libs ()
{
    my $self = shift;
    # only need to test ACE_ROOT/lib since all libs from '.' and extra libpaths are copied already
    my @libpaths = ("$ENV{'ACE_ROOT'}/lib");
    my $query;

    if (defined $self->{TARGET} && defined $self->{TARGET}->{RUNTIME_LIBDEP}) {

        foreach my $runtimelib (@{$self->{TARGET}->{RUNTIME_LIBDEP}}) {

            if (-e $runtimelib) {
                $self->add_unique_lib ($runtimelib);
            } else {

                if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                    print STDERR "Inspecting libpaths (@libpaths) for runtime libe $runtimelib\n";
                }

                foreach my $libpath (@libpaths) {
                    # look for versioned and non-versioned libraries
                    $query = "$libpath/lib$runtimelib.so*";
                    my @files = glob ($query);
                    foreach my $file (@files) {
                        $self->add_unique_lib ($file);
                    }
                }
            }
        }
    }
}

sub copy_system_libs ()
{
    my $self = shift;
    my $syslibs = shift;
    my @liblist = split (',', $syslibs);
    foreach my $lib (@liblist) {
        if (-e $lib) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Found system library $lib\n";
            }
            $self->PutFile ($lib, "$self->{FSROOT}/lib/" . basename ($lib));
        } else {
            print STDERR "Cannot find system library $lib!\n";
        }
    }
}

sub PutFile ($)
{
    my $self = shift;
    my $src = shift;
    my $dest = shift;

    if (defined $self->{TARGET}) {
        return $self->{TARGET}->PutLib($src, $dest);
    } else {
        my $silent;

        if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
          $silent = "2> /dev/null"
        }

        my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

        my $cmd = "$adb_process" . ' push '. "\"$src\" \"$dest\" $silent";

        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
          print STDERR "PutFile cmd: $cmd\n";
        }

        system ( $cmd );
        if ($? != 0) {
            return -1;
        }
        return 0;
    }
}

1;
