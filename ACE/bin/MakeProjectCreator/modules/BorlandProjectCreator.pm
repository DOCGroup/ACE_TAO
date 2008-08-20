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
use WinProjectBase;
use File::Basename;

use vars qw(@ISA);
@ISA = qw(WinProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub dollar_special {
  #my($self) = shift;
  return 1;
}

sub get_and_symbol {
  #my $self = shift;
  return '&$(__TRICK_BORLAND_MAKE__)&';
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
      my($dirname) = $self->mpc_dirname($file);
      if ($dirname eq '') {
        $dirname = '.';
      }
      elsif ($self->convert_slashes()) {
        $dirname = $self->slash_to_backslash($dirname);
      }
      $dirnames{$dirname} = 1;
    }

    ## Sort the directories to ensure that '.' comes first
    $value = join(';', sort keys %dirnames);
  }
  elsif ($name eq 'relwd') {
    my($useenv) = $self->get_use_env();
    my($rel)    = ($useenv ? \%ENV : $self->get_relative());
    $value = $self->getcwd();

    foreach my $key (keys %$rel) {
      ## Do not use PWD or CD if we are expanding environment variables.
      ## They could conflict with the "real" values we're looking for.
      if ($useenv && ($key eq 'PWD' || $key eq 'CD')) {
        next;
      }

      ## Get the relative replacement value and convert back-slashes
      my($val) = $$rel{$key};
      $val =~ s/\\/\//g;

      ## We only need to check for reverse replacement if the length
      ## of the string is less than or equal to the length of our
      ## replacement value or the string has a slash at the position
      ## of the length of the replacement value
      my($vlen) = length($val);
      if (length($value) <= $vlen || substr($value, $vlen, 1) eq '/') {
        ## Cut the string down by the length of the replacement value
        my($lval) = substr($value, 0, $vlen);

        ## Here we make an assumption that we
        ## have a case-insensitive file system.
        if (lc($lval) eq lc($val)) {
          substr($value, 0, length($val) + 1) = '';
          last;
        }
      }
    }
    $value = $self->slash_to_backslash($value);
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
