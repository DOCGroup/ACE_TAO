package ProjectCreator;

# ************************************************************
# Description   : Base class for all project creators
# Author        : Chad Elliott
# Create Date   : 3/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Path;
use File::Compare;
use File::Basename;

use Creator;
use TemplateInputReader;
use TemplateParser;
use FeatureParser;

use vars qw(@ISA);
@ISA = qw(Creator);

# ************************************************************
# Data Section
# ************************************************************

my($BaseClassExtension)      = 'mpb';
my($ProjectCreatorExtension) = 'mpc';
my($TemplateExtension)       = 'mpd';
my($TemplateInputExtension)  = 'mpt';

## Valid names for assignments within a project
my(%validNames) = ('exename'         => 1,
                   'sharedname'      => 1,
                   'staticname'      => 1,
                   'libpaths'        => 1,
                   'install'         => 1,
                   'includes'        => 1,
                   'idlflags'        => 1,
                   'idlpreprocessor' => 1,
                   'defaultlibs'     => 1,
                   'depends'         => 1,
                   'libs'            => 1,
                   'lit_libs'        => 1,
                   'pch_header'      => 1,
                   'pch_source'      => 1,
                   'ssl'             => 1,
                   'dllout'          => 1,
                   'libout'          => 1,
                   'dllflags'        => 1,
                   'libflags'        => 1,
                   'version'         => 1,
                   'requires'        => 1,
                   'avoids'          => 1,
                   'compname'        => 1,
                   'comps'           => 1,
                   'tagname'         => 1,
                   'tagchecks'       => 1,
                   'include_dir'     => 1,
                   'core'            => 1,
                   'idlgendir'       => 1,
                   'macros'          => 1,
                  );

## Custom definitions only
my(%customDefined) = ('automatic'               => 1,
                      'command'                 => 1,
                      'commandflags'            => 1,
                      'inputext'                => 1,
                      'output_option'           => 1,
                      'pch_option'              => 1,
                      'pre_extension'           => 1,
                      'pre_filename'            => 1,
                      'source_outputext'        => 1,
                      'template_outputext'      => 1,
                      'header_outputext'        => 1,
                      'inline_outputext'        => 1,
                      'documentation_outputext' => 1,
                      'resource_outputext'      => 1,
                      'generic_outputext'       => 1,
                     );

## Custom sections as well as definitions
my(%custom) = ('commandflags'  => 1,
               'gendir'        => 1,
              );

## Deal with these components in a special way
my(@specialComponents) = ('header_files', 'inline_files');

## Valid component names within a project along with the valid file extensions
my(%vc) = ('source_files'        => [ "\\.cpp", "\\.cxx", "\\.cc", "\\.c", "\\.C", ],
           'template_files'      => [ "_T\\.cpp", "_T\\.cxx", "_T\\.cc", "_T\\.c", "_T\\.C", ],
           'header_files'        => [ "\\.h", "\\.hxx", "\\.hh", ],
           'inline_files'        => [ "\\.i", "\\.inl", ],
           'idl_files'           => [ "\\.idl", ],
           'documentation_files' => [ "README", "readme", "\\.doc", "\\.txt", ],
           'resource_files'      => [ "\\.rc", ],
          );

## Exclude these extensions when auto generating the component values
my(%ec) = ('source_files' => [ "_T\\.cpp", "_T\\.cxx", "_T\\.cc", "_T\\.C", ],
          );

## Match up assignments with the valid components
my(%ma) = ('idl_files'    => [ 'idlgendir', 'idlflags' ],
          );

my(%genext) = ('idl_files' => {'automatic'     => 1,
                               'pre_filename'  => [ '' ],
                               'pre_extension' => [ 'C', 'S' ],
                               'source_files'  => [ '\\.cpp', '\\.cxx', '\\.cc', '\\.C', ],
                               'inline_files'  => [ '\\.i', '\\.inl', ],
                               'header_files'  => [ '\\.h', '\\.hxx', '\\.hh', ],
                              },
              );

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)     = shift;
  my($global)    = shift;
  my($inc)       = shift;
  my($template)  = shift;
  my($ti)        = shift;
  my($dynamic)   = shift;
  my($static)    = shift;
  my($relative)  = shift;
  my($addtemp)   = shift;
  my($addproj)   = shift;
  my($progress)  = shift;
  my($toplevel)  = shift;
  my($baseprojs) = shift;
  my($gfeature)  = shift;
  my($feature)   = shift;
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, $dynamic, $static,
                                $relative, $addtemp, $addproj,
                                $progress, $toplevel, $baseprojs,
                                $feature, 'project');

  $self->{$self->{'type_check'}}   = 0;
  $self->{'project_info'}          = [];
  $self->{'reading_parent'}        = [];
  $self->{'dexe_template_input'}   = undef;
  $self->{'lexe_template_input'}   = undef;
  $self->{'lib_template_input'}    = undef;
  $self->{'dll_template_input'}    = undef;
  $self->{'writing_type'}          = 0;
  $self->{'flag_overrides'}        = {};
  $self->{'special_supplied'}      = {};
  $self->{'verbatim'}              = {};
  $self->{'type_specific_assign'}  = {};
  $self->{'pctype'}                = $self->extractType("$self");
  $self->{'defaulted'}             = {};
  $self->{'custom_types'}          = {};
  $self->{'parents_read'}          = {};
  $self->{'feature_parser'}        = new FeatureParser($gfeature, $feature);
  $self->{'convert_slashes'}       = $self->convert_slashes();
  $self->{'sort_files'}            = $self->sort_files();
  $self->reset_generating_types();

  return $self;
}


sub read_global_configuration {
  my($self)   = shift;
  my($input)  = $self->get_global_cfg();
  my($status) = 1;

  if (defined $input) {
    $self->{'reading_global'} = 1;
    $status = $self->parse_file($input);
    $self->{'reading_global'} = 0;
  }

  return $status;
}


