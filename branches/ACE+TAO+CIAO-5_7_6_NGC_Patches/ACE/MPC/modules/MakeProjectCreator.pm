package MakeProjectCreator;

# ************************************************************
# Description   : A Generic Make Project Creator
# Author        : Chad Elliott
# Create Date   : 2/18/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use MakeProjectBase;
use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(MakeProjectBase ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my %info = (Creator::cplusplus => {'dllexe'   => 'makeexe',
                                   'dll'      => 'makedll',
                                   'template' => 'make',
                                  },
            Creator::csharp    => {'dllexe'   => 'make.net',
                                   'dll'      => 'make.net',
                                   'template' => 'make.net',
                                  },
            Creator::java      => {'dllexe'   => 'makeexe',
                                   'dll'      => 'makedll',
                                   'template' => 'make',
                                  },
            Creator::vb        => {'dllexe'   => 'make.net',
                                   'dll'      => 'make.net',
                                   'template' => 'make.net',
                                   },
           );

# ************************************************************
# Subroutine Section
# ************************************************************

sub languageSupported {
  return defined $info{$_[0]->get_language()};
}


sub escape_spaces {
  #my $self = shift;
  return 1;
}


sub get_dll_exe_template_input_file {
  return $info{$_[0]->get_language()}->{'dllexe'};
}


sub get_dll_template_input_file {
  return $info{$_[0]->get_language()}->{'dll'};
}


sub get_template {
  return $info{$_[0]->get_language()}->{'template'};
}

sub fill_value {
  my($self, $name) = @_;

  if ($name eq 'compilers') {
    ## The default compilers template variable value is determined by the
    ## language and directly corresponds to a group of settings in the
    ## .mpt file (make.net.mpt for csharp and makedll.mpt for all
    ## others).
    my $language = $self->get_language();
    if ($language eq Creator::java) {
      return 'java';
    }
    elsif ($language eq Creator::csharp) {
      return 'gmcs';
    }
    else {
      return 'gcc';
    }
  }
  elsif ($name eq 'language') {
    ## Allow the language to be available to the template.  Certain
    ## things are not used in make.mpd when the language is java.
    return $self->get_language();
  }
  elsif ($name eq 'main') {
    ## The main is needed when generating the makefiles for use with gcj.
    my @sources = $self->get_component_list('source_files', 1);
    my $exename = $self->find_main_file(\@sources);
    return $exename if (defined $exename);
  }

  return undef;
}
1;
