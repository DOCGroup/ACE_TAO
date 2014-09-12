#! /usr/bin/perl
package PerlACE::TestTarget_Android;

# ******************************************************************
# Description : Creates a PerlACE::Android
# Author      : Marcel Smit
# Create Date : 29/20/2008
#          $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;

use PerlACE::TestTarget;
use PerlACE::ProcessVX;
use File::Copy;
use File::Glob;
use File::Spec;
use File::Basename;
use Cwd;
use English;

use POSIX "sys_wait_h";
require PerlACE::ProcessAndroid;

our @ISA = qw(PerlACE::TestTarget);

$PerlACE::TestTarget_Android::EMULATOR_RUNNING = 0;
$PerlACE::TestTarget_Android::LIBS = ();

sub new
{
    my $proto = shift;
    my $config_name = shift;
    my $component = shift;

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "New Android target: $config_name : $component\n";
    }

    my $class = ref ($proto) || $proto;
    my $self = {};
    bless ($self, $class);

    $self->GetConfigSettings($config_name);
    $self->{FSROOT} = $ENV{'ANDROID_FS_ROOT'};
    $self->{PROCESS} = undef;
    $self->{RUNNING} = 0;

    # Start the target.
    if ($PerlACE::TestTarget_Android::EMULATOR_RUNNING == 0) {
        $self->start_target ();
    }

    return $self;
}

sub DESTROY
{
    my $self = shift;
    if ($self->{RUNNING} == 1) {
        # kill the emulator. No need to shutdown gracefully.
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Killing the Android emulator\n";
        }
        $self->KillAll ('emulator*');
        $self->KillAll ('adb');
        $PerlACE::TestTarget_Android::EMULATOR_RUNNING = 0;
        $PerlACE::TestTarget_Android::LIBS = ();
    }
}

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub LocalFile
{
    my $self = shift;
    my $file = shift;

    my $newfile = $self->{FSROOT} . "/" . $file;
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Android LocalFile for $file is $newfile\n";
    }
    return $newfile;
}

sub AddLibPath ($)
{
    my $self = shift;
    my $dir = shift;
    my $noarch = shift;

    # If we have -Config ARCH, use the -ExeSubDir setting as a sub-directory
    # of the lib path.  This is in addition to the regular LibPath.
    if (!$noarch && defined $self->{ARCH}) {
        $self->AddLibPath($dir, 1);
        $dir .= '/' . $self->{EXE_SUBDIR};
    }

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Adding libpath $dir\n";
    }
    $self->{LIBPATH} = PerlACE::concat_path ($self->{LIBPATH}, $dir);
}

sub CreateProcess
{
    my $self = shift;
    my $process = new PerlACE::ProcessAndroid ($self, @_);
    return $process;
}

sub NeedReboot ($)
{
    my $self = shift;
    $self->{REBOOT_NEEDED} = 1;
}

# Reboot target
sub RebootNow ($)
{
    my $self = shift;
    $self->{REBOOT_NEEDED} = undef;
    print STDERR "Attempting to reboot target...\n";
    $self->reboot ();
}

sub start_target ()
{
    # For now, we're assuming one target (avd) is running in the test environment.
    # Need to change this when more than one avd's need to start
    my $self = shift;
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = "2> /dev/null"
    }

    if (! defined ($ENV{'ANDROID_SDK_ROOT'})) {
        print STDERR "Error: Android SDK root not defined.\n";
        return 0;
    }
    if (! defined ($ENV{'ANDROID_AVD_NAME'})) {
        print STDERR "Error: Android AVD name not defined.\n";
        return 0;
    }
    my $avd_name = $ENV{'ANDROID_AVD_NAME'};
    my $android_process = $ENV{'ANDROID_SDK_ROOT'} . "/tools/android";
    my $avd_process = $ENV{'ANDROID_SDK_ROOT'} . "/tools/emulator";
    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";
    my $user_data_image = $ENV{'ANDROID_SDK_HOME'} . "/.android/avd/" . $avd_name . ".avd/userdata-qemu.img";

    my $avd_options = "-noaudio -no-window -wipe-data";

    if (defined ($ENV{'ANDROID_AVD_OPTIONS'})) {
        print STDERR "Resetting AVD options\n";
        $avd_options = $ENV{'ANDROID_AVD_OPTIONS'};
    }

    $self->KillAll ("emulator*");

    FORK: {
        if ($self->{PROCESS} = fork) {
            #parent here
            bless $self;
        }
        elsif (defined $self->{PROCESS}) {
           #child here
           my $user_image_cmd = "rm -f " . $user_data_image;
           if (defined $ENV{'ACE_TEST_VERBOSE'}) {
               print STDERR "Removing user data image: $user_image_cmd\n";
           }

           system ( $user_image_cmd );
           if ($? != 0) {
               print STDERR "failed to execute: $!\n";
           }

           my $avd_cmd = "$avd_process" .' -avd ' .  "$avd_name $avd_options";
           if (defined $ENV{'ACE_TEST_VERBOSE'}) {
               print STDERR "Starting emulator cmd: $avd_cmd\n";
           }

           system ( $avd_cmd );
           if ($? != 0) {
               print STDERR "failed to execute: $!\n";
           }
           exit;
       }
       elsif ($! =~ /No more process/) {
            #EAGAIN, supposedly recoverable fork error
            sleep 5;
            redo FORK;
       }
       else {
           # weird fork error
           print STDERR "ERROR: Can't fork <" . $avd_process . ">: $!\n";
       }
    }

    eval {
        my $timeout = $self->AdbWaitForDeviceTimeout ();

        local $SIG{ALRM} = sub { die "alarm\n" }; # NB: \n required
        alarm $timeout;
        # start the waiting...
        my $wait_cmd = $adb_process . ' wait-for-device';
        system ( $wait_cmd );
        # reset alarm
        alarm 0;
    };

    if ($@) {
        # timed out
        exit unless $@ eq "alarm\n";   # propagate unexpected errors
    }
    else {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Emulator is running <$self->{PROCESS}> -> start the tests.\n";
        }
    }


    # AVD is up and running and ready to spawn executables.
    # First some preparation.
    my $cmd = $adb_process . ' shell "mkdir ' . $self->{FSROOT} . '/tmp "' . $silent;
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Start to execute : $cmd\n";
    }
    system ( $cmd );

    $self->{RUNNING} = 1;
    $PerlACE::TestTarget_Android::EMULATOR_RUNNING = 1;
    return 0;
}