sub parse_line {
  my($self)   = shift;
  my($ih)     = shift;
  my($line)   = shift;
  my($type)   = $self->{'grammar_type'};
  my($status,
     $errorString,
     @values) = $self->parse_known($line);

  ## parse_known() passes back an array of values
  ## that make up the contents of the line parsed.
  ## The array can have 0 to 3 items.  The first,
  ## if defined, is always an identifier of some
  ## sort.

  if ($status && defined $values[0]) {
    if ($values[0] eq $type) {
      my($name)      = $values[1];
      my($typecheck) = $self->{'type_check'};
      if (defined $name && $name eq '}') {
        ## Project Ending
        my($rp) = $self->{'reading_parent'};
        if (!defined $$rp[0] && !$self->{'reading_global'}) {
          ## Fill in all the default values
          $self->generate_defaults();

          ## Fill in type specific assignments
          my($tsa) = $self->{'type_specific_assign'}->{$self->{'pctype'}};
          if (defined $tsa) {
            foreach my $key (keys %$tsa) {
              $self->process_assignment_add($key, $$tsa{$key});
            }
          }

          ## Perform any additions, subtractions
          ## or overrides for the project values.
          my($addproj) = $self->get_addproj();
          foreach my $ap (keys %$addproj) {
            if (defined $validNames{$ap}) {
              my($val) = $$addproj{$ap};
              if ($$val[0] > 0) {
                $self->process_assignment_add($ap, $$val[1]);
              }
              elsif ($$val[0] < 0) {
                $self->process_assignment_sub($ap, $$val[1]);
              }
              else {
                $self->process_assignment($ap, $$val[1]);
              }
            }
            else {
              $errorString = 'ERROR: Invalid ' .
                             "assignment modification name: $ap";
              $status = 0;
            }
          }

          if ($status) {
            ## End of project; Write out the file.
            ($status, $errorString) = $self->write_project();

            foreach my $key (keys %{$self->{'valid_components'}}) {
              delete $self->{$key};
              $self->{'defaulted'}->{$key} = 0;
            }
            $self->{'assign'}               = {};
            $self->{'verbatim'}             = {};
            $self->{'special_supplied'}     = {};
            $self->{'type_specific_assign'} = {};
            $self->{'flag_overrides'}       = {};
            $self->{'parents_read'}         = {};
            $self->reset_generating_types();
          }
        }
        $self->{$typecheck} = 0;
      }
      else {
        ## Project Beginning
        ## Deal with the inheritance hiearchy first
        my($parents) = $values[2];

        ## Add in the base projects from the command line
        if (!$self->{'reading_global'} &&
            !defined $self->{'reading_parent'}->[0]) {
          my($baseprojs) = $self->get_baseprojs();

          if (defined $parents) {
            foreach my $base (@$baseprojs) {
              my($found) = 0;
              foreach my $parent (@$parents) {
                if ($base eq $parent) {
                  $found = 1;
                  last;
                }
              }
              if (!$found) {
                push(@$parents, $base);
              }
            }
          }
          else {
            $parents = $baseprojs;
          }
        }

        if (defined $parents) {
          foreach my $parent (@$parents) {
            ## Read in the parent onto ourself
            my($file) = $self->search_include_path(
                                 "$parent.$BaseClassExtension");
            if (!defined $file) {
              $file = $self->search_include_path(
                                   "$parent.$ProjectCreatorExtension");
            }

            if (defined $file) {
              foreach my $currently (@{$self->{'reading_parent'}}) {
                if ($currently eq $file) {
                  $status = 0;
                  $errorString = 'ERROR: Cyclic inheritance detected: ' .
                                 $parent;
                }
              }

              if ($status) {
                if (!defined $self->{'parents_read'}->{$file}) {
                  $self->{'parents_read'}->{$file} = 1;
                  ## Begin reading the parent
                  push(@{$self->{'reading_parent'}}, $file);
                  $status = $self->parse_file($file);
                  pop(@{$self->{'reading_parent'}});

                  if (!$status) {
                    $errorString = "ERROR: Invalid parent: $parent";
                  }
                }
              }
            }
            else {
              $status = 0;
              $errorString = "ERROR: Unable to locate parent: $parent";
            }
          }
        }

        ## Set up some initial values
        if (defined $name) {
          if ($name =~ /[\/\\]/) {
            $status = 0;
            $errorString = 'ERROR: Projects can not have a slash ' .
                           'or a back slash in the name';
          }
          else {
            $name =~ s/^\(\s*//;
            $name =~ s/\s*\)$//;
            $name = $self->transform_file_name($name);

            ## Replace any *'s with the default name
            my($def) = $self->get_default_project_name();
            $name = $self->fill_type_name($name, $def);

            $self->process_assignment('project_name', $name);
          }
        }
        $self->{$typecheck} = 1;

        ## Copy each value from global_assign into assign
        if (!$self->{'reading_global'}) {
          foreach my $key (keys %{$self->{'global_assign'}}) {
            if (!defined $self->{'assign'}->{$key}) {
              $self->{'assign'}->{$key} = $self->{'global_assign'}->{$key};
            }
          }
        }
      }
    }
    elsif ($values[0] eq 'assignment') {
      my($name)  = $values[1];
      my($value) = $values[2];
      if (defined $validNames{$name}) {
        $self->process_assignment($name, $value);
      }
      else {
        $errorString = "ERROR: Invalid assignment name: $name";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'assign_add') {
      my($name)  = $values[1];
      my($value) = $values[2];
      if (defined $validNames{$name}) {
        $self->process_assignment_add($name, $value);
      }
      else {
        $errorString = "ERROR: Invalid addition name: $name";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'assign_sub') {
      my($name)  = $values[1];
      my($value) = $values[2];
      if (defined $validNames{$name}) {
        $self->process_assignment_sub($name, $value);
      }
      else {
        $errorString = "ERROR: Invalid subtraction name: $name";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'component') {
      my($comp) = $values[1];
      my($name) = $values[2];
      if (defined $name) {
        $name =~ s/^\(\s*//;
        $name =~ s/\s*\)$//;
      }
      else {
        $name = 'default';
      }

      my($vc) = $self->{'valid_components'};
      if (defined $$vc{$comp}) {
        if (!$self->parse_components($ih, $comp, $name)) {
          $errorString = "ERROR: Unable to process $comp";
          $status = 0;
        }
      }
      else {
        if ($comp eq 'verbatim') {
          my($type, $loc) = split(/\s*,\s*/, $name);
          if (!$self->parse_verbatim($ih, $comp, $type, $loc)) {
            $errorString = "ERROR: Unable to process $comp";
            $status = 0;
          }
        }
        elsif ($comp eq 'specific') {
          ($status, $errorString) = $self->parse_scope(
                       $ih, $values[1], $values[2], \%validNames,
                       $self->{'type_specific_assign'}->{$self->{'pctype'}});
        }
        elsif ($comp eq 'define_custom') {
          ($status, $errorString) = $self->parse_define_custom($ih, $name);
        }
        else {
          $errorString = "ERROR: Invalid component name: $comp";
          $status = 0;
        }
      }
    }
    else {
      $errorString = "ERROR: Unrecognized line: $line";
      $status = 0;
    }
  }
  elsif ($status == -1) {
    $status = 0;
  }

  return $status, $errorString;
}


sub parse_scoped_assignment {
  my($self)   = shift;
  my($tag)    = shift;
  my($type)   = shift;
  my($name)   = shift;
  my($value)  = shift;
  my($flags)  = shift;
  my($order)  = shift;
  my($over)   = {};
  my($status) = 0;

  if (defined $self->{'matching_assignments'}->{$tag}) {
    foreach my $possible (@{$self->{'matching_assignments'}->{$tag}}) {
      if ($possible eq $name) {
        $status = 1;
        last;
      }
    }
  }

  if ($status) {
    if (defined $self->{'flag_overrides'}->{$tag}) {
      $over = $self->{'flag_overrides'}->{$tag};
    }
    else {
      $self->{'flag_overrides'}->{$tag} = $over;
    }

    if ($type eq 'assignment') {
      $self->process_assignment($name, $value, $flags);
    }
    elsif ($type eq 'assign_add') {
      ## If there is no value in $$flags, then we need to get
      ## the outer scope value and put it in there.
      if (!defined $self->get_assignment($name, $flags)) {
        my($outer) = $self->get_assignment($name);
        $self->process_assignment($name, $outer, $flags);
      }
      $self->process_assignment_add($name, $value, $flags, $order);
    }
    elsif ($type eq 'assign_sub') {
      ## If there is no value in $$flags, then we need to get
      ## the outer scope value and put it in there.
      if (!defined $self->get_assignment($name, $flags)) {
        my($outer) = $self->get_assignment($name);
        $self->process_assignment($name, $outer, $flags);
      }
      $self->process_assignment_sub($name, $value, $flags);
    }
  }
  return $status;
}


sub parse_components {
  my($self)    = shift;
  my($fh)      = shift;
  my($tag)     = shift;
  my($name)    = shift;
  my($current) = '000_FILES';
  my($status)  = 1;
  my($names)   = {};
  my($comps)   = {};
  my($order)   = 0;
  my($set)     = 0;
  my(%flags)   = ();
  my($custom)  = defined $self->{'generated_exts'}->{$tag};

  if ($custom) {
    ## For the custom scoped assignments, we want to put a copy of
    ## the original custom defined values in our flags associative array.
    foreach my $key (keys %custom) {
      if (defined $self->{'generated_exts'}->{$tag}->{$key}) {
        $flags{$key} = $self->{'generated_exts'}->{$tag}->{$key};
      }
    }
  }

  if (defined $self->{$tag}) {
    $names = $self->{$tag};
  }
  else {
    $self->{$tag} = $names;
  }
  if (defined $$names{$name}) {
    $comps = $$names{$name};
  }
  else {
    $$names{$name} = $comps;
  }
  if (!defined $$comps{$current}) {
    $$comps{$current} = [];
  }

  foreach my $special (@specialComponents) {
    if ($special eq $tag) {
      $self->{'special_supplied'}->{$tag} = 1;
      last;
    }
  }

  while(<$fh>) {
    my($line) = $self->strip_line($_);

    if ($line eq '') {
    }
    elsif ($line =~ /^(\w+)\s*{$/) {
      if (!defined $current || !$set) {
        if (defined $current && !defined $$comps{$current}->[0]) {
          ## The default components name was never used
          ## so we remove it from the components
          delete $$comps{$current};
        }
        $current = sprintf("%03d_$1", $order);
        $set = 1;
        $order++;
        if (!defined $$comps{$current}) {
          $$comps{$current} = [];
        }
      }
      else {
        $status = 0;
        last;
      }
    }
    elsif ($line =~ /^}/) {
      if (defined $current && $set) {
        $current = undef;
      }
      else {
        ## This is not an error,
        ## this is the end of the components
        last;
      }
    }
    elsif (defined $current) {
      my(@values) = ();
      ## If this returns true, then we've found an assignment
      if ($self->parse_assignment($line, \@values)) {
        $status = $self->parse_scoped_assignment($tag, @values,
                                                 \%flags, $custom);
        if (!$status) {
          last;
        }
      }
      else {
        my($over) = $self->{'flag_overrides'}->{$tag};
        if (defined $over) {
          $$over{$line} = \%flags;
        }
        my($array) = $$comps{$current};
        push(@$array, $line);
      }
    }
    else {
      $status = 0;
      last;
    }
  }

  return $status;
}


sub parse_verbatim {
  my($self)    = shift;
  my($fh)      = shift;
  my($tag)     = shift;
  my($type)    = shift;
  my($loc)     = shift;

  ## All types are lowercase
  $type = lc($type);

  if (!defined $self->{'verbatim'}->{$type}) {
    $self->{'verbatim'}->{$type} = {};
  }
  $self->{'verbatim'}->{$type}->{$loc} = [];
  my($array) = $self->{'verbatim'}->{$type}->{$loc};

  while(<$fh>) {
    my($line) = $self->strip_line($_);

    if ($line eq '') {
    }
    elsif ($line =~ /^}/) {
      ## This is not an error,
      ## this is the end of the components
      last;
    }
    else {
      push(@$array, $line);
    }
  }

  return 1;
}


sub process_array_assignment {
  my($self)  = shift;
  my($aref)  = shift;
  my($type)  = shift;
  my($array) = shift;

  if (!defined $$aref || $type eq 'assignment') {
    if ($type ne 'assign_sub') {
      $$aref = $array;
    }
  }
  else {
    if ($type eq 'assign_add') {
      push(@{$$aref}, @$array);
    }
    elsif ($type eq 'assign_sub') {
      my($count) = scalar(@{$$aref});
      for(my $i = 0; $i < $count; ++$i) {
        foreach my $val (@$array) {
          if ($$aref->[$i] eq $val) {
            splice(@{$$aref}, $i, 1);
            --$i;
            --$count;
            last;
          }
        }
      }
    }
  }
}


sub parse_define_custom {
  my($self)        = shift;
  my($fh)          = shift;
  my($tag)         = shift;
  my($status)      = 0;
  my($errorString) = "ERROR: Unable to process $tag";
  my(%flags)       = ();

  ## Make the tag something _files
  $tag = lc($tag) . '_files';

  if (defined $self->{'valid_components'}->{$tag}) {
    $errorString = "ERROR: $tag has already been defined";
  }
  else {
    ## Update the custom_types assignment
    $self->process_assignment_add('custom_types', $tag);

    if (!defined $self->{'matching_assignments'}->{$tag}) {
      my(@keys) = keys %custom;
      $self->{'matching_assignments'}->{$tag} = \@keys;
    }

    while(<$fh>) {
      my($line) = $self->strip_line($_);

      if ($line eq '') {
      }
      elsif ($line =~ /^}/) {
        $status = 1;
        $errorString = '';
        if (!defined $self->{'generated_exts'}->{$tag}->{'pre_filename'}) {
          $self->{'generated_exts'}->{$tag}->{'pre_filename'} = [ '' ];
        }
        if (!defined $self->{'generated_exts'}->{$tag}->{'pre_extension'}) {
          $self->{'generated_exts'}->{$tag}->{'pre_extension'} = [ '' ];
        }
        if (!defined $self->{'generated_exts'}->{$tag}->{'automatic'}) {
          $self->{'generated_exts'}->{$tag}->{'automatic'} = 1;
        }
        last;
      }
      else {
        my(@values) = ();
        ## If this returns true, then we've found an assignment
        if ($self->parse_assignment($line, \@values)) {
          my($type)  = $values[0];
          my($name)  = $values[1];
          my($value) = $values[2];
          if (defined $customDefined{$name}) {
            if ($name eq 'inputext') {
              $value = $self->escape_regex_special($value);
              my(@array) = split(/\s*,\s*/, $value);
              $self->process_array_assignment(
                        \$self->{'valid_components'}->{$tag}, $type, \@array);
            }
            else {
              if (!defined $self->{'generated_exts'}->{$tag}) {
                $self->{'generated_exts'}->{$tag} = {};
              }
              if ($value =~ /\$\(.*\)/) {
                my($envstart, $envend) = $self->get_env_accessor();
                if (defined $envstart) {
                  if (!defined $envend) {
                    $envend = '';
                  }
                  $value =~ s/\$\(([^\)]+)\)/$envstart$1$envend/g;
                }
              }
              if ($name eq 'command'      || $name eq 'automatic'     ||
                  $name eq 'commandflags' || $name eq 'output_option' ||
                  $name eq 'pch_option') {
                if ($type eq 'assignment') {
                  $self->process_assignment(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
                elsif ($type eq 'assign_add') {
                  $self->process_assignment_add(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
                elsif ($type eq 'assign_sub') {
                  $self->process_assignment_sub(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
              }
              else {
                ## Transform the name from something outputext to
                ## something files.  We expect this to match the
                ## names of valid_assignments.
                $name =~ s/outputext/files/g;

                ## Get it ready for regular expressions
                $value = $self->escape_regex_special($value);

                ## Process the array assignment
                my(@array) = split(/\s*,\s*/, $value);
                $self->process_array_assignment(
                            \$self->{'generated_exts'}->{$tag}->{$name},
                            $type, \@array);
              }
            }
          }
          else {
            $status = 0;
            $errorString =  "ERROR: Invalid assignment name: $name";
            last;
          }
        }
      }
    }
  }

  return $status, $errorString;
}


sub handle_scoped_end {
  my($self)  = shift;
  my($type)  = shift;
  my($flags) = shift;

  if (defined $self->{'type_specific_assign'}->{$type}) {
    foreach my $key (keys %$flags) {
      $self->{'type_specific_assign'}->{$type}->{$key} = $$flags{$key};
    }
  }
  else {
    $self->{'type_specific_assign'}->{$type} = $flags;
  }
}


sub process_duplicate_modification {
  my($self)   = shift;
  my($name)   = shift;
  my($assign) = shift;

  ## If we are modifying the libs, libpaths or includes assignment with
  ## either addition or subtraction, we are going to
  ## perform a little fix on the value to avoid multiple
  ## libraries and to try to insure the correct linking order
  if ($name eq 'libs' || $name eq 'libpaths' || $name eq 'includes') {
    my($nval) = $self->get_assignment($name, $assign);
    if (defined $nval) {
      my($parts) = $self->create_array($nval);
      my(%seen)  = ();
      my($value) = '';
      foreach my $part (reverse @$parts) {
        if (!defined $seen{$part}) {
          $value = "$part $value";
          $seen{$part} = 1;
        }
      }
      $self->process_assignment($name, $value, $assign);
    }
  }
}


sub read_template_input {
  my($self)        = shift;
  my($status)      = 1;
  my($errorString) = '';
  my($file)        = undef;
  my($tag)         = undef;
  my($ti)          = $self->get_ti_override();
  my($override)    = 0;

  if ($self->exe_target()) {
    if ($self->{'writing_type'} == 1) {
      $tag = 'lexe_template_input';
      if (!defined $self->{$tag}) {
        if (defined $$ti{'lib_exe'}) {
          $file = $$ti{'lib_exe'};
          $override = 1;
        }
        else {
          $file = $self->get_lib_exe_template_input_file();
        }
      }
    }
    else {
      $tag = 'dexe_template_input';
      if (!defined $self->{$tag}) {
        if (defined $$ti{'dll_exe'}) {
          $file = $$ti{'dll_exe'};
          $override = 1;
        }
        else {
          $file = $self->get_dll_exe_template_input_file();
        }
      }
    }
  }
  else {
    if ($self->{'writing_type'} == 1) {
      $tag = 'lib_template_input';
      if (!defined $self->{$tag}) {
        if (defined $$ti{'lib'}) {
          $file = $$ti{'lib'};
          $override = 1;
        }
        else {
          $file = $self->get_lib_template_input_file();
        }
      }
    }
    else {
      $tag = 'dll_template_input';
      if (!defined $self->{$tag}) {
        if (defined $$ti{'dll'}) {
          $file = $$ti{'dll'};
          $override = 1;
        }
        else {
          $file = $self->get_dll_template_input_file();
        }
      }
    }
  }

  if (defined $file) {
    my($file) = $self->search_include_path("$file.$TemplateInputExtension");
    if (defined $file) {
      $self->{$tag} = new TemplateInputReader($self->get_include_path());
      ($status, $errorString) = $self->{$tag}->read_file($file);
    }
    else {
      if ($override) {
        $status = 0;
        $errorString = 'Unable to locate template input file.';
      }
    }
  }

  return $status, $errorString;
}


sub already_added {
  my($self)  = shift;
  my($array) = shift;
  my($name)  = shift;

  foreach my $file (@$array) {
    if ($file eq $name) {
      return 1;
    }
  }
  return 0;
}


sub add_generated_files {
  my($self)    = shift;
  my($gentype) = shift;
  my($tag)     = shift;
  my($arr)     = shift;
  my($names)   = $self->{$tag};
  my($wanted)  = $self->{'valid_components'}->{$gentype}->[0];

  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my(@added) = ();
      my($array) = $$comps{$key};
      foreach my $i (@$arr) {
        my($file) = $i;
        $file =~ s/$wanted$//;
        foreach my $pf (@{$self->{'generated_exts'}->{$gentype}->{'pre_filename'}}) {
          foreach my $pe (@{$self->{'generated_exts'}->{$gentype}->{'pre_extension'}}) {
            $self->list_generated_file($gentype, $tag, \@added, "$pf$file$pe");
          }
        }
      }
      unshift(@$array, @added);
    }
  }
}


sub generate_default_target_names {
  my($self) = shift;

  if (!$self->exe_target()) {
    my($sharedname) = $self->get_assignment('sharedname');
    if (defined $sharedname &&
        !defined $self->get_assignment('staticname')) {
      $self->process_assignment('staticname', $sharedname);
    }
    my($staticname) = $self->get_assignment('staticname');
    if (defined $staticname &&
        !defined $self->get_assignment('sharedname')) {
      $self->process_assignment('sharedname', $staticname);
      $sharedname = $staticname;
    }

    ## If it's neither an exe or library target, we will search
    ## through the source files for a main()
    my(@sources) = $self->get_component_list('source_files');
    if (!$self->lib_target()) {
      my($fh)      = new FileHandle();
      my($exename) = undef;
      foreach my $file (@sources) {
        if (open($fh, $file)) {
          while(<$fh>) {
            ## Remove c++ comments (ignore c style comments for now)
            $_ =~ s/\/\/.*//;

            ## Check for main
            if (/(main|ACE_MAIN|ACE_WMAIN|ACE_TMAIN)\s*\(/) {
              ## If we found a main, set the exename to the basename
              ## of the cpp file with the extension removed
              $exename = basename($file);
              $exename =~ s/\.[^\.]+$//;
              last;
            }
          }
          close($fh);
        }

        ## Set the exename assignment
        if (defined $exename) {
          $self->process_assignment('exename', $exename);
          last;
        }
      }

      ## If we still don't have a project type, then we will
      ## default to a library if there are source files
      if (!$self->exe_target() && $#sources >= 0) {
        my($base) = $self->get_assignment('project_name');
        $self->process_assignment('sharedname', $base);
        $self->process_assignment('staticname', $base);
      }
    }
  }
}


sub generate_default_pch_filenames {
  my($self)  = shift;
  my($files) = shift;
  my($pname) = $self->escape_regex_special(
                       $self->get_assignment('project_name'));

  if (!defined $self->get_assignment('pch_header')) {
    my($count)    = 0;
    my($matching) = undef;
    foreach my $file (@$files) {
      foreach my $ext (@{$self->{'valid_components'}->{'header_files'}}) {
        if ($file =~ /(.*_pch$ext)/) {
          $self->process_assignment('pch_header', $1);
          ++$count;
          if ($file =~ /$pname/) {
            $matching = $file;
          }
          last;
        }
      }
    }
    if ($count > 1 && defined $matching) {
      $self->process_assignment('pch_header', $matching);
    }
  }

  if (!defined $self->get_assignment('pch_source')) {
    my($count)    = 0;
    my($matching) = undef;
    foreach my $file (@$files) {
      foreach my $ext (@{$self->{'valid_components'}->{'source_files'}}) {
        if ($file =~ /(.*_pch$ext)/) {
          $self->process_assignment('pch_source', $1);
          ++$count;
          if ($file =~ /$pname/) {
            $matching = $file;
          }
          last;
        }
      }
    }
    if ($count > 1 && defined $matching) {
      $self->process_assignment('pch_source', $matching);
    }
  }
}


sub fix_pch_filenames {
  my($self) = shift;
  foreach my $type ('pch_header', 'pch_source') {
    my($pch) = $self->get_assignment($type);
    if (defined $pch && $pch eq '') {
      $self->process_assignment($type, undef);
    }
  }
}


sub remove_extra_pch_listings {
  my($self) = shift;
  my(@pchs) = ('pch_header', 'pch_source');
  my(@tags) = ('header_files', 'source_files');

  for(my $j = 0; $j <= $#pchs; ++$j) {
    my($pch) = $self->get_assignment($pchs[$j]);

    if (defined $pch) {
      ## If we are converting slashes, then we need to
      ## convert the pch file back to forward slashes
      if ($self->{'convert_slashes'}) {
        $pch =~ s/\\/\//g;
      }

      ## Find out which files are duplicated
      my($names) = $self->{$tags[$j]};
      foreach my $name (keys %$names) {
        my($comps) = $$names{$name};
        foreach my $key (keys %$comps) {
          my($array) = $$comps{$key};
          my($count) = scalar(@$array);
          for(my $i = 0; $i < $count; ++$i) {
            if ($pch eq $$array[$i]) {
              splice(@$array, $i, 1);
              --$count;
            }
          }
        }
      }
    }
  }
}


sub is_special_tag {
  my($self) = shift;
  my($tag)  = shift;

  foreach my $t (@specialComponents) {
    if ($tag eq $t) {
      return 1;
    }
  }

  return 0;
}


sub sift_files {
  my($self)  = shift;
  my($files) = shift;
  my($exts)  = shift;
  my($pchh)  = shift;
  my($pchc)  = shift;
  my($tag)   = shift;
  my($array) = shift;
  my(@saved) = ();
  my($ec)    = $self->{'exclude_components'};

  foreach my $file (@$files) {
    foreach my $ext (@$exts) {
      ## Always exclude the precompiled header and cpp
      if ($file =~ /$ext$/ && (!defined $pchh || $file ne $pchh) &&
                              (!defined $pchc || $file ne $pchc)) {
        my($exclude) = 0;
        if (defined $$ec{$tag}) {
          my($excludes) = $$ec{$tag};
          foreach my $exc (@$excludes) {
            if ($file =~ /$exc$/) {
              $exclude = 1;
              last;
            }
          }
        }
        elsif ($tag eq 'resource_files') {
          ## Save these files for later.  There may
          ## be more than one and we want to try and
          ## find the one that corresponds to this project
          $exclude = 1;
          push(@saved, $file);
        }

        if (!$exclude) {
          if (!$self->already_added($array, $file)) {
            push(@$array, $file);
          }
        }
        last;
      }
    }
  }

  ## Now deal with the saved files
  if (defined $saved[0]) {
    if ($#saved == 0) {
      ## Theres only one rc file, take it
      push(@$array, $saved[0]);
    }
    else {
      my($unescaped) = $self->transform_file_name(
                               $self->get_assignment('project_name'));
      my($pjname)    = $self->escape_regex_special($unescaped);
      foreach my $save (@saved) {
        my($file) = $self->escape_regex_special($save);
        if ($unescaped =~ /$file/ || $save =~ /$pjname/) {
          if (!$self->already_added($array, $save)) {
            push(@$array, $save);
          }
        }
      }
    }
  }
}


sub generate_default_components {
  my($self)   = shift;
  my($files)  = shift;
  my($passed) = shift;
  my($vc)     = $self->{'valid_components'};
  my(@tags)   = (defined $passed ? $passed : keys %$vc);
  my($pchh)   = $self->get_assignment('pch_header');
  my($pchc)   = $self->get_assignment('pch_source');

  foreach my $tag (@tags) {
    my($exts) = $$vc{$tag};
    if (defined $$exts[0]) {
      if (defined $self->{$tag}) {
        ## If the tag is defined, then process directories
        my($names) = $self->{$tag};
        foreach my $name (keys %$names) {
          my($comps) = $$names{$name};
          foreach my $comp (keys %$comps) {
            my($array) = $$comps{$comp};
            if (defined $passed) {
              $self->sift_files($files, $exts, $pchh, $pchc, $tag, $array);
            }
            else {
              my(@built) = ();
              foreach my $file (@$array) {
                if (-d $file) {
                  my(@gen) = $self->generate_default_file_list($file);
                  $self->sift_files(\@gen, $exts, $pchh, $pchc, $tag, \@built);
                }
                else {
                  if (!$self->already_added(\@built, $file)) {
                    push(@built, $file);
                  }
                }
              }
              $$comps{$comp} = \@built;
            }
          }
        }
      }
      else {
        ## Generate default values for undefined tags
        my($names) = {};
        $self->{$tag} = $names;
        my($comps) = {};
        $$names{'default'} = $comps;
        $$comps{'000_FILES'} = [];
        my($array) = $$comps{'000_FILES'};

        if (!$self->is_special_tag($tag)) {
          $self->sift_files($files, $exts, $pchh, $pchc, $tag, $array);
          if (defined $self->{'generated_exts'}->{$tag}) {
            if (defined $$array[0]) {
              $self->{'defaulted'}->{$tag} = 1;
            }
          }
          elsif ($tag eq 'source_files') {
            foreach my $gentype (keys %{$self->{'generated_exts'}}) {
              ## If we are auto-generating the source_files, then
              ## we need to make sure that any generated source
              ## files that are added are put at the front of the list.
              my(@front) = ();
              my(@copy)  = @$array;
              my(@exts)  = $self->generated_extensions($gentype, $tag);

              $self->{'defaulted'}->{$tag} = 1;
              @$array = ();
              foreach my $file (@copy) {
                my($found) = 0;
                foreach my $ext (@exts) {
                  if ($file =~ /$ext$/) {
                    ## No need to check for previously added files
                    ## here since there are none.
                    push(@front, $file);
                    $found = 1;
                    last;
                  }
                }
                if (!$found) {
                  ## No need to check for previously added files
                  ## here since there are none.
                  push(@$array, $file);
                }
              }

              if (defined $front[0]) {
                unshift(@$array, @front);
              }
            }
          }
        }
      }
    }
  }
}


sub remove_duplicated_files {
  my($self)   = shift;
  my($dest)   = shift;
  my($source) = shift;
  my($names)  = $self->{$dest};
  my(@slist)  = $self->get_component_list($source);
  my(%shash)  = ();

  ## Convert the array into keys for a hash table
  @shash{@slist} = ();

  ## Find out which source files are listed
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my($array) = $$comps{$key};
      my($count) = scalar(@$array);
      for(my $i = 0; $i < $count; ++$i) {
        ## Is the source file in the component array?
        if (exists $shash{$$array[$i]}) {
          ## Remove the element and fix the index and count
          splice(@$array, $i, 1);
          --$count;
          --$i;
        }
      }
    }
  }
}


sub generated_extensions {
  my($self) = shift;
  my($name) = shift;
  my($tag)  = shift;
  my(@exts) = ();
  my($gen)  = $self->{'generated_exts'}->{$name};

  if (defined $gen->{$tag}) {
    foreach my $pe (@{$gen->{'pre_extension'}}) {
      foreach my $ext (@{$gen->{$tag}}) {
        push(@exts, "$pe$ext");
      }
    }
  }
  return @exts;
}


sub generated_source_listed {
  my($self)  = shift;
  my($gent)  = shift;
  my($tag)   = shift;
  my($arr)   = shift;
  my($names) = $self->{$tag};
  my(@gen)   = $self->generated_extensions($gent, $tag);
  my(@found) = ();

  ## Find out which generated source files are listed
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my($array) = $$comps{$key};
      foreach my $val (@$array) {
        foreach my $ext (@gen) {
          foreach my $i (@$arr) {
            my($ifile) = $self->escape_regex_special($i);
            if ($val =~ /$ifile$ext$/) {
              push(@found, $val);
            }
          }
        }
      }
    }
  }
  return (defined $found[0]);
}


sub list_default_generated {
  my($self)    = shift;
  my($gentype) = shift;
  my($tags)    = shift;

  if ($self->{'defaulted'}->{$gentype} &&
      $self->{'generated_exts'}->{$gentype}->{'automatic'}) {
    ## After all source and headers have been defaulted, see if we
    ## need to add the generated .h, .i and .cpp files
    if (defined $self->{$gentype}) {
      ## Build up the list of files
      my(@arr)    = ();
      my($wanted) = $self->{'valid_components'}->{$gentype}->[0];
      my($names)  = $self->{$gentype};
      foreach my $name (keys %$names) {
        my($comps) = $$names{$name};
        foreach my $key (keys %$comps) {
          my($array) = $$comps{$key};
          foreach my $val (@$array) {
            my($f) = $val;
            $f =~ s/$wanted$//;
            push(@arr, $f);
          }
        }
      }

      foreach my $type (@$tags) {
        if (!$self->generated_source_listed($gentype, $type, \@arr)) {
          $self->add_generated_files($gentype, $type, \@arr);
        }
      }
    }
  }
}


sub list_generated_file {
  my($self)    = shift;
  my($gentype) = shift;
  my($tag)     = shift;
  my($array)   = shift;
  my($file)    = shift;

  if (defined $self->{'generated_exts'}->{$gentype}->{$tag}) {
    my(@gen)     = $self->get_component_list($gentype);
    my(@genexts) = $self->generated_extensions($gentype, $tag);
    foreach my $gen (@gen) {
      ## If we are converting slashes, then we need to
      ## convert the component back to forward slashes
      if ($self->{'convert_slashes'}) {
        $gen =~ s/\\/\//g;
      }

      ## Remove the extension
      my($start) = $gen;
      foreach my $ext (@{$self->{'valid_components'}->{$gentype}}) {
        $gen =~ s/$ext$//;
        if ($gen ne $start) {
          last;
        }
      }

      ## See if we need to add the file
      foreach my $pf (@{$self->{'generated_exts'}->{$gentype}->{'pre_filename'}}) {
        foreach my $genext (@genexts) {
          if ("$pf$gen$genext" =~ /$file(.*)?$/) {
            my($created) = "$file$1";
            $created =~ s/\\//g;
            if (!$self->already_added($array, $created)) {
              push(@$array, $created);
            }
            last;
          }
        }
      }
    }
  }
}


sub add_corresponding_component_files {
  my($self)  = shift;
  my($ftags) = shift;
  my($tag)   = shift;
  my(@all)   = ();

  foreach my $filetag (@$ftags) {
    my($names) = $self->{$filetag};
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $comp (keys %$comps) {
        foreach my $sfile (@{$$comps{$comp}}) {
          my($scopy) = $sfile;
          $scopy =~ s/\.[^\.]+$//;
          push(@all, $scopy);
        }
      }
    }
  }

  my(@exts)  = ();
  my($names) = $self->{$tag};

  foreach my $ext (@{$self->{'valid_components'}->{$tag}}) {
    my($ecpy) = $ext;
    $ecpy =~ s/\\//g;
    push(@exts, $ecpy);
  }

  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $comp (keys %$comps) {
      my($array) = $$comps{$comp};
      foreach my $sfile (@all) {
        my($found)   = 0;
        my(%scfiles) = ();
        foreach my $ext (@exts) {
          $scfiles{"$sfile$ext"} = 1;
        }
        foreach my $file (@$array) {
          if (defined $scfiles{$file}) {
            $found = 1;
            last;
          }
        }

        if (!$found) {
          foreach my $ext (@exts) {
            my($built) = "$sfile$ext";
            if (-r $built) {
               push(@$array, $built);
               $found = 1;
               last;
            }
          }

          if (!$found) {
            foreach my $gentype (keys %{$self->{'generated_exts'}}) {
              $self->list_generated_file($gentype, $tag, $array, $sfile);
            }
          }
        }
      }
    }
  }
}


