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

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub sort_files {
  #my($self) = shift;
  return 0;
}


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name.mak";
}


sub get_env_accessor {
  #my($self) = shift;
  return ('%', '%');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'nmakeexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'nmakedll';
}


sub get_template {
  #my($self) = shift;
  return 'nmake';
}


1;
