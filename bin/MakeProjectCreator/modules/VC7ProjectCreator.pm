package VC7ProjectCreator;

# ************************************************************
# Description   : A VC7 Project Creator
# Author        : Chad Elliott
# Create Date   : 4/23/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GUID;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my($sname) = '_Static';

# ************************************************************
# Subroutine Section
# ************************************************************

sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;

  if ($key eq 'after' && $val ne '') {
    my($arr) = $self->create_array($val);
    $val = '';

    ## Only write dependencies for non-static projects
    ## and static exe projects
    if ($self->get_writing_type() == 0 || $self->exe_target()) {
      foreach my $entry (@$arr) {
        $val .= '"' . $entry . '" ';
      }
      $val =~ s/\s+$//;
    }
  }
  return $val;
}


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub get_vcversion {
  #my($self) = shift;
  return '7.00';
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq 'guid') {
    my($guid) = new GUID();
    $value = $guid->generate($self->project_file_name(),
                             $self->get_current_input(),
                             $self->getcwd());
  }
  elsif ($name eq 'vcversion') {
    $value = $self->get_vcversion();
  }
  return $value;
}


sub separate_static_project {
  #my($self) = shift;
  return 1;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name.vcproj";
}


sub static_project_file_name {
  my($self) = shift;
  return $self->project_name() . "$sname.vcproj";
}


sub get_env_accessor {
  #my($self) = shift;
  return ('%', '%');
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'vc7exe';
}


sub get_lib_exe_template_input_file {
  #my($self) = shift;
  return 'vc7libexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'vc7dll';
}


sub get_lib_template_input_file {
  #my($self) = shift;
  return 'vc7lib';
}


sub get_template {
  #my($self) = shift;
  return 'vc7';
}


1;
