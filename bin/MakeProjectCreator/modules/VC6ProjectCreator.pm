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
# Data Section
# ************************************************************

my($dynamiclib) = "DLL";
my($staticlib)  = "LIB";
my($dynamicexe) = "EXE";
my($staticexe)  = "Static EXE";
my($sname)      = "_Static";

# ************************************************************
# Subroutine Section
# ************************************************************

sub remove_type_append {
  my($self) = shift;
  my($str)  = shift;

  foreach my $type ($staticexe, $dynamicexe, $staticlib, $dynamiclib) {
    if ($str =~ /(.*)\s+$type$/) {
      $str = $1;
      last;
    }
  }

  return $str;
}


sub base_project_name {
  my($self) = shift;
  return $self->transform_file_name(
               $self->remove_type_append($self->project_name()) .
               ($self->get_writing_type() == 1 ? $sname : ""));
}


sub get_static_append {
  my($self) = shift;
  return $sname;
}


sub get_type_append {
  my($self) = shift;
  my($type) = "";
  if ($self->lib_target()) {
    ## Set the type_append preserving whitespace
    if ($self->get_writing_type() == 1) {
      $type = " $staticlib";
    }
    else {
      $type = " $dynamiclib";
    }
  }
  else {
    ## Set the type_append preserving whitespace
    if ($self->get_writing_type() == 1) {
      $type = " $staticexe";
    }
    else {
      $type = " $dynamicexe";
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
    my($app) = $dynamiclib;
    $val = "";

    ## Only write dependencies for non-static projects
    ## and static exe projects
    my($wt) = $self->get_writing_type();
    if ($wt == 0 || $self->exe_target()) {
      if ($wt == 1) {
        $app = $staticlib;
      }
      foreach my $entry (@$arr) {
        my($dep) = $app;
        ## Hack for executable dependencies
        if ($entry =~ /exe/i) {
          if ($wt == 1) {
            $dep = $staticexe;
          }
          else {
            $dep = $dynamicexe;
          }
        }

        $val .= "\"$entry $dep\" ";
      }
      $val =~ s/\s+$//;
    }
  }
  return $val;
}


sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub crlf {
  my($self) = shift;
  return $self->windows_crlf();
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq "make_file_name") {
    $value = $self->base_project_name() . ".mak";
  }

  return $value;
}


sub separate_static_project {
  my($self) = shift;
  return 1;
}


sub project_file_name {
  my($self) = shift;
  return $self->project_name() . ".dsp";
}


sub static_project_file_name {
  my($self) = shift;
  return $self->project_name() . "$sname.dsp";
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
