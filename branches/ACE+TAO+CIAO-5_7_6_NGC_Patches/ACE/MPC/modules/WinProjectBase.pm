package WinProjectBase;

# ************************************************************
# Description   : A Windows base module for Project Creators
# Author        : Chad Elliott
# Create Date   : 1/4/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Data Section
# ************************************************************

my $max_win_env = 'MPC_MAX_WIN_FILE_LENGTH';

# ************************************************************
# Subroutine Section
# ************************************************************

sub convert_slashes {
  #my $self = shift;
  return 1;
}


sub case_insensitive {
  #my $self = shift;
  return 1;
}


sub translate_directory {
  my($self, $dir) = @_;

  ## Call the base class version
  $dir = $self->DirectoryManager::translate_directory($dir);

  ## Change drive letters and $() macros
  $dir =~ s/^([A-Z]):/$1/i;
  $dir =~ s/\$\(([^\)]+)\)/$1/g;

  ## We need to make sure that we do not exceed the maximum file name
  ## limitation (including the cwd (- c:\) and object file name).  So, we
  ## check the total length against a predetermined "acceptable" value.
  ## This acceptable value is modifiable through the environment.
  my $maxenv = $ENV{$max_win_env};
  my $maxlen = (defined $maxenv && $maxenv =~ /^\d+$/ ? $maxenv : 128) + 3;
  my $dirlen = length($dir);
  my $diff   = (length($self->getcwd()) + $dirlen + 1) - $maxlen;

  if ($diff > 0) {
    if ($diff > $dirlen) {
      $dir = substr($dir, $dirlen - 1);
    }
    else {
      $dir = substr($dir, $diff);
    }
    while($dir =~ s/^\\//) {
    }
  }

  return $dir;
}


sub validated_directory {
  my($self, $dir) = @_;

  ## $(...) could contain a drive letter and Windows can not
  ## make a directory that resembles a drive letter.  So, we have
  ## to exclude those directories with $(...).
  if ($dir =~ /\$\([^\)]+\)/ || $dir =~ /\.\.\\/ || $dir =~ /^[A-Z]:/i) {
    return '.';
  }
  else {
    return $dir;
  }
}


sub crlf {
  return $_[0]->windows_crlf();
}


sub get_cmdsep_symbol {
  #my $self = shift;
  return '&';
}


sub file_sorter {
  #my $self  = shift;
  #my $left  = shift;
  #my $right = shift;
  return lc($_[1]) cmp lc($_[2]);
}


sub get_properties {
  my $self = shift;

  ## Get the base class properties and add the properties that we
  ## support.
  my $props = $self->ProjectCreator::get_properties();

  ## All projects that use this base class are for Windows.
  $$props{'windows'} = 1;

  return $props;
}


1;
