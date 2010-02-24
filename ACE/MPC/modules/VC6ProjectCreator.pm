package VC6ProjectCreator;

# ************************************************************
# Description   : A VC6 Project Creator
# Author        : Chad Elliott
# Create Date   : 3/14/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use VCProjectBase;

use vars qw(@ISA);
@ISA = qw(VCProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub project_file_extension {
  #my $self = shift;
  return '.dsp';
}


sub override_valid_component_extensions {
  my($self, $comp) = @_;

  ## Visual C++ 6.0 doesn't understand all of the extensions that MPC
  ## supports.
  if ($comp eq 'source_files' && $self->languageIs(Creator::cplusplus)) {
    return ["\\.cpp", "\\.cxx", "\\.c"];
  }

  return undef;
}


sub override_exclude_component_extensions {
  my($self, $comp) = @_;

  ## Visual C++ 6.0 doesn't understand all of the extensions that MPC
  ## supports.
  if ($comp eq 'source_files' && $self->languageIs(Creator::cplusplus)) {
    return ["_T\\.cpp", "_T\\.cxx"];
  }

  return undef;
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'vc6dspdllexe';
}


sub get_lib_exe_template_input_file {
  #my $self = shift;
  return 'vc6dsplibexe';
}


sub get_lib_template_input_file {
  #my $self = shift;
  return 'vc6dsplib';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'vc6dspdll';
}


1;
