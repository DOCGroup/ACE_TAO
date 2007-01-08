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

require 5.006;

use strict;
use Config;
use FindBin;
use File::Spec;
use File::Basename;

my($basePath) = $FindBin::Bin;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
  $basePath = VMS::Filespec::unixify($basePath);
}
$basePath .= '/MakeProjectCreator';
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

# ************************************************************
# Main Section
# ************************************************************

## Allocate a driver
my($driver) = new MWC();

## Add our creators to the front of the list
my($creators) = $driver->getCreatorList();
unshift(@$creators, @creators);

## Add the mpc path to the include paths, but preserve
## the original @ARGV as it is included in the output of
## most of the workspace creators.
my(@args) = ('-include', "$mpcpath/config",
             '-include', "$mpcpath/templates", @ARGV);

## Execute the driver
exit($driver->execute($basePath, basename($0), \@args));
