package BMakeProjectCreator;

# ************************************************************
# Description   : A BMake Project Creator
# Author        : Chad Elliott
# Create Date   : 2/03/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub dollar_special {
  #my($self) = shift;
  return 1;
}


sub sort_files {
  #my($self) = shift;
  return 0;
}


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;
  my(%names) = ('cppdir' => 'source_files',
                'rcdir'  => 'resource_files',
               );

  if (defined $names{$name}) {
    my(%dirnames) = ();
    foreach my $file ($self->get_component_list($names{$name}, 1)) {
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

  return $self->get_modified_project_file_name($name, '.bmak');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'bmakedllexe';
}


sub get_lib_exe_template_input_file {
  #my($self) = shift;
  return 'bmakelibexe';
}


sub get_lib_template_input_file {
  #my($self) = shift;
  return 'bmakelib';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'bmakedll';
}


sub get_template {
  #my($self) = shift;
  return 'bmake';
}


1;