sub get_default_project_name {
  my($self) = shift;
  my($name) = $self->get_current_input();

  if ($name eq '') {
    $name = $self->transform_file_name($self->base_directory());
  }
  else {
    ## Since files on UNIX can have back slashes, we transform them
    ## into underscores.
    $name =~ s/\\/_/g;

    ## Convert then name to a usable name
    $name = $self->transform_file_name($name);

    ## Take off the extension
    $name =~ s/\.[^\.]+$//;
  }

  return $name;
}


sub generate_defaults {
  my($self) = shift;

  ## Generate default project name
  if (!defined $self->get_assignment('project_name')) {
    $self->process_assignment('project_name',
                              $self->get_default_project_name());
  }

  ## Generate the default pch file names (if needed)
  my(@files) = $self->generate_default_file_list();
  $self->generate_default_pch_filenames(\@files);

  ## If the pch file names are empty strings then we need to fix that
  $self->fix_pch_filenames();

  ## Generate default components, but @specialComponents
  ## are skipped in the initial default components generation
  $self->generate_default_components(\@files);

  ## Remove source files that are also listed in the template files
  ## If we do not do this, then generated projects can be invalid.
  $self->remove_duplicated_files('source_files', 'template_files');

  ## If pch files are listed in header_files or source_files more than
  ## once, we need to remove the extras
  $self->remove_extra_pch_listings();

  ## Generate the default generated list of source files
  ## only if we defaulted the generated file list
  foreach my $gentype (keys %{$self->{'generated_exts'}}) {
    $self->list_default_generated($gentype, ['source_files']);
  }

  ## Add @specialComponents files based on the
  ## source_components (i.e. .h and .i or .inl based on .cpp)
  foreach my $tag (@specialComponents) {
    $self->add_corresponding_component_files(['source_files',
                                              'template_files'], $tag);
  }

  ## Now, if the @specialComponents are still empty
  ## then take any file that matches the components extension
  foreach my $tag (@specialComponents) {
    if (!$self->{'special_supplied'}->{$tag}) {
      my($names) = $self->{$tag};
      if (defined $names) {
        foreach my $name (keys %$names) {
          my($comps) = $$names{$name};
          foreach my $comp (keys %$comps) {
            my($array) = $$comps{$comp};
            if (!defined $$array[0] ||
                $self->{'defaulted'}->{'source_files'}) {
              $self->generate_default_components(\@files, $tag);
            }
          }
        }
      }
    }
  }

  ## Generate default target names after all source files are added
  $self->generate_default_target_names();
}


