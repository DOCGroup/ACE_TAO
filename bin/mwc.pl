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
use File::Basename;

my($basePath) = getExecutePath($0) . '/MakeProjectCreator';
unshift(@INC, $basePath . '/modules');

my($mpcpath) = (defined $ENV{MPC_CORE} ? "$ENV{MPC_CORE}" :
                                         dirname(dirname($basePath)) . '/MPC');
unshift(@INC, $mpcpath . '/modules');

if (! -x "$mpcpath/modules") {
  print STDERR "ERROR: Unable to access the MPC module path.\n" .
               "       Perhaps you need to check out the MPC module?\n";
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

sub which {
  my($prog)   = shift;
  my($exec)   = $prog;
  my($part)   = '';
  my($envSep) = ($^O eq 'MSWin32' ? ';' : ':');

  if (defined $ENV{'PATH'}) {
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
    if ($prog =~ /^[\/\\]/ ||
        $prog =~ /^[A-Za-z]:[\/\\]?/) {
      $loc = dirname($prog);
    }
    else {
      $loc = getcwd() . '/' . dirname($prog);
    }
  }
  else {
    $loc = dirname(which($prog));
  }

  if ($loc eq '.') {
    $loc = getcwd();
  }

  if ($loc ne '') {
    $loc .= '/';
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
