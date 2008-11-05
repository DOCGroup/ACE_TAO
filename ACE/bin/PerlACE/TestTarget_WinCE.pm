#! /usr/bin/perl
package PerlACE::TestTarget_WinCE;

# ******************************************************************
# Description : Creates a PerlACE::WiNCE
# Author      : Johnny Willemsen
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

our @ISA = qw(PerlACE::TestTarget);

sub new
{
    my $proto = shift;
    my $config_name = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};
    bless ($self, $class);
    $self->GetConfigSettings($config_name);
    my $targethost;
    my $env_name = $config_name.'_IPNAME';
    if (exists $ENV{$env_name}) {
        $targethost = $ENV{$env_name};
    }
    else {
        print STDERR "You must define target hostname/IP with $env_name\n";
        undef $self;
        return undef;
    }

    $env_name = $config_name.'_FS_ROOT';
    my $fsroot = '\network\temp\ACE\wince6';
    if (exists $ENV{$env_name}) {
        $fsroot = $ENV{$env_name};
    }
    else {
        print STDERR "Warning: no $env_name variable; falling back ",
                     "to $fsroot\n";
    }
    $self->{FSROOT} = $fsroot;

    $self->{REBOOT_CMD} = $ENV{"ACE_REBOOT_LVRT_CMD"};
    if (!defined $self->{REBOOT_CMD}) {
        $self->{REBOOT_CMD} = 'I_Need_A_Reboot_Command';
    }
    $self->{REBOOT_TIME} = $ENV{"ACE_LVRT_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 200;
    }

    $self->{REBOOT_TIME} = $ENV{"ACE_RUN_LVRT_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 200;
    }
    $self->{REBOOT_NEEDED} = undef;

    my $telnet_port = $ENV{'ACE_RUN_VX_TGT_TELNET_PORT'};
    my $telnet_host = $ENV{'ACE_RUN_VX_TGT_TELNET_HOST'};
    if (!defined $telnet_host)  {
      $telnet_host = $ENV{'ACE_RUN_VX_TGTHOST'};
    }
    if (!defined $telnet_port)  {
      $telnet_port = 23;
    }

    $PerlACE::ProcessVX::ExeExt = '.exe';

    return $self;
}


# ******************************************************************
# Subroutine Section
# ******************************************************************

sub LocalFile {
    my $self = shift;
    my $file = shift;
    my $newfile = $self->{FSROOT} . '\\' . $file;
    print STDERR "WinCE LocalFile for $file is $newfile\n";
    return $newfile;
}

sub CreateProcess {
  my $self = shift;
  my $process = new PerlACE::ProcessVX ($self, @_);
  return $process;
}

# Need a reboot when this target is destroyed.
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
    reboot ();
}

sub WaitForFileTimed ($)
{
    my $self = shift;
    my $file = shift;
    my $timeout = shift;
print STDERR "wait for $file time $timeout\n";
    return PerlACE::waitforfile_timed ("\\\\thor\\temp\\ACE\\wince6\\$file", $timeout);
}

sub ProcessStartWaitInterval ($)
{
    my $self = shift;
    my $new_val = shift;
return 45;
    if (defined $new_val) {
        $self->{PROCESS_START_WAIT_INTERVAL} = $new_val;
    }
    return $self->{PROCESS_START_WAIT_INTERVAL};
}

sub GetFile ($)
{
    my $self = shift;
    my $file = shift;
    my $remote_file = "\\\\thor\\temp\\ACE\\wince6\\$file";
#    if ($remote_file != $file) {
        copy ($remote_file, "c:\\ace\\latest\\ace_wrappers\\tao\\tests\\hello\\server.ior");
print STDERR "copy $remote_file $file\n";
#    }
    return 0;
}

# Put file from a to b
sub PutFile ($)
{
    my $self = shift;
    my $src = shift;
print STDERR "put $src\n";
    return 0;
}


sub DeleteFile ($)
{
    my $self = shift;
    my $file = shift;
    unlink ("\\\\thor\\temp\\ACE\\wince6\\$file");
}

1;