sub project_name {
  my($self) = shift;
  return $self->get_assignment('project_name');
}


sub lib_target {
  my($self) = shift;
  return (defined $self->get_assignment('sharedname') ||
          defined $self->get_assignment('staticname'));
}


sub exe_target {
  my($self) = shift;
  return (defined $self->get_assignment('exename'));
}


sub get_component_list {
  my($self)  = shift;
  my($tag)   = shift;
  my($names) = $self->{$tag};
  my(@list)  = ();

  foreach my $name (keys %$names) {
    my($comps)  = $$names{$name};
    foreach my $key (sort keys %$comps) {
      my($array)  = $$comps{$key};
      push(@list, @$array);
    }
  }

  if ($self->{'convert_slashes'}) {
    for(my $i = 0; $i <= $#list; $i++) {
      $list[$i] = $self->slash_to_backslash($list[$i]);
    }
  }

  if ($self->{'sort_files'}) {
    @list = sort { $self->file_sorter($a, $b) } @list;
  }

  return @list;
}


sub check_custom_output {
  my($self)    = shift;
  my($based)   = shift;
  my($pf)      = shift;
  my($cinput)  = shift;
  my($type)    = shift;
  my($comps)   = shift;
  my(@outputs) = ();
  my($gen)     = $self->{'generated_exts'}->{$based};

  if (defined $gen->{$type}) {
    foreach my $pe (@{$gen->{'pre_extension'}}) {
      foreach my $ext (@{$gen->{$type}}) {
        my($ge) = "$pe$ext";
        $ge =~ s/\\//g;
        my($built) = "$pf$cinput$ge";
        if (@$comps == 0) {
          push(@outputs, $built);
          last;
        }
        else {
          my($base) = $built;
          if ($self->{'convert_slashes'}) {
            $base =~ s/\\/\//g;
          }
          my($re) = $self->escape_regex_special(basename($base));
          foreach my $c (@$comps) {
            ## We only match if the built file name matches from
            ## beginning to end or from a slash to the end.
            if ($c =~ /^$re$/ || $c =~ /[\/\\]$re$/) {
              push(@outputs, $built);
              last;
            }
          }
        }
      }
    }
  }

  return @outputs;
}


