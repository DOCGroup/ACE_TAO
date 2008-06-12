# $Id$
#
# The TestTarget class is for operations that are per-target while testing.
# They can be overridden for specific needs like embedded systems, etc.

package PerlACE::TestTarget;

use strict;
use English;
use POSIX qw(:time_h);
use File::Copy;

###############################################################################

# Create the proper kind of TestTarget based on specified test component.
# Pass the component name as the first argument. If there's no
# DOC_TEST_<component> environment variable, use the local machine.

sub create_target
{
    my $component = shift;
    my $target = undef;
    my $envname = "DOC_TEST_\U$component";
    if (!exists $ENV{$envname}) {
        print "$component has no config name\n";
        $target = new PerlACE::TestTarget("default");
        return $target;
    }
    my $config_name = $ENV{$envname};
    # There's a configuration name; use it to look up the platform.
    $config_name = uc $config_name;
    $envname = $config_name.'_OS';
    if (!exists $ENV{$envname}) {
        print STDERR "$config_name requires an OS type in $envname\n";
        return undef;
    }
    my $config_os = $ENV{$envname};
    SWITCH: {
      if ($config_os =~ m/local/i) {
        $target = new PerlACE::TestTarget ($config_name);
        last SWITCH;
      }
      if ($config_os =~ m/LabVIEW_RT/i) {
        require PerlACE::TestTarget_LVRT;
        $target = new PerlACE::TestTarget_LVRT ($config_name);
        last SWITCH;
      }
      print STDERR "$config_os is an unknown OS type!\n";
    }
    return $target;
}

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    my $config_name = shift;
    bless ($self, $class);
    $self->GetConfigSettings($config_name);

    return $self;
}

sub DESTROY
{
    my $self = shift;
}

# If there was a config name specified, use it to look up the configure
# info. Else, use the traditional defaults.
sub GetConfigSettings ($)
{
    my $self = shift;
    my $config_name = shift;
    my $env_prefix = '';
    if (defined $config_name) {
        $env_prefix = $config_name."_";
    }
    my $env_name = $env_prefix.'ACE_ROOT';
    if (exists $ENV{$env_name}) {
        $self->{ace_root} = $ENV{$env_name};
    }
    else {
        # Fall back to naked ACE_ROOT if no config-specific one.
        $self->{ace_root} = $ENV{'ACE_ROOT'};
    }
    $env_name = $env_prefix.'TAO_ROOT';
    if (exists $ENV{$env_name}) {
        $self->{tao_root} = $ENV{$env_name};
    } else {
        $self->{tao_root} = "$self->{ace_root}/TAO";
    }
    $env_name = $env_prefix.'CIAO_ROOT';
    if (exists $ENV{$env_name}) {
        $self->{ciao_root} = $ENV{$env_name};
    } else {
        $self->{ciao_root} = "$self->{tao_root}/CIAO";
    }
    $env_name = $env_prefix.'EXE_SUBDIR';
    if (exists $ENV{$env_name}) {
        $self->{EXE_SUBDIR} = $ENV{$env_name}.'/';
    } else {
        # If no ExeSubDir given via env variable, and this is an unnamed
        # config, allow use of the subdir specified on the command line.
        # This preserves historical behavior.
        if (defined $config_name) {
            $self->{EXE_SUBDIR} = './';
        }
        else {
            $self->{EXE_SUBDIR} = $PerlACE::Process::ExeSubDir;
        }
    }
    $self->{PROCESS_START_WAIT_INTERVAL} = 15;
    $self->{PROCESS_STOP_WAIT_INTERVAL} = 10;
}

##################################################################

sub ACE_ROOT ($)
{
    my $self = shift;
    return $self->{ace_root};
}

sub TAO_ROOT ($)
{
    my $self = shift;
    return $self->{tao_root};
}

sub CIAO_ROOT ($)
{
    my $self = shift;
    return $self->{ciao_root};
}

sub ExeSubDir ($)
{
    my $self = shift;
    my $new_val = shift;
    if (defined $new_val) {
        $self->{EXE_SUBDIR} = $new_val;
    }
    return $self->{EXE_SUBDIR};
}

sub ProcessStartWaitInterval ($)
{
    my $self = shift;
    my $new_val = shift;
    if (defined $new_val) {
        $self->{PROCESS_START_WAIT_INTERVAL} = $new_val;
    }
    return $self->{PROCESS_START_WAIT_INTERVAL};
}

sub ProcessStopWaitInterval ($)
{
    my $self = shift;
    my $new_val = shift;
    if (defined $new_val) {
        $self->{PROCESS_STOP_WAIT_INTERVAL} = $new_val;
    }
    return $self->{PROCESS_STOP_WAIT_INTERVAL};
}

sub LocalFile ($)
{
    my $self = shift;
    my $file = shift;
    my $newfile = PerlACE::LocalFile($file);
    print STDERR "LocalFile for $file is $newfile\n";
    return $newfile;
}

sub DeleteFile ($)
{
    my $self = shift;
    my $file = shift;
    unlink $file;
}

sub GetFile ($)
{
    # On local host, the file is already there.
    my $self = shift;
    my $remote_file = shift;
    my $local_file = shift;
    return 0;
}

# Put file from a to b
sub PutFile ($)
{
    my $self = shift;
    my $src = shift;
    my $dest = shift;
    if ($src != $dest) {
        copy ($src, $dest);
    }
}

sub WaitForFileTimed ($)
{
    my $self = shift;
    my $file = shift;
    my $timeout = shift;
    return PerlACE::waitforfile_timed ($file, $timeout);
}

sub CreateProcess ($)
{
    my $self = shift;
    my $process = new PerlACE::Process (@_);
    $process->Target($self);
    return $process;
}

# Don't need to do anything in most cases.
sub GetStderrLog ($)
{
    my $self = shift;
    return;
}

1;
