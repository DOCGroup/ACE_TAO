package PerlACE::TestTarget_VxWorks;

# ******************************************************************
# Description : Creates a PerlACE::ProcessVX
# Author      : Chad Elliott
# Create Date : 6/20/2008
#          $Id$
# ******************************************************************

# ******************************************************************
# Pragma Section
# ******************************************************************

use strict;

use PerlACE::TestTarget;
use PerlACE::ProcessVX;

our @ISA = qw(PerlACE::TestTarget);

# ******************************************************************
# Subroutine Section
# ******************************************************************

sub LocalFile {
  my($self, $file) = @_;
  print STDERR "LocalFile for $file is $file\n";
  return $file;
}

sub CreateProcess {
  my $self = shift;
  return new PerlACE::ProcessVX(@_);
}

1;