sub get_custom_value {
  my($self)  = shift;
  my($cmd)   = shift;
  my($based) = shift;
  my($value) = undef;

  if ($cmd eq 'input_files') {
    my($generic) = 'generic_files';  ## Matches with generic_outputext
    my(@array) = $self->get_component_list($based);
    $value = \@array;

    $self->{'custom_output_files'} = {};
    my(%vcomps) = ();
    foreach my $vc (keys %{$self->{'valid_components'}}, $generic) {
      my(@comps) = $self->get_component_list($vc);
      $vcomps{$vc} = \@comps;
    }
    foreach my $input (@array) {
      my(@outputs) = ();
      my($cinput)  = $input;
      $cinput =~ s/\.[^\.]+$//;
      foreach my $pf (@{$self->{'generated_exts'}->{$based}->{'pre_filename'}}) {
        foreach my $vc (keys %{$self->{'valid_components'}}, $generic) {
          push(@outputs,
               $self->check_custom_output($based, $pf,
                                          $cinput, $vc, $vcomps{$vc}));
        }
      }
      $self->{'custom_output_files'}->{$input} = \@outputs;
    }
  }
  elsif ($cmd eq 'output_files') {
    # Generate output files based on $based
    if (defined $self->{'custom_output_files'}) {
      $value = $self->{'custom_output_files'}->{$based};
    }
  }
  elsif ($cmd eq 'inputexts') {
    my(@array) = @{$self->{'valid_components'}->{$based}};
    foreach my $val (@array) {
      $val =~ s/\\\.//g;
    }
    $value = \@array;
  }
  elsif ($cmd eq 'command'       || $cmd eq 'pch_option' ||
         $cmd eq 'output_option' || defined $custom{$cmd}) {
    $value = $self->get_assignment($cmd,
                                   $self->{'generated_exts'}->{$based});
  }

  return $value;
}


