package BorlandProjectCreator;

# ************************************************************
# Description   : A Borland Project Creator
# Author        : Chad Elliott
# Create Date   : 3/14/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use ProjectCreator;
use File::Basename;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub dollar_special {
  #my($self) = shift;
  return 1;
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq 'cppdir') {
    my(%dirnames) = ();
    foreach my $file ($self->get_component_list('source_files', 1)) {
      my($dirname) = dirname($file);
      if ($dirname eq '') {
        $dirname = '.';
      }
      elsif ($self->convert_slashes()) {
        $dirname = $self->slash_to_backslash($dirname);
      }
      $dirnames{$dirname} = 1;
    }
    $value = join(';', keys %dirnames);
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return $self->get_modified_project_file_name($name, '.bor');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'borexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'bordll';
}


sub get_template {
  #my($self) = shift;
  return 'bor';
}


1;
