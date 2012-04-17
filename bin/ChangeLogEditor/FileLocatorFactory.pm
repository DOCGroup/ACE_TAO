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
  ## Check for Subversion first.  It is unlikely that the .svn directory
  ## will exist when Subversion isn't the rcs being used.  However, that
  ## is not the case for CVS directories.
  switch: {
    ((defined $ENV{SVN_ASP_DOT_NET_HACK} && -d '_svn') || -d '.svn')
      && do { return new SVNFileLocator(); };
    -d 'CVS'  && do { return new CVSFileLocator(); };
    print STDERR "WARNING: Unsupported revision control protocol\n";
  }

  return new FileLocator();
}


1;