sub check_features {
  my($self)     = shift;
  my($requires) = $self->get_assignment('requires');
  my($status)   = 1;

  if (defined $requires) {
    foreach my $require (split(/\s+/, $requires)) {
      my($fval) = $self->{'feature_parser'}->get_value($require);

      ## By default, if the feature is not listed, then it is enabled.
      if (defined $fval && !$fval) {
        $status = 0;
        last;
      }
    }
  }

  ## If it passes the requires, then check the avoids
  if ($status) {
    my($avoids) = $self->get_assignment('avoids');
    if (defined $avoids) {
      foreach my $avoid (split(/\s+/, $avoids)) {
        my($fval) = $self->{'feature_parser'}->get_value($avoid);

        ## By default, if the feature is not listed, then it is enabled.
        if (!defined $fval || $fval) {
          $status = 0;
          last;
        }
      }
    }
  }

  return $status;
}


sub need_to_write_project {
  my($self) = shift;

  if ($self->check_features()) {
    foreach my $key ('source_files', keys %{$self->{'generated_exts'}}) {
      my($names) = $self->{$key};
      foreach my $name (keys %$names) {
        foreach my $key (keys %{$names->{$name}}) {
          if (defined $names->{$name}->{$key}->[0]) {
            return 1;
          }
        }
      }
    }
  }

  return 0;
}


