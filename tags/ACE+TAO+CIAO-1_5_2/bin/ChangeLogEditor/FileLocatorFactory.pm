package FileLocatorFactory;

# ************************************************************
# Description   : Create FileLocator objects.
# Author        : Chad Elliott
# Create Date   : 11/29/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use CVSFileLocator;
use SVNFileLocator;

# ************************************************************
# Subroutine Section
# ************************************************************

sub create {
  switch: {
    -d 'CVS'  && do { return new CVSFileLocator(); };
    ((defined $ENV{SVN_ASP_DOT_NET_HACK} && -d '_svn') || -d '.svn')
      && do { return new SVNFileLocator(); };
    print STDERR "WARNING: Unsupported revision control protocol\n";
  }

  return new FileLocator();
}


1;
