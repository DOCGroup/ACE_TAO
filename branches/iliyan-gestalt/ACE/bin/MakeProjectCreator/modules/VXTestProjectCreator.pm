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

  return $self->get_modified_project_file_name($name, '.vxtest');
}

sub get_template {
  #my($self) = shift;
  return 'vxtest';
}

1;