sub write_output_file {
  my($self)     = shift;
  my($name)     = shift;
  my($status)   = 0;
  my($error)    = '';
  my($tover)    = $self->get_template_override();
  my($template) = (defined $tover ? $tover : $self->get_template()) .
                  ".$TemplateExtension";
  my($tfile)    = $self->search_include_path($template);

  if (defined $tfile) {
    ## Read in the template values for the
    ## specific target and project type
    ($status, $error) = $self->read_template_input();

    if ($status) {
      my($tp) = new TemplateParser($self);

      ## Set the project_file assignment for the template parser
      $self->process_assignment('project_file', $name);

      ($status, $error) = $tp->parse_file($tfile);

      if ($status) {
        if ($self->get_toplevel()) {
          my($fh)  = new FileHandle();
          my($dir) = dirname($name);

          if ($dir ne '.') {
            mkpath($dir, 0, 0777);
          }

          ## First write the output to a temporary file
          my($tmp) = "MPC$>.$$";
          my($different) = 1;
          if (open($fh, ">$tmp")) {
            my($lines) = $tp->get_lines();
            foreach my $line (@$lines) {
              print $fh $line;
            }
            close($fh);
            
            if (-r $name &&
                -s $tmp == -s $name && compare($tmp, $name) == 0) {
              $different = 0;
            }
          }
          else {
            $error = "ERROR: Unable to open $tmp for output.";
            $status = 0;
          }

          if ($status) {
            ## If they are different, then rename the temporary file
            if ($different) {
              unlink($name);
              if (rename($tmp, $name)) {
                $self->add_file_written($name);
              }
              else {
                $error = "ERROR: Unable to open $name for output.";
                $status = 0;
              }
            }
            else {
              ## We will pretend that we wrote the file
              unlink($tmp);
              $self->add_file_written($name);
            }
          }
        }
      }
    }
  }
  else {
    $error = "ERROR: Unable to locate the template file: $template.";
    $status = 0;
  }

  return $status, $error;
}


