# $Id$
#
# The TestTarget class is for operations that are per-target while testing.
# They can be overridden for specific needs like embedded systems, etc.

package PerlACE::TestTarget;

use strict;
use English;
use POSIX qw(:time_h);

###############################################################################

# Create the proper kind of TestTarget based on arguments or test
# configuration. Pass the PerlACE::ConfigList as the first argument.

sub create_target
{
    my $config = shift;
    my $target = undef;
    if ($config->check_config("LabVIEW_RT")) {
        require PerlACE::TestTarget_LVRT;
        $target = new PerlACE::TestTarget_LVRT;
    }
    else {
        $target = new PerlACE::TestTarget;
    }
    return $target;
}

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    bless ($self, $class);
    return $self;
}

sub DESTROY
{
    my $self = shift;
}

##################################################################

sub LocalFile ($)
{
    my $self = shift;
    my $file = shift;
    my $newfile = PerlACE::LocalFile($file);
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
    return $process;
}

# Don't need to do anything in most cases.
sub GetStderrLog ($)
{
    my $self = shift;
    return;
}

1;
