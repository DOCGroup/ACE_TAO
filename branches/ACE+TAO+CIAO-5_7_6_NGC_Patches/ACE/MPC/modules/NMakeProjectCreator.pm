package NMakeProjectCreator;

# ************************************************************
# Description   : An NMake Project Creator
# Author        : Chad Elliott
# Create Date   : 5/31/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use WinProjectBase;
use MakeProjectBase;

use vars qw(@ISA);
@ISA = qw(MakeProjectBase WinProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub project_file_extension {
  #my $self = shift;
  return '.mak';
}


sub get_dll_exe_template_input_file {
  #my $self = shift;
  return 'nmakeexe';
}


sub get_dll_template_input_file {
  #my $self = shift;
  return 'nmakedll';
}


sub get_properties {
  my $self = shift;

  ## Create the map of properties that we support.  This is a reproduced
  ## property from the VCProjectBase and out of laziness I have not made
  ## a base project for just this property.
  my $props = {'microsoft' => 1};

  ## Merge in properties from all base projects
  foreach my $base (@ISA) {
    my $func = $base . '::get_properties';
    my $p = $self->$func();
    foreach my $key (keys %$p) {
      $$props{$key} = $$p{$key};
    }
  }

  return $props;
}


1;