sub write_project {
  my($self)     = shift;
  my($status)   = 1;
  my($error)    = '';
  my($name)     = $self->transform_file_name($self->project_file_name());
  my($progress) = $self->get_progress_callback();

  if (defined $progress) {
    &$progress();
  }

  if ($self->need_to_write_project()) {
    ## Writing the non-static file so set it to 0
    if ($self->get_dynamic()) {
      $self->{'writing_type'} = 0;
      ($status, $error) = $self->write_output_file($name);
    }

    if ($status && $self->get_static() && $self->separate_static_project()) {
      $name = $self->transform_file_name($self->static_project_file_name());

      ## Writing the static file so set it to 1
      $self->{'writing_type'} = 1;
      ($status, $error) = $self->write_output_file($name);
    }
  }

  return $status, $error;
}


sub get_project_info {
  my($self) = shift;
  return $self->{'project_info'};
}


sub get_writing_type {
  my($self) = shift;
  return $self->{'writing_type'};
}


sub set_component_extensions {
  my($self) = shift;
  my($vc)   = $self->{'valid_components'};
  my($ec)   = $self->{'exclude_components'};

  foreach my $key (keys %$vc) {
    my($ov) = $self->override_valid_component_extensions($key);
    if (defined $ov) {
      $$vc{$key} = $ov;
    }
  }

  foreach my $key (keys %$ec) {
    my($ov) = $self->override_exclude_component_extensions($key);
    if (defined $ov) {
      $$ec{$key} = $ov;
    }
  }
}


sub reset_values {
  my($self) = shift;
  $self->{'project_info'} = [];
}


sub reset_generating_types {
  my($self)  = shift;
  my(%reset) = ('matching_assignments' => \%ma,
                'valid_components'     => \%vc,
                'generated_exts'       => \%genext,
                'exclude_components'   => \%ec,
               );

  foreach my $r (keys %reset) {
    $self->{$r} = {};
    foreach my $key (keys %{$reset{$r}}) {
      $self->{$r}->{$key} = $reset{$r}->{$key};
    }
  }

  $self->{'custom_types'} = {};

  ## Allow subclasses to override the default extensions
  $self->set_component_extensions();
}


sub get_template_input {
  my($self) = shift;

  ## This follows along the same logic as read_template_input() by
  ## checking for exe target and then defaulting to a lib target
  if ($self->exe_target()) {
    if ($self->{'writing_type'} == 1) {
      return $self->{'lexe_template_input'};
    }
    else {
      return $self->{'dexe_template_input'};
    }
  }

  if ($self->{'writing_type'} == 1) {
    return $self->{'lib_template_input'};
  }
  else {
    return $self->{'dll_template_input'};
  }
}


sub update_project_info {
  my($self)    = shift;
  my($tparser) = shift;
  my($append)  = shift;
  my($names)   = shift;
  my($sep)     = shift;
  my($pi)      = $self->get_project_info();
  my($value)   = '';
  my($arr)     = ($append && defined $$pi[0] ? pop(@$pi) : []);

  ## Set up the hash table when we are starting a new project_info
  if ($append == 0) {
    $self->{'project_info_hash_table'} = {};
  }

  ## Append the values of all names into one string
  my(@narr) = @$names;
  for(my $i = 0; $i <= $#narr; $i++) {
    my($key) = $narr[$i];
    $value .= $self->translate_value($key,
                                     $tparser->get_value_with_default($key)) .
              (defined $sep && $i != $#narr ? $sep : '');
  }

  ## If we haven't seen this value yet, put it on the array
  if (!defined $self->{'project_info_hash_table'}->{"@narr $value"}) {
    $self->{'project_info_hash_table'}->{"@narr $value"} = 1;
    $self->save_project_value("@narr", $value);
    push(@$arr, $value);
  }

  ## Always push the array back onto the project_info
  push(@$pi, $arr);

  return $value;
}


sub get_verbatim {
  my($self)   = shift;
  my($marker) = shift;
  my($str)    = undef;
  my($thash)  = $self->{'verbatim'}->{$self->{'pctype'}};

  if (defined $thash) {
    if (defined $thash->{$marker}) {
      my($crlf) = $self->crlf();
      foreach my $line (@{$thash->{$marker}}) {
        if (!defined $str) {
          $str = '';
        }
        $str .= $self->process_special($line) . $crlf;
      }
      if (defined $str) {
        $str .= $crlf;
      }
    }
  }
  return $str;
}


sub generate_recursive_input_list {
  my($self) = shift;
  my($dir)  = shift;
  return $self->extension_recursive_input_list($dir,
                                               $ProjectCreatorExtension);
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub specific_lookup {
  #my($self) = shift;
  #my($key)  = shift;
  return undef;
}


sub save_project_value {
  #my($self)  = shift;
  #my($name)  = shift;
  #my($value) = shift;
}


sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;

  if ($key eq 'depends' && $val ne '') {
    my($arr) = $self->create_array($val);
    $val = '';
    foreach my $entry (@$arr) {
      $val .= '"' . $self->project_file_name($entry) . '" ';
    }
    $val =~ s/\s+$//;
  }
  return $val;
}


sub fill_value {
  #my($self) = shift;
  #my($name) = shift;
  return undef;
}


sub separate_static_project {
  #my($self) = shift;
  return 0;
}


sub project_file_name {
  #my($self) = shift;
  return undef;
}


sub static_project_file_name {
  #my($self) = shift;
  return undef;
}


sub override_valid_component_extensions {
  #my($self) = shift;
  #my($comp) = shift;
  return undef;
}


sub override_exclude_component_extensions {
  #my($self) = shift;
  #my($comp) = shift;
  return undef;
}


sub get_env_accessor {
  #my($self) = shift;
  return ();
}

sub get_dll_exe_template_input_file {
  #my($self) = shift;
  return undef;
}


sub get_lib_exe_template_input_file {
  #my($self) = shift;
  return undef;
}


sub get_lib_template_input_file {
  #my($self) = shift;
  return undef;
}


sub get_dll_template_input_file {
  #my($self) = shift;
  return undef;
}


sub get_template {
  #my($self) = shift;
  return undef;
}


1;
