package VA4ProjectCreator;

# ************************************************************
# Description   : A VA4 Project Creator
# Author        : Chad Elliott
# Create Date   : 7/8/2002
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

my($sname) = "_Static";

# ************************************************************
# Subroutine Section
# ************************************************************

sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;

  if ($key eq 'depends' && $val ne "") {
    my($wt)  = $self->get_writing_type();
    my($arr) = $self->create_array($val);
    $val = "";
    foreach my $entry (@$arr) {
      $val .= "\"" . ($wt == 1 ? $self->static_project_file_name($entry) :
                                 $self->project_file_name($entry)) . "\" ";
    }
    $val =~ s/\s+$//;
  }
  return $val;
}


sub convert_slashes {
  my($self) = shift;
  return 0;
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


sub separate_static_project {
  my($self) = shift;
  return 1;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name.icc";
}


sub static_project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name$sname.icc";
}


sub get_dll_exe_template_input_file {
  my($self) = shift;
  return "va4iccdllexe";
}


sub get_lib_exe_template_input_file {
  my($self) = shift;
  return "va4icclibexe";
}


sub get_lib_template_input_file {
  my($self) = shift;
  return "va4icclib";
}


sub get_dll_template_input_file {
  my($self) = shift;
  return "va4iccdll";
}


sub get_template {
  my($self) = shift;
  return "va4icc";
}


1;
