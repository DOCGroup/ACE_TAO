#!/usr/bin/perl -w

# ************************************************************
# Description   : Front end for the Notify Service Monitor
# Author        : Chad Elliott
# Create Date   : 7/17/2007
#            $Id$
# ************************************************************

# ************************************************************
# Subroutine Section
# ************************************************************

sub BEGIN {
  if (!defined $ENV{OPALORB_ROOT}) {
    print STDERR "You must set the OPALORB_ROOT environment variable ",
                 "to the full path of opalORB.\n";
    exit(1);
  }
}

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FindBin;
use File::Spec;
use File::Basename;

use Tk;

use lib "$ENV{OPALORB_ROOT}";
use lib "$ENV{OPALORB_ROOT}/Naming";
use CORBA;
use CosNotification::NotificationServiceMonitorControl;

my($basePath) = $FindBin::Bin;
if ($^O eq 'VMS') {
  $basePath = File::Spec->rel2abs(dirname($0)) if ($basePath eq '');
  $basePath = VMS::Filespec::unixify($basePath);
}
unshift(@INC, $basePath . '/modules');

require MonitorControl;

# ************************************************************
# Main Section
# ************************************************************

try {
  my($orb) = CORBA::ORB_init(\@ARGV);

  my($loc) = 'corbaname:rir:#TAO_MonitorAndControl';
  $loc = $ARGV[0] if (defined $ARGV[0]);

  my($monitor) = new MonitorControl($orb, $loc);
  $monitor->Show();
}
catch CORBA::SystemException with {
  my($ex) = shift;
  print STDERR "$ex\n";
}
catch Error::Simple with {
  my($ex) = shift;
  print STDERR "Exception: $ex\n";
};
