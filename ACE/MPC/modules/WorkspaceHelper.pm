package WorkspaceHelper;

# ************************************************************
# Description   : Base class and factory for all workspace helpers
# Author        : Chad Elliott
# Create Date   : 9/01/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Data Section
# ************************************************************

my %required;

# ************************************************************
# Subroutine Section
# ************************************************************

sub get {
  my $type = shift;

  ## Create the helper name
  $type =~ s/Creator/Helper/;
  $type =~ s/=HASH.*//;

  ## If we can find a helper with this name, we will
  ## create a singleton of that type and return it.
  if (!$required{$type}) {
    foreach my $inc (@INC) {
      if (-r "$inc/$type.pm") {
        require "$type.pm";
        $required{$type} = $type->new();
        last;
      }
    }

    ## If we can't find the helper, we just create an
    ## empty helper and return that.
    $required{$type} = new WorkspaceHelper() if (!$required{$type});
  }

  return $required{$type};
}


sub new {
  my $class = shift;
  return bless {}, $class;
}


sub modify_value {
  my($self, $name, $value) = @_;
  return $value;
}


sub modify_libpath {
  #my $self    = shift;
  #my $str     = shift;
  #my $reldir  = shift;
  #my $libname = shift;
  return undef;
}


sub write_settings {
  #my $self   = shift;
  #my $fh     = shift;
  #my @locals = @_;
  return 1, undef;
}


sub perform_custom_processing {
  #my $self     = shift;
  #my $fh       = shift;
  #my $creator  = shift;
  #my $toplevel = shift;
}

1;
