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

require Driver;

# ************************************************************
# Data Section
# ************************************************************

my(@creators) = ('GNUACEWorkspaceCreator',
                 'NMakeWorkspaceCreator',
                 'VC6WorkspaceCreator',
                 'VC7WorkspaceCreator',
                 'VC71WorkspaceCreator',
                 'BorlandWorkspaceCreator',
                 'GHSWorkspaceCreator',
                 'EM3WorkspaceCreator',
                 'VA4WorkspaceCreator',
                 'MakeWorkspaceCreator',
                 'AutomakeWorkspaceCreator',
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
# Subroutine Section
# ************************************************************

my($driver) = new Driver($basePath, basename($0), @creators);
exit($driver->run(@ARGV));
