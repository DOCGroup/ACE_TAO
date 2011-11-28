package FileLocatorFactory;

# ************************************************************
# Description   : Create FileLocator objects.
# Author        : Chad Elliott
# Create Date   : 11/29/2005
# $Id$
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use Cwd;
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
    ((defined $ENV{SVN_ASP_DOT_NET_HACK} && -d '_svn') || searchParentDirectory('.svn'))
      && do { return new SVNFileLocator(); };
    -d 'CVS'  && do { return new CVSFileLocator(); };
    print STDERR "WARNING: Unsupported revision control protocol\n";
  }

  return new FileLocator();
}

sub searchParentDirectory {
  my($hidden)  = shift;
  my($path)    = cwd();
  my($index)   = -1;

  # Search all parent directories for the specified hidden
  # directory. We stop when we either found the hidden directory
  # of there are no more parent directories let to search.
  do {
    if (-d $path . '/' . $hidden) {
      return 1;
    }

    $index = rindex($path, '/');

    if ($index != -1) {
      $path = substr ($path, 0, $index);
    }
  } while ($index != -1);

  return 0;
}


1;
