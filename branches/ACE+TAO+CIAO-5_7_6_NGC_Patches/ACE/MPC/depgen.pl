#! /usr/bin/perl
eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ************************************************************
# Description   : Generate dependencies for Make and NMake.
# Author        : Chad Elliott
# Create Date   : 5/06/2002
#            $Id$
# ************************************************************

# ************************************************************
# Pragma Section
# ************************************************************

use strict;
use FindBin;
use File::Spec;
use File::Basename;

my $basePath = $FindBin::Bin;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
  $basePath = VMS::Filespec::unixify($basePath);
}
unshift(@INC, $basePath . '/modules/Depgen', $basePath . '/modules');

require Driver;

# ************************************************************
# Main Section
# ************************************************************

my $driver = new Driver();
exit($driver->run(\@ARGV));
