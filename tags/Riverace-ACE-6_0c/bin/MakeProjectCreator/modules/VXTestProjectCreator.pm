package VXTestProjectCreator;

# ************************************************************
# Description   : A Project Creator for VxWorks testing
# Author        : Johnny Willemsen
# Create Date   : 3/27/2008
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

sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  my($value) = $self->get_assignment('exename');
  if (!defined $value) {
    $value = $self->get_assignment('staticname');
  }
  if (!defined $value) {
    $value = $self->get_assignment('sharedname');
  }
  return $self->get_modified_project_file_name($value, '.vxtest');
}

sub get_template {
  #my($self) = shift;
  return 'vxtest';
}

sub need_to_write_project {
  my $self = shift;

  ## We only want to return 1 if the project has source files and isn't a
  ## custom-only project.  We have to check for custom_only because
  ## non-custom related project settings are only removed after calling
  ## need_to_write_project().
  if (!$self->get_assignment('custom_only') &&
      $self->SUPER::need_to_write_project() == 1) {
    ## We only want to return 1 if this is an executable or library project
    return 1 if ($self->exe_target() || $self->lib_target());
  }

  return 0;
}

sub warn_useless_project {
  return 0;
}

1;
