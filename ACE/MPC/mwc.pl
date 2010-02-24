#! /usr/bin/perl
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
use FindBin;
use File::Spec;
use File::Basename;

my $basePath = $FindBin::RealBin;
my $baseName = $FindBin::RealScript;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
  $basePath = VMS::Filespec::unixify($basePath);
}
unshift(@INC, $basePath . '/modules');

require Driver;

# ************************************************************
# Subroutine Section
# ************************************************************

sub getBasePath {
  return $basePath;
}

# ************************************************************
# Main Section
# ************************************************************

my $driver = new Driver($basePath, Driver::workspaces());
exit($driver->run(@ARGV));
