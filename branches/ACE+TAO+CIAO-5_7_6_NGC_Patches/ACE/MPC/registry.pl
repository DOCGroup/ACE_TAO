#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#  Author: Chad Elliott
#    Date: 12/05/2005
#     $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use FindBin;
use FileHandle;
use File::Basename;

# ******************************************************************
# Data Section
# ******************************************************************

my $Registry;
my $MPC_ROOT = $FindBin::Bin;
$MPC_ROOT =~ s!/!\\!g;

my $version = '1.4';
my %types   = ('nmake' => ['NMAKE', 'NMAKE'],
               'bmake' => ['Borland Make', 'Borland Make'],
               'vc6'   => ['DSW', 'DSP'],
               'vc71'  => ['SLN 7.1', 'VCPROJ 7.1'],
               'vc8'   => ['SLN 8.0', 'VCPROJ 8.0'],
               'vc9'   => ['SLN 9.0', 'VCPROJ 9.0'],
               'vc10'  => ['SLN 10.0', 'VCPROJ 10.0'],
               'wix'   => ['WiX', 'WiX Project'],
              );

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub set_ext_icon {
  my($ext, $num) = @_;
  my $extf = $ext . 'file';
  $Registry->{"HKEY_CLASSES_ROOT/.$ext/"} = {'/' => $extf};
  $Registry->{"HKEY_CLASSES_ROOT/$extf/"} = {};
  $Registry->{"HKEY_CLASSES_ROOT/$extf/DefaultIcon/"} =
                           {'/' => "$MPC_ROOT\\MPC.ico,$num"};
}


sub set_dir_command {
  my($type, $desc) = @_;
  my $shell = 'HKEY_CLASSES_ROOT/Directory/shell';
  my $hash = $Registry->{$shell};

  ## If there is no shell setting, just create an empty one.  However,
  ## this isn't very likely.
  if (!defined $hash) {
    $Registry->{$shell} = {};
    $hash = $Registry->{$shell};
  }

  ## Create an entry for this project type (vc6, vc7, etc.)
  my $key = 'MPC' . uc($type) . '/';
  $hash->{$key} = {'/' => "MPC -> $desc"};

  ## Now store the command for creating a workspace for this project
  ## type.
  $key .= 'command/';
  $hash->{$key} = {'/' => "cmd /c \"cd %L && $MPC_ROOT\\mwc.pl -type $type -recurse || pause\""};
}


sub set_mwc_command {
  my($type, $desc) = @_;
  my $shell = 'HKEY_CLASSES_ROOT/mwcfile/shell';
  my $hash = $Registry->{$shell};

  ## Create the new entry for the mwc files.  This is likely to not
  ## exist.
  if (!defined $hash) {
    $Registry->{$shell} = {};
    $hash = $Registry->{$shell};
  }

  ## Create an entry for this project type (vc6, vc7, etc.)
  my $key = 'MPC' . uc($type) . '/';
  $hash->{$key} = {'/' => "MPC -> $desc"};

  ## Now store the command for creating a workspace for this project
  ## type.
  $key .= 'command/';
  $hash->{$key} = {'/' => "cmd /c \"$MPC_ROOT\\mwc.pl -type $type %L || pause\""};

  ## Since MPC will create a workspace out of a directory, we want to do
  ## the same thing for directories too.
  set_dir_command($type, $desc);
}


sub set_mpc_command {
  my($type, $desc) = @_;
  my $shell = 'HKEY_CLASSES_ROOT/mpcfile/shell';
  my $hash = $Registry->{$shell};

  ## Create the new entry for the mpc files.  This is likely to not
  ## exist.
  if (!defined $hash) {
    $Registry->{$shell} = {};
    $hash = $Registry->{$shell};
  }

  ## Create an entry for this project type (vc6, vc7, etc.)
  my $key = 'MPC' . uc($type) . '/';
  $hash->{$key} = {'/' => "MPC -> $desc"};

  ## Now store the command for creating a single project for this project
  ## type.
  $key .= 'command/';
  $hash->{$key} = {'/' => "cmd /c \"$MPC_ROOT\\mpc.pl -type $type %L || pause\""};
}


sub delete_key {
  my $key = shift;
  my $val = $Registry->{$key};

  ## Delete everything associated with this key (recursively traversing
  ## each key).
  if (UNIVERSAL::isa($val, 'HASH')) {
    foreach my $k (keys %$val) {
      delete_key($key . $k);
    }
  }

  ## Now get the key itself.
  delete $Registry->{$key};
}

# ******************************************************************
# Main Section
# ******************************************************************

if ($^O eq 'MSWin32') {
  ## Pull in the registry modules and import the necessary items.
  require Win32::TieRegistry;
  Win32::TieRegistry->import(TiedRef => \$Registry,
                             Delimiter => '/');
}
else {
  ## Currently, no other registry type is supported.
  print "ERROR: This script will only run on Windows.\n";
  exit(1);
}

if (defined $ARGV[0]) {
  if ($ARGV[0] eq '-r') {
    ## Get rid of the MPC_ROOT environment variable.
    delete $Registry->{'HKEY_CURRENT_USER/Environment/MPC_ROOT'};

    ## Now delete all the keys that this script knows how to make.
    delete_key('HKEY_CLASSES_ROOT/.mwc/');
    delete_key('HKEY_CLASSES_ROOT/mwcfile/');
    delete_key('HKEY_CLASSES_ROOT/.mpc/');
    delete_key('HKEY_CLASSES_ROOT/mpcfile/');
    delete_key('HKEY_CLASSES_ROOT/.mpb/');
    delete_key('HKEY_CLASSES_ROOT/mpbfile/');

    foreach my $type (keys %types) {
      delete_key('HKEY_CLASSES_ROOT/Directory/shell/MPC' . uc($type) . '/');
    }

    exit(0);
  }
  else {
    print STDERR "registry v$version\n",
                 "Usage: ", basename($0), " [-r]\n\n",
                 "       -r  Remove MPC related registry keys.\n\n",
                 "Set the MPC_ROOT environment variable to the location of this script.\n",
                 "Also, add context menus for .mwc files and directories.\n";
    exit(0);
  }
}

## Set the MPC_ROOT environment variable.
$Registry->{'HKEY_CURRENT_USER/Environment/MPC_ROOT'} = [$MPC_ROOT, 'REG_SZ'];

## Associate the icons with the various MPC file types.
set_ext_icon('mwc', 0);
set_ext_icon('mpc', 1);
set_ext_icon('mpb', 1);

## Create the command settings for each type
foreach my $type (keys %types) {
  set_mwc_command($type, $types{$type}->[0]);
  set_mpc_command($type, $types{$type}->[1]);
}

print "You may need to log out and then ",
      "log back in for some of these settings to take effect.\n";

exit(0);
