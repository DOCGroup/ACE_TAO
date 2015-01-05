package GNUACEProjectCreator;

# ************************************************************
# Description   : A GNU Project Creator for ACE
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use MakeProjectBase;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(MakeProjectBase ProjectCreator);

# ************************************************************
# Subroutine Section
# ************************************************************

sub expand_variables_from_template_values {
  #my($self) = shift;
  return 0;
}


sub convert_slashes {
  #my($self) = shift;
  return 0;
}


sub fill_value {
  my($self) = shift;
  my($name) = shift;

  if ($name eq 'vpath') {
    my(%vpath) = ();
    foreach my $item ($self->get_component_list('source_files')) {
      my($dname) = $self->relative($self->mpc_dirname($item));
      if ($dname ne '.') {
        $vpath{$dname} = 1;
      }
    }
    my($str) = join(':', keys %vpath);
    if ($str ne '') {
      return 'VPATH = .:' . $str . $self->crlf();
    }
  }
  elsif ($name eq 'tao') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    return ((defined $incs && $incs =~ /tao/i) ||
            (defined $libs && $libs =~ /tao/i));
  }
  elsif ($name eq 'ciao') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    return ((defined $incs && $incs =~ /ciao/i) ||
            (defined $libs && $libs =~ /ciao/i));
  }
  elsif ($name eq 'dance') {
    my($incs) = $self->get_assignment('includes');
    my($libs) = $self->get_assignment('libpaths');
    return ((defined $incs && $incs =~ /DAnCE/i) ||
            (defined $libs && $libs =~ /DAnCE/i));
  }
  elsif ($name eq 'genins') {
    my $ins = '';
    $self->get_install_info(sub { $ins .= '#' . $_[0] });
    return $ins;
  }

  return undef;
}


sub project_file_prefix {
  #my($self) = shift;
  return 'GNUmakefile.';
}


sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return 'gnuexe';
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return 'gnudll';
}


sub get_template {
  #my($self) = shift;
  return 'gnu';
}

1;
