package DirectoryManager;

# ************************************************************
# Description   : This module provides directory related methods
# Author        : Chad Elliott
# Create Date   : 5/13/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Spec;
use File::Basename;

# ************************************************************
# Data Section
# ************************************************************

my $onVMS = ($^O eq 'VMS');
my $case_insensitive = File::Spec->case_tolerant();
my $cwd = Cwd::getcwd();
if ($^O eq 'cygwin' && $cwd !~ /[A-Za-z]:/) {
  my $cyg = `cygpath -w $cwd`;
  if (defined $cyg) {
    $cyg =~ s/\\/\//g;
    chop($cwd = $cyg);
  }
  $case_insensitive = 1;
}
elsif ($onVMS) {
  $cwd = VMS::Filespec::unixify($cwd);
  $cwd =~ s!/$!!g;
}
my $start = $cwd;

# ************************************************************
# Subroutine Section
# ************************************************************

sub cd {
  my($self, $dir) = @_;
  my $status = chdir($dir);

  if ($status && $dir ne '.') {
    ## First strip out any /./ or ./ or /.
    $dir =~ s/\/\.\//\//g;
    $dir =~ s/^\.\///;
    $dir =~ s/\/\.$//;

    ## If the new directory contains a relative directory
    ## then we just get the real working directory
    if (index($dir, '..') >= 0) {
      $cwd = Cwd::getcwd();
      if ($^O eq 'cygwin' && $cwd !~ /[A-Za-z]:/) {
        ## We're using Cygwin perl, use cygpath to get the windows path
        ## and then fix up the slashes.
        my $cyg = `cygpath -w $cwd`;
        if (defined $cyg) {
          $cyg =~ s/\\/\//g;
          chop($cwd = $cyg);
        }
      }
      elsif ($onVMS) {
        ## On VMS, we nee to get the UNIX style path and remove the
        ## trailing slash.
        $cwd = VMS::Filespec::unixify($cwd);
        $cwd =~ s!/$!!g;
      }
    }
    else {
      if ($dir =~ /^(\/|[a-z]:)/i) {
        ## It was a full path, just store it.
        $cwd = $dir;
      }
      else {
        ## This portion was relative, add it onto the current working
        ## directory.
        $cwd .= "/$dir";
      }
    }
  }
  return $status;
}


sub getcwd {
  #my $self = shift;
  return $cwd;
}


sub getstartdir {
  #my $self = shift;
  return $start;
}


sub mpc_basename {
  #my $self = $_[0];
  #my $file = $_[1];
  return substr($_[1], rindex($_[1], '/') + 1);
}


sub mpc_dirname {
  my($self, $dir) = @_;

  ## The dirname() on VMS doesn't work as we expect it to.
  if ($onVMS) {
    ## If the directory contains multiple parts, we need to get the
    ## dirname in a UNIX style format and then remove the slash from the
    ## end.
    if (index($dir, '/') >= 0) {
      $dir = VMS::Filespec::unixify(dirname($dir));
      $dir =~ s!/$!!g;
      return $dir;
    }
    else {
      ## There's no directory portion, so just return '.'
      return '.';
    }
  }
  else {
    return dirname($dir);
  }
}


sub mpc_glob {
  my($self, $pattern) = @_;

  ## glob() provided by OpenVMS does not understand [] within
  ## the pattern.  So, we implement our own through recursive calls
  ## to mpc_glob().
  if ($onVMS && $pattern =~ /(.*)\[([^\]]+)\](.*)/) {
    my @files;
    my($pre, $mid, $post) = ($1, $2, $3);
    for(my $i = 0; $i < length($mid); $i++) {
      foreach my $new ($self->mpc_glob($pre . substr($mid, $i, 1) . $post)) {
        push(@files, $new) if (!StringProcessor::fgrep($new, \@files));
      }
    }
    return @files;
  }

  ## Otherwise, we just return the globbed pattern.
  return glob($pattern);
}


sub onVMS {
  return $onVMS;
}


sub path_is_relative {
  ## To determine if the path is relative, we just determine if it is not
  ## an absolute path.
  #my($self, $path) = @_;
  return (index($_[1], '/') != 0 && $_[1] !~ /^[A-Z]:\//i);
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub translate_directory {
  my($self, $dir) = @_;

  ## Remove the current working directory from $dir (if it is contained)
  my $cwd = $self->getcwd();
  $cwd =~ s/\//\\/g if ($self->convert_slashes());
  if (index($dir, $cwd) == 0) {
    my $cwdl = length($cwd);
    return '.' if (length($dir) == $cwdl);
    $dir = substr($dir, $cwdl + 1);
  }

  ## Translate .. to $dd
  if (index($dir, '..') >= 0) {
    my $dd = 'dotdot';
    $dir =~ s/^\.\.([\/\\])/$dd$1/;
    $dir =~ s/([\/\\])\.\.$/$1$dd/;
    $dir =~ s/([\/\\])\.\.([\/\\])/$1$dd$2/g;
    $dir =~ s/^\.\.$/$dd/;
  }

  return $dir;
}


sub convert_slashes {
  #my $self = shift;
  return 0;
}


sub case_insensitive {
  #my $self = shift;
  return $case_insensitive;
}

1;
