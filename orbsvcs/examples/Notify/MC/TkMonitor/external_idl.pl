eval '(exit $?0)' && eval 'exec perl -w -S $0 ${1+"$@"}'
    & eval 'exec perl -w -S $0 $argv:q'
    if 0;

# ******************************************************************
#      Author: Chad Elliott
#        Date: 7/17/2007
# Description: A script to generate the Notify Service Monitor modules
#         $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;
use File::Path;

# ******************************************************************
# Main Section
# ******************************************************************

if (!defined $ENV{OPALORB_ROOT}) {
  print STDERR "Please set the OPALORB_ROOT environment variable ",
               "to the full path of opalORB\n";
  exit(1);
}

my($rm)   = undef;
my($root) = undef;
for(my $i = 0; $i <= $#ARGV; $i++) {
  if ($ARGV[$i] eq '-r') {
    $rm = $i + 1;
    last;
  }
  else {
    $root = $ARGV[$i];
  }
}

if ($rm) {
  for(my $i = $rm; $i <= $#ARGV; $i++) {
    rmtree($ARGV[$i]);
  }
  exit(0);
}

if (!defined $root) {
  if (defined $ENV{NOTIFY_ROOT}) {
    $root = $ENV{NOTIFY_ROOT};
  }
  elsif (defined $ENV{TAO_ROOT}) {
    $root = "$ENV{TAO_ROOT}/orbsvcs/orbsvcs";
  }
}

if (!defined $root) {
  print STDERR "Please provide the full path ",
               "to the Notify Service IDL files\n";
  exit(1);
}

foreach my $i ('Notify/MonitorControl/NotificationServiceMC.idl',
               'Notify/MonitorControlExt/NotifyMonitoringExt.idl') {
  system("$ENV{OPALORB_ROOT}/idl/idl.pl --include $root --include $root/.. --client $root/$i");
}
