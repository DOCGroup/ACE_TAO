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
use VCProjectBase;
use XMLProjectBase;

use vars qw(@ISA);
@ISA = qw(XMLProjectBase VCProjectBase ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my %info = (Creator::cplusplus => {'ext'      => '.vcproj',
                                   'dllexe'   => 'vc7exe',
                                   'libexe'   => 'vc7libexe',
                                   'dll'      => 'vc7dll',
                                   'lib'      => 'vc7lib',
                                   'template' => 'vc7',
                                  },
            Creator::csharp    => {'ext'      => '.csproj',
                                   'dllexe'   => 'vc7csharp',
                                   'libexe'   => 'vc7csharp',
                                   'dll'      => 'vc7csharp',
                                   'lib'      => 'vc7csharp',
                                   'template' => 'vc7csharp',
                                  },
            Creator::java      => {'ext'      => '.vjsproj',
                                   'dllexe'   => 'vc7java',
                                   'libexe'   => 'vc7java',
                                   'dll'      => 'vc7java',
                                   'lib'      => 'vc7java',
                                   'template' => 'vc7java',
                                  },
            Creator::vb        => {'ext'      => '.vbproj',
                                   'dllexe'   => 'vc7vb',
                                   'libexe'   => 'vc7vb',
                                   'dll'      => 'vc7vb',
                                   'lib'      => 'vc7vb',
                                   'template' => 'vc7vb',
                                  },
           );

my %config = ('vcversion'    => '7.00',
              'forloopscope' => 'TRUE',
             );

# ************************************************************
# Subroutine Section
# ************************************************************

sub languageSupported {
  return defined $info{$_[0]->get_language()};
}


sub get_info_hash {
  #my($self, $key) = @_;
  return $info{$_[1]};
}


sub get_configurable {
  #my($self, $name) = @_;
  return $config{$_[1]};
}


sub fill_value {
  my($self, $name) = @_;

  if ($name eq 'guid') {
    ## Return a repeatable GUID for use within the template.  The values
    ## provided will be hashed and returned in a format expected by
    ## Visual Studio.
    return GUID::generate($self->project_file_name(),
                          $self->{'current_input'}, $self->getcwd());
  }
  elsif ($name eq 'language') {
    ## If this project is a Web Application, the language is website.
    ## Since Visual Studio 2003 doesn't support Web Applications, this
    ## will never happen.  However, this code is shared by the vc8
    ## project type, so it can happen then.
    return Creator::website if ($self->get_assignment('webapp'));

    ## Also for the vc8 project type, the language is stored in the
    ## project file as a comment when external C# references need to be
    ## added to the resulting project.  Since a C++ project can mix with
    ## C#, the particular project language can not be determined by the
    ## workspace language.
    return $self->get_language();
  }

  ## Consult another method for this template name.  This method is
  ## overrridden by the other project creators that inherit from this
  ## one.
  return $self->get_configurable($name);
}


sub project_file_extension {
  return $_[0]->get_info_hash($_[0]->get_language())->{'ext'};
}


sub get_dll_exe_template_input_file {
  return $_[0]->get_info_hash($_[0]->get_language())->{'dllexe'};
}


sub get_lib_exe_template_input_file {
  return $_[0]->get_info_hash($_[0]->get_language())->{'libexe'};
}


sub get_dll_template_input_file {
  return $_[0]->get_info_hash($_[0]->get_language())->{'dll'};
}


sub get_lib_template_input_file {
  return $_[0]->get_info_hash($_[0]->get_language())->{'lib'};
}


sub get_template {
  return $_[0]->get_info_hash($_[0]->get_language())->{'template'};
}


sub get_cmdsep_symbol {
  #my $self = shift;
  return '&amp;';
}


1;
