package CCProjectCreator;

# ************************************************************
# Description   : A Code Composer Project Creator
# Author        : Chad Elliott
# Create Date   : 9/18/2006
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use WinProjectBase;

use vars qw(@ISA);
@ISA = qw(WinProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub compare_output {
  #my $self = shift;
  return 1;
}


sub override_valid_component_extensions {
  my $self  = shift;
  my $comp  = shift;
  my @array = @_;

  if ($comp eq 'source_files' && $self->languageIs(Creator::cplusplus)) {
    push(@array, "\\.cdb");
  }

  return \@array;
}


sub project_file_extension {
  #my $self = shift;
  return '.pjt';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'ccexe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'cclib';
}


1;