sub WaitForFileTimed ($)
{
    my $self = shift;
    my $file = shift;
    my $timeout = shift;
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = ' > /dev/null 2>&1';
    }

    if ($PerlACE::Process::WAIT_DELAY_FACTOR > 0) {
        $timeout *= $PerlACE::Process::WAIT_DELAY_FACTOR;
    }

    my $newfile = $self->LocalFile ($file);
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print STDERR "Android waits $timeout seconds for $newfile\n";
    }

    # Since the file is available on the target (which we cannot reach),
    # we will try to pull the file from the target to a local directory.
    # If succeed, the the file is there an we can continue.
    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";
    my $fsroot_target = $self->{FSROOT};

    my $cmd_copy_ior = $adb_process . ' pull ' . $newfile . ' ' .
                          File::Spec->tmpdir() . '/' .
                          basename ($newfile) . $silent;

    while ($timeout-- != 0) {
        # copy the ior back to the host sytem
        if (system ( $cmd_copy_ior ) == 0) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Pull $newfile succeeded\n";
            }
            return 0;
        }
        sleep (1);
    }
    return -1;
}

sub DeleteFile ($)
{
    my $self = shift;
    my $file = shift;
    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = ' > /dev/null 2>&1';
    }

    my $targetfile = $self->LocalFile ($file);
    my $cmd = "$adb_process" . ' shell rm '. "$targetfile" . $silent;

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "DeleteFile cmd: $cmd\n";
    }

    system ( $cmd );
    if ($? != 0) {
        return -1;
    }
    return 0;
}

sub Kill ()
{
    my $self = shift;

    if ($self->{RUNNING} == 1) {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Killing emulator process <$self->{PROCESS}>\n";
        }

        my $cnt = kill (1, $self->{PROCESS});

        waitpid ($self->{PROCESS}, WNOHANG);
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Killed $cnt process(es)\n";
        }
        # $self->check_return_value ($?);
    }

    $self->{RUNNING} = 0;
}

sub KillAll ($)
{
    my $self = shift;
    my $procmask = shift;
    if ($OSNAME eq 'MSWin32') {
        if (defined $ENV{'ACE_TEST_VERBOSE'}) {
            print STDERR "Killall not implemented for Windows\n";
            return;
        }
    }
    else {
        my $cmd_killall = "killall -q -r $procmask";
        system ( $cmd_killall );
    }
}

sub PutFile ($)
{
    my $self = shift;
    my $src = shift;
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = "2> /dev/null"
    }

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

    my $targetfile = $self->LocalFile ($src);
    my $cmd = "$adb_process" . ' push '. "\"$src\" \"$targetfile\" $silent";

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "PutFile cmd: $cmd\n";
    }

    system ( $cmd );
    if ($? != 0) {
        return -1;
    }
    return 0;
}

sub GetFile ($)
{
    my $self = shift;
    my $remote_file = shift;
    my $local_file = shift;
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = "2> /dev/null"
    }

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

    if (!defined $local_file) {
        $local_file = $remote_file;
    }
    $remote_file = $self->LocalFile($remote_file);
    my $cmd = "$adb_process" . ' pull '. "$remote_file $local_file $silent";

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "GetFile cmd: $cmd\n";
    }

    system ( $cmd );
    if ($? != 0) {
        return -1;
    }
    return 0;
}

sub PutLib ($)
{
    my $self = shift;
    my $newlib = shift;
    my $tgtlib = shift;
    my $silent;

    if (!defined $ENV{'ACE_TEST_VERBOSE'}) {
      $silent = "2> /dev/null"
    }

    foreach my $lib (@{$PerlACE::TestTarget_Android::LIBS}) {
        if ($lib eq $newlib) {
            if (defined $ENV{'ACE_TEST_VERBOSE'}) {
                print STDERR "Duplicate lib $newlib\n";
            }
            return 0;
        }
    }

    my $adb_process = $ENV{'ANDROID_SDK_ROOT'} . "/platform-tools/adb";

    my $cmd = "$adb_process" . ' push '. "\"$newlib\" \"$tgtlib\" $silent";

    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print STDERR "PutLib cmd: $cmd\n";
    }

    system ( $cmd );
    if ($? != 0) {
        return -1;
    }

    # keep tabs on copied libs
    push(@{$PerlACE::TestTarget_Android::LIBS}, $newlib);

    return 0;
}

1;

