eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 6/17/2002
#         $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use Cwd;
use Config;
use File::Basename;

if ( $^O eq 'VMS' ) {
  require VMS::Filespec;
  import VMS::Filespec qw(unixpath);
}

my($basePath) = getExecutePath($0) . '/MakeProjectCreator';
unshift(@INC, $basePath . '/modules');

my($mpcroot) = $ENV{MPC_ROOT};
my($mpcpath) = (defined $mpcroot ? $mpcroot :
                                   dirname(dirname($basePath)) . '/MPC');
unshift(@INC, $mpcpath . '/modules');

if (defined $mpcroot) {
  print STDERR "MPC_ROOT was set to $mpcroot.\n";
}

if (! -d "$mpcpath/modules") {
  print STDERR "ERROR: Unable to find the MPC modules in $mpcpath.\n";
  if (defined $mpcroot) {
    print STDERR "Your MPC_ROOT environment variable does not point to a ",
                 "valid MPC location.\n";
  }
  else {
    print STDERR "You can set the MPC_ROOT environment variable to the ",
                 "location of MPC.\n";
  }
  exit(255);
}

require MWC;

# ************************************************************
# Data Section
# ************************************************************

my(@creators) = ('GNUACEWorkspaceCreator',
                 'BorlandWorkspaceCreator',
                );

# ************************************************************
# Subroutine Section
# ************************************************************

sub getBasePath {
  return $mpcpath;
}


sub which {
  my($prog) = shift;
  my($exec) = $prog;

  if (defined $ENV{'PATH'}) {
    my($part)   = '';
    my($envSep) = $Config{'path_sep'};
    foreach $part (split(/$envSep/, $ENV{'PATH'})) {
      $part .= "/$prog";
      if ( -x $part ) {
        $exec = $part;
        last;
      }
    }
  }

  return $exec;
}


sub getExecutePath {
  my($prog) = shift;
  my($loc)  = '';

  if ($prog ne basename($prog)) {
    my($dir) = ($^O eq 'VMS' ? unixpath(dirname($prog)) : dirname($prog));
    if ($prog =~ /^[\/\\]/ ||
        $prog =~ /^[A-Za-z]:[\/\\]?/) {
      $loc = $dir;
    }
    else {
      $loc = ($^O eq 'VMS' ? unixpath(getcwd()) : getcwd()) . '/' . $dir;
    }
  }
  else {
    $loc = dirname(which($prog));
    if ($^O eq 'VMS') {
      $loc = unixpath($loc);
    }
  }

  $loc =~ s/\/\.$//;

  if ($loc eq '.') {
    $loc = ($^O eq 'VMS' ? unixpath(getcwd()) : getcwd());
  }

  return $loc;
}


# ************************************************************
# Main Section
# ************************************************************

## Allocate a driver
my($driver) = new MWC();

## Add our creators to the front of the list
my($creators) = $driver->getCreatorList();
unshift(@$creators, @creators);

## Add the mpc path to the include paths
unshift(@ARGV, '-include', "$mpcpath/config",
               '-include', "$mpcpath/templates");

## Execute the driver
exit($driver->execute($basePath, basename($0), \@ARGV));
