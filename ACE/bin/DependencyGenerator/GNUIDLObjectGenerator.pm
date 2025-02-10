package GNUIDLObjectGenerator;

# ************************************************************
# Description   : Generates object files for GNU IDL Makefiles.
# Author        : Chip Jones
# Create Date   : 11/01/2011
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Spec;
use ObjectGenerator;

use vars qw(@ISA);
@ISA = qw(ObjectGenerator);

# ************************************************************
# Constants
# ************************************************************
my $prefix = '$(IDL_GEN_FILES_DIR)/';
my @suffixes = qw/
    $(IDL_CLIENT_HDR_EXT)
    $(IDL_CLIENT_INL_EXT)
    $(IDL_CLIENT_SRC_EXT)
    $(IDL_SERVER_HDR_EXT)
    $(IDL_SERVER_SRC_EXT)
    $(IDL_SERVER_THDR_EXT)
    $(IDL_SERVER_TINL_EXT)
    $(IDL_SERVER_TSRC_EXT)
    /;

# ************************************************************
# Subroutine Section
# ************************************************************

sub process {
  my($noext) = $_[1];
  $noext =~ s/\.[^\.]+$//o;
  $noext =~ s/.+\/// if $noext =~ /\.\.\//;
  my($vol, $dir, $file) = File::Spec->splitpath($noext);
  my @list = map "$prefix$file$_", @suffixes;
  return \@list;
}

1;
