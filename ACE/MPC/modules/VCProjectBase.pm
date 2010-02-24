package VCProjectBase;

# ************************************************************
# Description   : A VC Project base module
# Author        : Chad Elliott
# Create Date   : 1/4/2005
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use WinProjectBase;

use vars qw(@ISA);
@ISA = qw(WinProjectBase);

# ************************************************************
# Subroutine Section
# ************************************************************

sub compare_output {
  #my $self = shift;
  return 1;
}


sub require_dependencies {
  my $self = shift;

  ## Only write dependencies for non-static projects
  ## and static exe projects, unless the user wants the
  ## dependency combined static library.
  return ($self->get_static() == 0 || $self->exe_target() ||
          $self->dependency_combined_static_library());
}


sub dependency_is_filename {
  #my $self = shift;
  return 0;
}


sub get_properties {
  my $self = shift;

  ## Get the base class properties and add the properties that we
  ## support.
  my $props = $self->WinProjectBase::get_properties();

  ## All projects that use this base class are for Microsoft compilers.
  $$props{'microsoft'} = 1;

  return $props;
}


1;
