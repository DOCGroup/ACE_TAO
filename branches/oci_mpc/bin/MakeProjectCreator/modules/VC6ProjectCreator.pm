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

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub get_type_append {
  my($self) = shift;
  my($type) = "";
  if ($self->lib_target()) {
    ## Set the type_append preserving whitespace
    if ($self->get_writing_type() == 1) {
      $type = " LIB";
    }
    else {
      $type = " DLL";
    }
  }
  else {
    ## Set the type_append preserving whitespace
    if ($self->get_writing_type() == 1) {
      $type = " Static EXE";
    }
    else {
      $type = " EXE";
    }
  }
  return $type;
}


sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;

  if ($key eq 'depends' && $val ne "") {
    my($arr) = $self->create_array($val);
    my($app) = "DLL";
    $val = "";

    ## Only write dependencies for non-static projects
    ## and static exe projects
    my($wt) = $self->get_writing_type();
    if ($wt == 0 || $self->exe_target()) {
      if ($wt == 1) {
        $app = "LIB";
      }

      foreach my $entry (@$arr) {
        $val .= "\"$entry $app\" ";
      }
      $val =~ s/\s+$//;
    }
  }
  return $val;
}


sub sort_files {
  my($self) = shift;
  return 1;
}


sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;   
  return lc($left) cmp lc($right);
}


sub crlf {
  my($self) = shift;
  return "\r\n";
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq "make_file_name") {
    $value = $self->transform_file_name($self->project_name() . ".mak");
  }

  return $value;
}


sub separate_static_project {   
  my($self) = shift;
  return 1;
}


sub project_file_name {
  my($self) = shift;
  return $self->transform_file_name($self->project_name() . ".dsp");
}


sub static_project_file_name {
  my($self) = shift;
  return $self->transform_file_name($self->project_name() . "_Static.dsp");
}


sub override_valid_component_extensions {
  my($self)  = shift;
  my($comp)  = shift;
  my($array) = undef;

  if ($comp eq 'source_files') {
    my(@exts) = ("\\.cpp", "\\.cxx", "\\.c");
    $array = \@exts;
  }

  return $array;
}
 
 
sub override_exclude_component_extensions {
  my($self)  = shift;
  my($comp)  = shift;
  my($array) = undef;

  if ($comp eq 'source_files') {
    my(@exts) = ("_T\\.cpp", "_T\\.cxx");
    $array = \@exts;
  }

  return $array;
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "vc6dspdllexe";
}


sub get_lib_exe_template_input_file {
  my($self) = shift;
  return "vc6dsplibexe";
}


sub get_lib_template_input_file {
  my($self) = shift;
  return "vc6dsplib";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "vc6dspdll";
}


sub get_template {
  my($self) = shift;
  return "vc6dsp";
}


1;
