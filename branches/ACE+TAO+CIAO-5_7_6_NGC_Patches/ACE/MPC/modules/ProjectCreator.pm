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

use Creator;
use TemplateInputReader;
use TemplateParser;
use FeatureParser;
use CommandHelper;

use vars qw(@ISA);
@ISA = qw(Creator);

# ************************************************************
# Data Section
# ************************************************************

## The basic extensions known to a project creator
my $BaseClassExtension      = 'mpb';
my $ProjectCreatorExtension = 'mpc';
my $TemplateExtension       = 'mpd';
my $TemplateInputExtension  = 'mpt';

## This feature is enabled or disabled depending on whether
## or not the -static option is used.
my $static_libs_feature = 'static_libs_only';

## Valid names for assignments within a project
## Bit Meaning
## 0   Preserve the order for additions (1) or invert it (0)
## 1   Add this value to template input value (if there is one)
## 2   Preserve <% %> settings for evaluation within the template
my %validNames = ('after'              => 1,
                  'avoids'             => 3,
                  'custom_only'        => 1,
                  'dllout'             => 1,
                  'dynamicflags'       => 3,
                  'exename'            => 1,
                  'exeout'             => 1,
                  'includes'           => 3,
                  'libout'             => 1,
                  'libpaths'           => 3,
                  'libs'               => 2,
                  'lit_libs'           => 2,
                  'macros'             => 3,
                  'managed'            => 1,
                  'pch_header'         => 1,
                  'pch_source'         => 1,
                  'postbuild'          => 5,
                  'postclean'          => 5,
                  'prebuild'           => 5,
                  'pure_libs'          => 2,
                  'recurse'            => 1,
                  'recursive_includes' => 3,
                  'recursive_libpaths' => 3,
                  'requires'           => 3,
                  'sharedname'         => 1,
                  'staticflags'        => 3,
                  'staticname'         => 1,
                  'tagchecks'          => 1,
                  'tagname'            => 1,
                  'version'            => 1,
                  'webapp'             => 1,
                 );

## Custom definitions only
## Bit  Meaning
## 0    Value is always an array
## 1    Value is an array and name gets 'outputext' converted to 'files'
## 2    Value is always scalar
## 3    Name can also be used in an 'optional' clause
## 4    Needs <%...%> conversion
my %customDefined = ('automatic_in'                => 0x04,
                     'automatic_out'               => 0x04,
                     'command'                     => 0x14,
                     'commandflags'                => 0x14,
                     'dependent'                   => 0x14,
                     'precommand'                  => 0x14,
                     'postcommand'                 => 0x14,
                     'inputext'                    => 0x01,
                     'libpath'                     => 0x04,
                     'output_follows_input'        => 0x04,
                     'output_option'               => 0x14,
                     'pch_postrule'                => 0x04,
                     'pre_extension'               => 0x08,
                     'source_pre_extension'        => 0x08,
                     'template_pre_extension'      => 0x08,
                     'header_pre_extension'        => 0x08,
                     'inline_pre_extension'        => 0x08,
                     'documentation_pre_extension' => 0x08,
                     'resource_pre_extension'      => 0x08,
                     'generic_pre_extension'       => 0x08,
                     'pre_filename'                => 0x08,
                     'source_pre_filename'         => 0x08,
                     'template_pre_filename'       => 0x08,
                     'header_pre_filename'         => 0x08,
                     'inline_pre_filename'         => 0x08,
                     'documentation_pre_filename'  => 0x08,
                     'resource_pre_filename'       => 0x08,
                     'generic_pre_filename'        => 0x08,
                     'pre_dirname'                 => 0x08,
                     'source_pre_dirname'          => 0x08,
                     'template_pre_dirname'        => 0x08,
                     'header_pre_dirname'          => 0x08,
                     'inline_pre_dirname'          => 0x08,
                     'documentation_pre_dirname'   => 0x08,
                     'resource_pre_dirname'        => 0x08,
                     'generic_pre_dirname'         => 0x08,
                     'source_outputext'            => 0x0a,
                     'template_outputext'          => 0x0a,
                     'header_outputext'            => 0x0a,
                     'inline_outputext'            => 0x0a,
                     'documentation_outputext'     => 0x0a,
                     'resource_outputext'          => 0x0a,
                     'generic_outputext'           => 0x0a,
                    );

## Custom sections as well as definitions
## Value  Meaning
## 0    No modifications
## 1    Needs <%...%> conversion
my %custom = ('command'       => 1,
              'commandflags'  => 1,
              'dependent'     => 1,
              'gendir'        => 0,
              'precommand'    => 1,
              'postcommand'   => 1,
             );

## All matching assignment arrays will get these keywords
my @default_matching_assignments = ('recurse',
                                   );

## Deal with these components in a special way
my %specialComponents = ('header_files'   => 1,
                         'inline_files'   => 1,
                         'template_files' => 1,
                        );
my %sourceComponents  = ('source_files'   => 1,
                         'template_files' => 1,
                        );

my $defgroup    = 'default_group';
my $grouped_key = 'grouped_';
my $tikey       = '/ti/';

## Matches with generic_outputext
my $generic_key = 'generic_files';

# ************************************************************
# C++ Specific Component Settings
# ************************************************************

## Resource files tag for C++
my $cppresource = 'resource_files';

## Valid component names within a project along with the valid file extensions
my %cppvc = ('source_files'        => [ "\\.cpp", "\\.cxx", "\\.cc", "\\.c", "\\.C", ],
             'template_files'      => [ "_T\\.cpp", "_T\\.cxx", "_T\\.cc", "_T\\.c", "_T\\.C", "_t\\.cpp", "_t\\.cxx", "_t\\.cc", "_t\\.c", "_t\\.C" ],
             'header_files'        => [ "\\.h", "\\.hpp", "\\.hxx", "\\.hh", ],
             'inline_files'        => [ "\\.i", "\\.ipp", "\\.inl", ],
             'documentation_files' => [ "README", "readme", "\\.doc", "\\.txt", "\\.html" ],
             $cppresource          => [ "\\.rc", ],
            );

## Exclude these extensions when auto generating the component values
my %cppec = ('source_files' => $cppvc{'template_files'},
            );

## These matching assignment arrays will get added, but only to the
## specific project component types.
my %cppma = ('source_files' => ['buildflags',
                                'managed',
                                'no_pch',
                               ],
            );

# ************************************************************
# C# Specific Component Settings
# ************************************************************

## Resource files tag for C#
my $csresource = 'resx_files';

## Valid component names within a project along with the valid file extensions
my %csvc = ('source_files'        => [ "\\.cs" ],
            'config_files'        => [ "\\.config" ],
            $csresource           => [ "\\.resx", "\\.resources" ],
            'aspx_files'          => [ "\\.aspx" ],
            'ico_files'           => [ "\\.ico" ],
            'documentation_files' => [ "README", "readme", "\\.doc", "\\.txt", "\\.html" ],
           );

my %csma = ('source_files' => [ 'dependent_upon',
                                'subtype',
                              ],
            $csresource    => [ 'dependent_upon',
                                'generates_source',
                                'subtype',
                              ],
           );

# ************************************************************
# Java Specific Component Settings
# ************************************************************

## Valid component names within a project along with the valid file extensions
my %jvc = ('source_files'        => [ "\\.java" ],
           'documentation_files' => [ "README", "readme", "\\.doc", "\\.txt", "\\.html" ],
          );

# ************************************************************
# Visual Basic Specific Component Settings
# ************************************************************

## Resource files tag for VB
my $vbresource = 'resx_files';

## Valid component names within a project along with the valid file extensions
my %vbvc = ('source_files'        => [ "\\.vb" ],
            'config_files'        => [ "\\.config" ],
            $vbresource           => [ "\\.resx" ],
            'aspx_files'          => [ "\\.aspx" ],
            'ico_files'           => [ "\\.ico" ],
            'documentation_files' => [ "README", "readme", "\\.doc", "\\.txt", "\\.html" ],
           );

my %vbma = ('source_files' => [ 'subtype' ],
           );

# ************************************************************
# Language Specific Component Settings
# ************************************************************

# Index Description
# ----- -----------
# 0     File types
# 1     Files automatically excluded from source_files
# 2     Assignments available in standard file types
# 3     The entry point for executables
# 4     The language uses a C preprocessor
# 5     Name of the tag for 'resource_files' for this language
#        * This is special because it gets treated like source_files in that
#          a project with only these files is a library/exe not "custom only".
my %language = (Creator::cplusplus => [ \%cppvc, \%cppec, \%cppma, 'main',
                                        1, $cppresource ],

                Creator::csharp    => [ \%csvc, {}, \%csma, 'Main', 0,
                                        $csresource ],

                Creator::java      => [ \%jvc, {}, {}, 'main', 0 ],

                Creator::vb        => [ \%vbvc, {}, \%vbma, 'Main', 0,
                                        $vbresource ],
               );
my %mains;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $global, $inc, $template, $ti, $dynamic, $static, $relative, $addtemp, $addproj, $progress, $toplevel, $baseprojs, $gfeature, $relative_f, $feature, $features, $hierarchy, $exclude, $makeco, $nmod, $applypj, $genins, $into, $language, $use_env, $expandvars, $gendot, $comments, $foreclipse) = @_;
  my $self = $class->SUPER::new($global, $inc,
                                $template, $ti, $dynamic, $static,
                                $relative, $addtemp, $addproj,
                                $progress, $toplevel, $baseprojs,
                                $feature, $features,
                                $hierarchy, $nmod, $applypj,
                                $into, $language, $use_env,
                                $expandvars,
                                'project');

  $self->{$self->{'type_check'}}   = 0;
  $self->{'feature_defined'}       = 0;
  $self->{'features_changed'}      = undef;
  $self->{'project_info'}          = [];
  $self->{'lib_locations'}         = {};
  $self->{'reading_parent'}        = [];
  $self->{'dll_exe_template_input'}= {};
  $self->{'lib_exe_template_input'}= {};
  $self->{'lib_template_input'}    = {};
  $self->{'dll_template_input'}    = {};
  $self->{'flag_overrides'}        = {};
  $self->{'custom_special_output'} = {};
  $self->{'custom_special_depend'} = {};
  $self->{'special_supplied'}      = {};
  $self->{'pctype'}                = $self->extractType("$self");
  $self->{'verbatim'}              = {};
  $self->{'verbatim_accessed'}     = {$self->{'pctype'} => {}};
  $self->{'defaulted'}             = {};
  $self->{'custom_types'}          = {};
  $self->{'parents_read'}          = {};
  $self->{'inheritance_tree'}      = {};
  $self->{'remove_files'}          = {};
  $self->{'expanded'}              = {};
  $self->{'gfeature_file'}         = $gfeature;
  $self->{'relative_file'}         = $relative_f;
  $self->{'feature_parser'}        = $self->create_feature_parser($features,
                                                                  $feature);
  $self->{'sort_files'}            = $self->sort_files();
  $self->{'source_callback'}       = undef;
  $self->{'dollar_special'}        = $self->dollar_special();
  $self->{'generate_ins'}          = $genins;
  $self->{'addtemp_state'}         = undef;
  $self->{'command_subs'}          = $self->get_command_subs();
  $self->{'escape_spaces'}         = $self->escape_spaces();
  $self->{'current_template'}      = undef;
  $self->{'make_coexistence'}      = $makeco;

  $self->add_default_matching_assignments();
  $self->reset_generating_types();

  return $self;
}


sub is_keyword {
  ## Is the name passed in a known keyword for a project.  This includes
  ## keywords mapped by Define_Custom or Modify_Custom.
  my($self, $name) = @_;
  return $self->{'valid_names'}->{$name};
}


sub read_global_configuration {
  my $self   = shift;
  my $input  = $self->get_global_cfg();
  my $status = 1;

  if (defined $input) {
    ## If it doesn't contain a path, search the include path
    if ($input !~ /[\/\\]/) {
      $input = $self->search_include_path($input);
      $input = $self->get_global_cfg() if (!defined $input);
    }

    ## Read and parse the global project file
    $self->{'reading_global'} = 1;
    $status = $self->parse_file($input);
    $self->{'reading_global'} = 0;
  }

  return $status;
}


sub convert_to_template_assignment {
  my($self, $name, $value, $calledfrom) = @_;

  ## If the value we are going to set for $name has been used as a
  ## scoped template variable, we need to hijack the whole assignment
  ## and turn it into a template variable assignment.
  my $atemp = $self->get_addtemp();
  foreach my $key (grep(/::$name$/, keys %$atemp)) {
    $self->update_template_variable(0, $calledfrom, $key, $value);
  }
}


sub create_recursive_settings {
  my($self, $name, $value, $assign) = @_;

  ## Handle both recursive_includes and recursive_libpaths in one
  ## search and replace.
  if ($name =~ s/^recursive_//) {
    ## This portion of code was lifted directly from Creator::relative()
    ## but modified to always expand the variables.  We will turn the
    ## expanded values back into variables below and once they're passed
    ## off to the assignment processing code, they will be turned into
    ## relative values (if possible).
    if (index($value, '$') >= 0) {
      my $ovalue = $value;
      my($rel, $how) = $self->get_initial_relative_values();
      $value = $self->expand_variables($value, $rel, 0, undef, 1);

      if ($ovalue eq $value || index($value, '$') >= 0) {
        ($rel, $how) = $self->get_secondary_relative_values();
        $value = $self->expand_variables($value, $rel, 0, undef, 1, 1);
      }
    }

    ## Create an array out of the recursive directory list.  Convert all
    ## of the relative or full path values back into $() values.
    my @dirs = ();
    my $elems = $self->create_array($value);
    foreach my $elem (@$elems) {
      my $dlist = $self->recursive_directory_list($elem, []);
      if ($dlist eq '') {
        ## This directory doesn't exist, just add the original value
        push(@dirs, $elem);
      }
      else {
        ## Create an array out of the directory list and add it to our
        ## array.
        my $array = $self->create_array($dlist);
        push(@dirs, @$array);
      }
    }

    ## We need to return a string, so we join it all together space
    ## separated.
    $value = join(' ', $self->back_to_variable(\@dirs));
  }

  return $name, $value;
}

sub process_assignment {
  my($self, $name, $value, $assign, $calledfrom) = @_;
  $calledfrom = 0 if (!defined $calledfrom);

  ## See if the name is one of the special "recursive" settings.  If so,
  ## fix up the value and change the name.
  ($name, $value) = $self->create_recursive_settings($name, $value, $assign);

  ## Support the '*' mechanism as in the project name, to allow
  ## the user to correctly depend on another project within the same
  ## directory.
  if (defined $value) {
    if ($name eq 'after' && index($value, '*') >= 0) {
      $value = $self->fill_type_name($value,
                                     $self->get_default_project_name());
    }

    ## If this particular project type does not consider the dollar sign
    ## special and the user has provided two dollarsigns as an escape, we
    ## will turn it into a single dollar sign.
    if (!$self->{'dollar_special'} && index($value, '$$') >= 0) {
      $value =~ s/\$\$/\$/g;
    }

    ## If the assignment name is valid and requires parameter (<%...%>)
    ## replacement, then do so.
    if (defined $self->{'valid_names'}->{$name} &&
        ($self->{'valid_names'}->{$name} & 0x04) == 0 &&
        index($value, '<%') >= 0) {
      $value = $self->replace_parameters($value, $self->{'command_subs'});
    }
  }

  if ($calledfrom == 0) {
    $self->convert_to_template_assignment($name, $value, $calledfrom);
  }

  ## Call the base process_assigment() after we have modified the name and
  ## value.
  $self->SUPER::process_assignment($name, $value, $assign);

  ## Support keyword mapping here only at the project level scope. The
  ## scoped keyword mapping is done through the parse_scoped_assignment()
  ## method.
  if (!defined $assign || $assign == $self->get_assignment_hash()) {
    my $mapped = $self->{'valid_names'}->{$name};
    if (defined $mapped && UNIVERSAL::isa($mapped, 'ARRAY')) {
      $self->parse_scoped_assignment($$mapped[0], 0,
                                     $$mapped[1], $value,
                                     $self->{'generated_exts'}->{$$mapped[0]});
    }
  }
}


sub process_assignment_add {
  my($self, $name, $value, $assign) = @_;

  ## See if the name is one of the special "recursive" settings.  If so,
  ## fix up the value and change the name.
  ($name, $value) = $self->create_recursive_settings($name, $value, $assign);

  return $self->SUPER::process_assignment_add($name, $value, $assign);
}


sub process_assignment_sub {
  my($self, $name, $value, $assign) = @_;

  ## See if the name is one of the special "recursive" settings.  If so,
  ## fix up the value and change the name.
  ($name, $value) = $self->create_recursive_settings($name, $value, $assign);

  return $self->SUPER::process_assignment_sub($name, $value, $assign);
}


sub addition_core {
  my($self, $name, $value, $nval, $assign) = @_;

  ## If there is a previous value ($nval) and the keyword is going to be
  ## evaled, we need to separate the values with a command separator.
  ## This has to be done at the MPC level because it isn't always
  ## possible for the user to know if a value has already been added to
  ## the keyword (prebuild, postbuild and postclean).
  if (defined $nval &&
      defined $validNames{$name} && ($validNames{$name} & 4)) {
    if ($self->preserve_assignment_order($name)) {
      $value = '<%cmdsep%> ' . $value;
    }
    else {
      $value .= '<%cmdsep%>';
    }
  }

  ## For an addition, we need to see if it is a project keyword being
  ## used within a 'specific' section.  If it is, we may need to update
  ## scoped settings for that variable (which are in essence template
  ## variables).
  $self->convert_to_template_assignment($name, $value, 1);

  ## Next, we just give everything to the base class method.
  $self->SUPER::addition_core($name, $value, $nval, $assign);
}


sub subtraction_core {
  my($self, $name, $value, $nval, $assign) = @_;

  ## For a subtraction, we need to see if it is a project keyword being
  ## used within a 'specific' section.  If it is, we may need to update
  ## scoped settings for that variable (which are in essence template
  ## variables).
  $self->convert_to_template_assignment($name, $value, -1);

  ## Next, we just give everything to the base class method.
  $self->SUPER::subtraction_core($name, $value, $nval, $assign);
}


sub get_assignment_for_modification {
  my($self, $name, $assign, $subtraction) = @_;

  ## If we weren't passed an assignment hash, then we need to
  ## look one up that may possibly correctly deal with keyword mappings
  if (!defined $assign) {
    my $mapped = $self->{'valid_names'}->{$name};

    if (defined $mapped && UNIVERSAL::isa($mapped, 'ARRAY')) {
      $name   = $$mapped[1];
      $assign = $self->{'generated_exts'}->{$$mapped[0]};
    }
  }

  ## Get the assignment value
  my $value = $self->get_assignment($name, $assign);

  ## If we are involved in a subtraction, we get back a value and
  ## it's a scoped or mapped assignment, then we need to possibly
  ## expand any template variables.  Otherwise, the subtractions
  ## may not work correctly.
  if ($subtraction && defined $value && defined $assign) {
    $value = $self->relative($value, 1);
  }

  return $value;
}


sub begin_project {
  my($self, $parents) = @_;
  my $status = 1;
  my $error;

  ## Deal with the inheritance hierarchy first
  ## Add in the base projects from the command line
  if (!$self->{'reading_global'} &&
      !defined $self->{'reading_parent'}->[0]) {
    my $baseprojs = $self->get_baseprojs();

    if (defined $parents) {
      foreach my $base (@$baseprojs) {
        push(@$parents, $base) if (!StringProcessor::fgrep($base, $parents));
      }
    }
    else {
      $parents = $baseprojs;
    }
  }

  if (defined $parents) {
    foreach my $parent (@$parents) {
      ## Read in the parent onto ourself
      my $file = $self->search_include_path(
                          "$parent.$BaseClassExtension");
      if (!defined $file) {
        $file = $self->search_include_path(
                             "$parent.$ProjectCreatorExtension");
      }

      if (defined $file) {
        if (defined $self->{'reading_parent'}->[0]) {
          if (StringProcessor::fgrep($file, $self->{'reading_parent'})) {
            $status = 0;
            $error = 'Cyclic inheritance detected: ' . $parent;
          }
        }

        if ($status) {
          if (!defined $self->{'parents_read'}->{$file}) {
            $self->{'parents_read'}->{$file} = 1;

            ## Push the base project file onto the parent stack
            push(@{$self->{'reading_parent'}}, $file);

            ## Collect up some information about the inheritance tree
            my $tree = $self->{'current_input'};
            if (!defined $self->{'inheritance_tree'}->{$tree}) {
              $self->{'inheritance_tree'}->{$tree} = {};
            }
            my $hash = $self->{'inheritance_tree'}->{$tree};
            foreach my $p (@{$self->{'reading_parent'}}) {
              $$hash{$p} = {} if (!defined $$hash{$p});
              $hash = $$hash{$p};
            }

            ## Begin reading the parent
            $status = $self->parse_file($file);

            ## Take the base project file off of the parent stack
            pop(@{$self->{'reading_parent'}});

            $error = "Invalid parent: $parent" if (!$status);
          }
          else {
            ## The base project has already been read.  So, if
            ## we are reading the original project (not a parent base
            ## project), then the current base project is redundant.
            if (!defined $self->{'reading_parent'}->[0]) {
              $file =~ s/\.[^\.]+$//;
              $self->information('Inheriting from \'' .
                                 $self->mpc_basename($file) .
                                 '\' in ' . $self->{'current_input'} .
                                 ' is redundant at line ' .
                                 $self->get_line_number() . '.');
            }
          }
        }
      }
      else {
        $status = 0;
        $error = "Unable to locate parent: $parent";
      }
    }
  }

  ## Copy each value from global_assign into assign
  if (!$self->{'reading_global'}) {
    foreach my $key (keys %{$self->{'global_assign'}}) {
      if (!defined $self->{'assign'}->{$key}) {
        $self->{'assign'}->{$key} = $self->{'global_assign'}->{$key};
      }
    }
  }

  return $status, $error;
}


sub get_process_project_type {
  my($self, $types) = @_;
  my $type    = '';
  my $defcomp = $self->get_default_component_name();

  foreach my $t (split(/\s*,\s*/, $types)) {
    my $not = ($t =~ s/^!\s*//);
    if ($not) {
      if ($t eq $self->{'pctype'}) {
        $type = '';
        last;
      }
      else {
        $type = $self->{'pctype'};
      }
    }
    elsif ($t eq $self->{'pctype'} || $t eq $defcomp) {
      $type = $t;
      last;
    }
  }

  return $type;
}


sub matches_specific_scope {
  my($self, $elements) = @_;

  ## First check for properties that correspond to the current project
  ## type.  Elements that begin with "prop:" indicate a property.
  my $list = '';
  my $props = $self->get_properties();
  foreach my $prop (split(/\s*,\s*/, $elements)) {
    my $not = ($prop =~ s/^!\s*//);
    if ($prop =~/(.+):(.+)/) {
      if ($1 eq 'prop') {
        $prop = $2;
        if ($not) {
          return $self->{'pctype'} if (!$$props{$prop});
        }
        else {
          return $self->{'pctype'} if ($$props{$prop});
        }
      }
      else {
        $self->warning("$prop is not recognized.");
      }
    }
    else {
      $list .= ($not ? '!' : '') . $prop . ',';
    }
  }

  ## If none of the elements match a property, then check the type
  ## against the current project type or the default component name
  ## (which is what it would be set to if a specific clause is used with
  ## out parenthesis).
  my $type = $self->get_process_project_type($list);
  return $self->{'pctype'} if ($type eq $self->{'pctype'} ||
                               $type eq $self->get_default_component_name());

  ## Nothing matched
  return undef;
}


sub parse_line {
  my($self, $ih, $line) = @_;
  my($status,
     $errorString,
     @values) = $self->parse_known($line);

  ## parse_known() passes back an array of values
  ## that make up the contents of the line parsed.
  ## The array can have 0 to 3 items.  The first,
  ## if defined, is always an identifier of some
  ## sort.

  if ($status && defined $values[0]) {
    if ($values[0] eq $self->{'grammar_type'}) {
      my $name      = $values[1];
      my $typecheck = $self->{'type_check'};
      if (defined $name && $name eq '}') {
        ## Project Ending
        if (!defined $self->{'reading_parent'}->[0] &&
            !$self->{'reading_global'}) {
          ## Fill in all the default values
          $self->generate_defaults();

          ## Perform any additions, subtractions
          ## or overrides for the project values.
          my $addproj = $self->get_addproj();
          foreach my $ap (keys %$addproj) {
            if (defined $self->{'valid_names'}->{$ap}) {
              foreach my $val (@{$$addproj{$ap}}) {
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
            }
            else {
              $errorString = 'Invalid ' .
                             "assignment modification name: $ap";
              $status = 0;
            }
          }

          if ($status) {
            ## Generate default target names after all source files are added
            ## and after we've added in all of the options from the
            ## command line.  If the user set exename on the command line
            ## and no "main" is found, sharedname will be set too and
            ## most templates do not handle that well.
            $self->generate_default_target_names();

            ## End of project; Write out the file.
            ($status, $errorString) = $self->write_project();

            ## write_project() can return 0 for error, 1 for project
            ## was written and 2 for project was skipped
            if ($status == 1) {
              ## Save the library name and location
              foreach my $name ('sharedname', 'staticname') {
                my $val = $self->get_assignment($name);
                if (defined $val) {
                  my $cwd    = $self->getcwd();
                  my $start  = $self->getstartdir();
                  my $amount = 0;
                  if ($cwd eq $start) {
                    $amount = length($start);
                  }
                  elsif (index($cwd, $start) == 0) {
                    $amount = length($start) + 1;
                  }
                  $self->{'lib_locations'}->{$val} =
                      substr($cwd, $amount);
                  last;
                }
              }

              ## Check for unused verbatim markers
              foreach my $key (keys %{$self->{'verbatim'}}) {
                if (defined $self->{'verbatim_accessed'}->{$key}) {
                  foreach my $ikey (keys %{$self->{'verbatim'}->{$key}}) {
                    if (!defined $self->{'verbatim_accessed'}->{$key}->{$ikey}) {
                      $self->warning("Marker $ikey does not exist.");
                    }
                  }
                }
              }
            }

            ## Reset all of the project specific data
            foreach my $key (keys %{$self->{'valid_components'}}) {
              delete $self->{$key};
              $self->{'defaulted'}->{$key} = 0;
            }
            if (defined $self->{'addtemp_state'}) {
              $self->restore_state($self->{'addtemp_state'}, 'addtemp');
              $self->{'addtemp_state'} = undef;
            }
            $self->{'assign'}                = {};
            $self->{'verbatim'}              = {};
            $self->{'verbatim_accessed'}     = {$self->{'pctype'} => {}};
            $self->{'special_supplied'}      = {};
            $self->{'flag_overrides'}        = {};
            $self->{'parents_read'}          = {};
            $self->{'inheritance_tree'}      = {};
            $self->{'remove_files'}          = {};
            $self->{'custom_special_output'} = {};
            $self->{'custom_special_depend'} = {};
            $self->{'expanded'}              = {};
            $self->reset_generating_types();
          }
        }
        $self->{$typecheck} = 0;
      }
      else {
        ## Project Beginning
        ($status, $errorString) = $self->begin_project($values[2]);

        ## Set up the default project name
        if ($status) {
          if (defined $name) {
            if ($name =~ /[\/\\]/) {
              $status = 0;
              $errorString = 'Projects can not have a slash ' .
                             'or a back slash in the name';
            }
            else {
              ## We should only set the project name if we are not
              ## reading in a parent project.
              if (!defined $self->{'reading_parent'}->[0]) {
                $name =~ s/^\(\s*//;
                $name =~ s/\s*\)$//;
                $name = $self->transform_file_name($name);

                ## Replace any *'s with the default name
                if (index($name, '*') >= 0) {
                  $name = $self->fill_type_name(
                                    $name,
                                    $self->get_default_project_name());
                }

                $self->set_project_name($name);
              }
              else {
                $self->warning("Ignoring project name " .
                               "$name in a base project.");
              }
            }
          }
        }

        ## Signify that we have a valid project
        $self->{$typecheck} = 1 if ($status);
      }
    }
    elsif ($values[0] eq '0') {
      ## $values[1] = name; $values[2] = value
      if (defined $self->{'valid_names'}->{$values[1]}) {
        $self->process_assignment($values[1], $values[2]);
      }
      else {
        $errorString = "Invalid assignment name: '$values[1]'";
        $status = 0;
      }
    }
    elsif ($values[0] eq '1') {
      ## $values[1] = name; $values[2] = value
      if (defined $self->{'valid_names'}->{$values[1]}) {
        $self->process_assignment_add($values[1], $values[2]);
      }
      else {
        $errorString = "Invalid addition name: $values[1]";
        $status = 0;
      }
    }
    elsif ($values[0] eq '-1') {
      ## $values[1] = name; $values[2] = value
      if (defined $self->{'valid_names'}->{$values[1]}) {
        $self->process_assignment_sub($values[1], $values[2]);
      }
      else {
        $errorString = "Invalid subtraction name: $values[1]";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'component') {
      my $comp = $values[1];
      my $name = $values[2];
      my $vc   = $self->{'valid_components'};

      if (defined $$vc{$comp}) {
        ($status, $errorString) = $self->parse_components($ih, $comp, $name);
      }
      else {
        if ($comp eq 'verbatim') {
          my($type, $loc, $add) = split(/\s*,\s*/, $name);
          ($status, $errorString) = $self->parse_verbatim($ih, $type,
                                                          $loc, $add);
        }
        elsif ($comp eq 'specific') {
          my $type = $self->matches_specific_scope($name);
          if (defined $type) {
            ($status, $errorString) = $self->parse_scope(
                                        $ih, $comp, $type,
                                        $self->{'valid_names'},
                                        $self->get_assignment_hash(),
                                        {});
          }
          else {
            ## We still need to parse the scope, but we will be
            ## throwing away whatever is processed.  However, it
            ## could still be invalid code that will cause an error.
            ($status, $errorString) = $self->parse_scope(
                                        $ih, $comp, undef,
                                        $self->{'valid_names'},
                                        undef,
                                        $self->get_assignment_hash());
          }
        }
        elsif ($comp eq 'define_custom') {
          ($status, $errorString) = $self->parse_define_custom($ih, $name);
        }
        elsif ($comp eq 'modify_custom') {
          ($status, $errorString) = $self->parse_define_custom($ih, $name, 1);
        }
        elsif ($comp eq 'expand') {
          $self->{'parsing_expand'} = 1;
          ($status, $errorString) = $self->parse_scope($ih, $comp, $name);
          $self->{'parsing_expand'} = undef;
        }
        else {
          $errorString = "Invalid component name: $comp";
          $status = 0;
        }
      }
    }
    elsif ($values[0] eq 'feature') {
      $self->{'feature_defined'} = 1;
      ($status, $errorString) = $self->process_feature($ih,
                                                       $values[1],
                                                       $values[2]);
      if ($status && $self->{'feature_defined'}) {
        $errorString = "Did not find the end of the feature";
        $status = 0;
      }
    }
    else {
      $errorString = "Unrecognized line: $line";
      $status = 0;
    }
  }
  elsif ($status == -1) {
    $status = 0;
  }

  return $status, $errorString;
}


sub parse_scoped_assignment {
  my($self, $tag, $type, $name, $value, $flags) = @_;

  ## Map the assignment name on a scoped assignment
  my $mapped = $self->{'valid_names'}->{$name};
  if (defined $mapped && UNIVERSAL::isa($mapped, 'ARRAY')) {
    $name = $$mapped[1];
  }

  if (defined $self->{'matching_assignments'}->{$tag} &&
      StringProcessor::fgrep($name, $self->{'matching_assignments'}->{$tag})) {
    my $over = {};
    if (defined $self->{'flag_overrides'}->{$tag}) {
      $over = $self->{'flag_overrides'}->{$tag};
    }
    else {
      $self->{'flag_overrides'}->{$tag} = $over;
    }

    if ($type == 0) {
      $self->process_assignment($name, $value, $flags);
    }
    elsif ($type == 1) {
      ## If there is no value in $$flags, then we need to get
      ## the outer scope value and put it in there.
      if (!defined $self->get_assignment($name, $flags)) {
        my $outer = $self->get_assignment($name);
        $self->process_assignment($name, $outer, $flags);
      }
      $self->process_assignment_add($name, $value, $flags);
    }
    elsif ($type == -1) {
      ## If there is no value in $$flags, then we need to get
      ## the outer scope value and put it in there.
      if (!defined $self->get_assignment($name, $flags)) {
        my $outer = $self->get_assignment($name);
        $self->process_assignment($name, $outer, $flags);
      }
      $self->process_assignment_sub($name, $value, $flags);
    }
    return 1;
  }

  return 0;
}


sub update_template_variable {
  my $self   = shift;
  my $check  = shift;
  my @values = @_;

  ## Save the addtemp state if we haven't done so before
  if (!defined $self->{'addtemp_state'}) {
    my %state = $self->save_state('addtemp');
    $self->{'addtemp_state'} = \%state;
  }

  ## If the name that is used within a specific is a mapped keyword
  ## then we need to translate it into the mapped keyword as it will
  ## be used by the TemplateParser.
  my $name;
  if ($values[1] =~ /(.*::)(.*)/) {
    my $base   = $1;
    my $mapped = $self->{'valid_names'}->{$2};
    if (defined $mapped && UNIVERSAL::isa($mapped, 'ARRAY')) {
      $name = $values[1];
      $values[1] = $base . 'custom_type->' . $$mapped[1];
    }
  }

  ## Now modify the addtemp values
  my $atemp = $self->get_addtemp();
  $self->information("'$values[1]' was used as a template modifier.");

  if ($check && !defined $atemp->{$values[1]}) {
    $name = $values[1] if (!defined $name);
    if ($name =~ s/.*:://) {
      my $value = $self->get_assignment($name);
      ## Regardless of whether there was and assignment value, we need to
      ## look at the template value of the base so that modification of a
      ## scoped variable includes the base values.
      if (defined $atemp->{$name}) {
        foreach my $arr (@{$atemp->{$name}}) {
          my @copy = @$arr;
          push(@{$atemp->{$values[1]}}, \@copy);
        }
      }
      unshift(@{$atemp->{$values[1]}},
              [0, $value, undef, $name]) if (defined $value);
    }
  }

  ## Subsitute all pseudo variables for the project specific characters.
  $values[2] = $self->replace_parameters($values[2], $self->{'command_subs'})
                 if (index($values[2], '<%') >= 0);

  if (defined $atemp->{$values[1]}) {
    ## If there are template variable settings, then we need to add
    ## this new one to the end of the settings that did not come from
    ## the command line.  That way, adjust_value() does not need to
    ## sort the values (and have knowledge about which came from the
    ## command line and which didn't).
    my $max = scalar(@{$atemp->{$values[1]}});
    for(my $i = 0; $i < $max; $i++) {
      if ($atemp->{$values[1]}->[$i]->[2]) {
        splice(@{$atemp->{$values[1]}}, $i, 0,
               [$values[0], $values[2], undef, $name]);
        return;
      }
    }
  }
  else {
    $atemp->{$values[1]} = [];
  }

  ## If the variable name is not scoped, we need to look through existing
  ## scoped variables that match the base.  If we find one, we need to
  ## propagate this value into the scoped settings.
  if (index($values[1], '::') == -1) {
    $name = $values[1] if (!defined $name);
    foreach my $key (keys %$atemp) {
      if ($key ne $name) {
        foreach my $entry (@{$atemp->{$key}}) {
          if ($$entry[3] eq $name) {
            push(@{$atemp->{$key}}, [$values[0], $values[2], undef, $name]);
            last;
          }
        }
      }
    }
  }

  ## 0: (0 set, 1 add, -1 subtract)
  ## 1: The text value
  ## 2: (true set on command line, false set in project)
  ## 3: The original variable name if it's scoped or mapped
  push(@{$atemp->{$values[1]}}, [$values[0], $values[2], undef, $name]);
}


sub handle_unknown_assignment {
  my $self   = shift;
  my $type   = shift;
  my @values = @_;

  ## Unknown assignments within a 'specific' section are handled as
  ## template value modifications.  These are handled exactly as the
  ## -value_template option in Options.pm.

  ## If $type is not defined, then we are skipping this section
  $self->update_template_variable(1, @values) if (defined $type);

  return 1, undef;
}


sub handle_scoped_unknown {
  my($self, $fh, $type, $flags, $line) = @_;

  if (defined $type && $self->{'parsing_expand'}) {
    if ($type eq $self->get_default_component_name()) {
      return 0, 'Can not set expansion in this context';
    }
    else {
      if (!defined $self->{'expanded'}->{$type}) {
        my $undef = $self->replace_env_vars(\$line);
        if (!$undef) {
          ## This is a special concession for Windows.  It will not allow
          ## you to set an empty environment variable.  If an empty
          ## double quoted string is found, we will assume that the user
          ## wanted an empty string.
          $line = '' if ($line eq '""');

          $self->{'expanded'}->{$type} = $line;
        }
      }
      return 1, undef;
    }
  }

  ## If the type is not defined, then this is something other than an
  ## assignment in a 'specific' section and should be flagged as an error
  return 0, "Unrecognized line: $line";
}

sub process_component_line {
  my($self, $tag, $line, $flags,
     $grname, $current, $excarr, $comps, $count) = @_;
  my $status = 1;
  my $error;
  my %exclude;
  my @values;

  ## If this returns true, then we've found an assignment
  if ($self->parse_assignment($line, \@values)) {
    $status = $self->parse_scoped_assignment($tag, @values, $flags);
    if (!$status) {
      $error = 'Unknown keyword: ' . $values[1];
    }
  }
  else {
    ## If we successfully remove a '!' from the front, then
    ## the file(s) listed are to be excluded
    my $rem = ($line =~ s/^\^\s*//);
    my $exc = $rem || ($line =~ s/^!\s*//);

    ## Convert any $(...) in this line before we process any
    ## wild card characters.  If we do not, scoped assignments will
    ## not work nor will we get the correct wild carded file list.
    ## We also need to make sure that any back slashes are converted to
    ## slashes to ensure that later flag_overrides checks will happen
    ## correctly.
    $line = $self->relative($line);
    $line =~ s/\\/\//g if ($self->{'convert_slashes'});

    ## Now look for specially listed files.
    ## Regular expressions are very slow.  Searching the line twice with
    ## index() is 328 times faster than searching with just the regular
    ## expression when it doesn't match (which is likely to be the case).
    if ((index($line, '>>') >= 0 || index($line, '<<') >= 0) &&
        $line =~ /(.*)\s+(>>|<<)\s+(.*)/) {
      $line   = $1;
      my $oop = $2;
      my $iop = ($oop eq '>>' ? '<<' : '>>');
      my $out = ($oop eq '>>' ? $3 : undef);
      my $dep = ($oop eq '<<' ? $3 : undef);

      $line =~ s/\s+$//;
      if (index($line, $iop) >= 0 && $line =~ /(.*)\s+$iop\s+(.*)/) {
        $line = $1;
        $out  = $2 if ($iop eq '>>');
        $dep  = $2 if ($iop eq '<<');
        $line =~ s/\s+$//;
      }

      ## Check for both possible error conditions
      if (index($line, $oop) >= 0) {
        $status = 0;
        $error  = "Duplicate $oop used";
      }
      elsif (index($line, $iop) >= 0) {
        $status = 0;
        $error  = "Duplicate $iop used";
      }

      ## Keys used internally to MPC need to be in forward slash format.
      my $key = $line;
      $key =~ s/\\/\//g if ($self->{'convert_slashes'});
      if (defined $out) {
        if (!defined $self->{'custom_special_output'}->{$tag}) {
          $self->{'custom_special_output'}->{$tag} = {};
        }
        ## We can not convert slashes here as we do for dependencies
        ## (below).  The files specified here need to retain the forward
        ## slashes as they are used elsewhere.
        $self->{'custom_special_output'}->{$tag}->{$key} = $self->create_array($out);
      }
      if (defined $dep) {
        $self->{'custom_special_depend'}->{$key} = $self->create_array($dep);
        if ($self->{'convert_slashes'}) {
          foreach my $depfile (@{$self->{'custom_special_depend'}->{$key}}) {
            $depfile =~ s/\//\\/g;
          }
        }
      }
    }

    ## If there is a command helper, we need to add the output files
    ## here.  It is possible that helper determined output files are
    ## the only files added by this component type.
    my $cmdHelper = CommandHelper::get($tag);
    if (defined $cmdHelper) {
      my $key = $line;
      $key =~ s/\\/\//g if ($self->{'convert_slashes'});
      my $cmdflags = $$flags{'commandflags'};
      my $add_out = $cmdHelper->get_output($key, $cmdflags);
      push(@{$self->{'custom_special_output'}->{$tag}->{$key}}, @$add_out);
    }

    ## Set up the files array.  If the line contains a wild card
    ## character use CORE::glob() to get the files specified.
    my @files;
    if ($line =~ /^"([^"]+)"$/) {
      push(@files, $1);
    }
    ## Don't glob the line if we're wanting to remove the file.  Wait
    ## until later to do the wildcard expansion (in remove_excluded).
    elsif (!$rem && $line =~ /[\?\*\[\]]/) {
      @files = $self->mpc_glob($line);
    }
    else {
      push(@files, $line);
    }

    ## If we want to remove these files at the end too, then
    ## add them to our remove_files hash array.
    if ($rem) {
      if (!defined $self->{'remove_files'}->{$tag}) {
        $self->{'remove_files'}->{$tag} = {};
      }
      foreach my $file (@files) {
        $self->{'remove_files'}->{$tag}->{$file} = 1;
      }
    }

    ## If we're excluding these files, then put them in the hash
    if ($exc) {
      $$grname = $current;
      @exclude{@files} = (@files);
      push(@$excarr, @files);
    }
    else {
      ## Set the flag overrides for each file
      my $over = $self->{'flag_overrides'}->{$tag};
      if (defined $over) {
        foreach my $file (@files) {
          $$over{$file} = $flags;
        }
      }

      foreach my $file (@files) {
        ## Add the file if we're not excluding it
        push(@{$$comps{$current}}, $file) if (!defined $exclude{$file});

        ## The user listed a file explicitly, whether we
        ## excluded it or not.
        ++$$count;
      }
    }
  }

  return $status, $error;
}


sub parse_conditional {
  my($self, $fh, $types, $tag, $flags,
     $grname, $current, $exclude, $comps, $count) = @_;
  my $status = 1;
  my $error;
  my $type = $self->matches_specific_scope($types);
  my $add = (defined $type ? 1 : 0);

  while(<$fh>) {
    my $line = $self->preprocess_line($fh, $_);

    if ($line eq '') {
    }
    elsif ($line =~ /^}\s*else\s*{$/) {
      $add ^= 1;
    }
    elsif ($line =~ /^}$/) {
      last;
    }
    elsif ($add) {
      ($status, $error) = $self->process_component_line(
                                              $tag, $line, $flags,
                                              $grname, $current,
                                              $exclude, $comps, $count);
      last if (!$status);
    }
  }

  return $status, $error;
}

sub parse_components {
  my($self, $fh, $tag, $name) = @_;
  my $current = $defgroup;
  my $status = 1;
  my $error;
  my $names = {};
  my $comps = {};
  my $set;
  my %flags;
  my @exclude;
  my $custom = defined $self->{'generated_exts'}->{$tag};
  my $grtag = $grouped_key . $tag;
  my $grname;

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
  $$comps{$current} = [] if (!defined $$comps{$current});

  my $count = 0;
  while(<$fh>) {
    my $line = $self->preprocess_line($fh, $_);

    if ($line eq '') {
    }
    elsif ($line =~ /^(\w+)\s*{$/) {
      if (!$set) {
        $current = $1;
        $set = 1;
        $$comps{$current} = [] if (!defined $$comps{$current});
      }
      else {
        $status = 0;
        $error  = 'Can not nest groups';
        last;
      }
    }
    elsif ($line =~ /^conditional\s*(\(([^\)]+)\))\s*{$/) {
      ($status, $error) = $self->parse_conditional(
                                         $fh, $2, $tag, \%flags, \$grname,
                                         $current, \@exclude, $comps,
                                         \$count);
      last if (!$status);
    }
    elsif ($line =~ /^}$/) {
      if (!defined $$comps{$current}->[0] && !defined $exclude[0]) {
        ## The default components name was never used
        ## so we remove it from the components
        delete $$comps{$current};
      }
      else {
        ## It was used, so we need to add that name to
        ## the set of group names unless it's already been added.
        $self->process_assignment_add($grtag, $current);
      }
      if ($set) {
        $current = $defgroup;
        $set = undef;
      }
      else {
        ## We are at the end of a component.  If the only group
        ## we added was the default group, then we need to remove
        ## the group setting altogether.
        my $groups = $self->get_assignment($grtag);
        if (defined $groups) {
          my $grarray = $self->create_array($groups);
          if (scalar(@$grarray) == 1 && $$grarray[0] eq $defgroup) {
            $self->process_assignment($grtag, undef);
          }
        }

        ## This is not an error,
        ## this is the end of the components
        last;
      }
    }
    else {
      ($status, $error) = $self->process_component_line($tag, $line, \%flags,
                                                        \$grname, $current,
                                                        \@exclude, $comps,
                                                        \$count);
      last if (!$status);
    }
  }

  ## If this is a "special" component, we need to see if the
  ## user provided all directories.  If they have, then we need to
  ## store an array of directories that the user supplied.  Otherwise,
  ## we just store a 1.
  if (defined $specialComponents{$tag}) {
    my @dirs;
    foreach my $name (keys %$names) {
      my $comps = $$names{$name};
      foreach my $comp (keys %$comps) {
        foreach my $item (@{$$comps{$comp}}) {
          if (-d $item) {
            push(@dirs, $item);
          }
          else {
            @dirs = ();
            last;
          }
        }
      }
    }
    if (defined $dirs[0]) {
      $self->{'special_supplied'}->{$tag} = \@dirs;
    }
    else {
      $self->{'special_supplied'}->{$tag} = 1;
    }
  }

  ## If we didn't encounter an error, didn't have any files explicitly
  ## listed and we attempted to exclude files, then we need to find the
  ## set of files that don't match the excluded files and add them.
  if ($status && defined $exclude[0] && defined $grname) {
    my $alldir = $self->get_assignment('recurse') || $flags{'recurse'};
    my %checked;
    my @files;
    foreach my $exc (@exclude) {
      my $dname = $self->mpc_dirname($exc);
      if (!defined $checked{$dname}) {
        $checked{$dname} = 1;
        push(@files, $self->generate_default_file_list($dname,
                                                       \@exclude,
                                                       undef, $alldir));
      }
    }

    $self->sift_files(\@files,
                      $self->{'valid_components'}->{$tag},
                      $self->get_assignment('pch_header'),
                      $self->get_assignment('pch_source'),
                      $tag,
                      $$comps{$grname});
  }

  return $status, $error;
}


sub parse_verbatim {
  my($self, $fh, $type, $loc, $add) = @_;

  if (!defined $loc) {
    return 0, 'You must provide a location parameter to verbatim';
  }

  ## All types are lower case
  $type = lc($type);

  if (!defined $self->{'verbatim'}->{$type}) {
    $self->{'verbatim'}->{$type} = {};
  }

  ## Instead of always creating a new array for a particular type and
  ## location, create a new array if there isn't one already or the user
  ## does not want to add to the existing verbatim settings.
  $self->{'verbatim'}->{$type}->{$loc} = []
          if (!$add || !defined $self->{'verbatim'}->{$type}->{$loc});
  my $array = $self->{'verbatim'}->{$type}->{$loc};

  while(<$fh>) {
    my $line = $self->preprocess_line($fh, $_);

    ## This is not an error,
    ## this is the end of the verbatim
    last if ($line =~ /^}$/);
    push(@$array, $line);
  }

  return 1, undef;
}


sub process_feature {
  my($self, $fh, $names, $parents) = @_;
  my $status = 1;
  my $error;

  my $requires = '';
  my $avoids   = '';
  foreach my $name (@$names) {
    if ($name =~ /^!\s*(.*)$/) {
      $avoids .= ' ' if ($avoids ne '');
      $avoids .= $1;
    }
    else {
      $requires .= ' ' if ($requires ne '');
      $requires .= $name;
    }
  }

  if ($self->check_features($requires, $avoids)) {
    ## The required features are enabled, so we say that
    ## a project has been defined and we allow the parser to
    ## find the data held within the feature.
    ($status, $error) = $self->begin_project($parents);
    if ($status) {
      $self->{'feature_defined'} = 0;
      $self->{$self->{'type_check'}} = 1;
    }
  }
  else {
    ## Otherwise, we read in all the lines until we find the
    ## closing brace for the feature and it appears to the parser
    ## that nothing was defined.
    my $curly = 1;
    while(<$fh>) {
      my $line = $self->preprocess_line($fh, $_);

      ## This is a very simplistic way of finding the end of
      ## the feature definition.  It will work as long as no spurious
      ## open curly braces are counted.
      ++$curly if ($line =~ /{$/);
      --$curly if ($line =~ /^}/);

      if ($curly == 0) {
        $self->{'feature_defined'} = 0;
        last;
      }
    }
  }

  return $status, $error;
}


sub process_array_assignment {
  my($self, $aref, $type, $array) = @_;

  if (!defined $$aref || $type == 0) {
    if ($type != -1) {
      $$aref = $array;
    }
  }
  else {
    if ($type == 1) {
      push(@{$$aref}, @$array);
    }
    elsif ($type == -1) {
      my $count = scalar(@{$$aref});
      for(my $i = 0; $i < $count; ++$i) {
        if (StringProcessor::fgrep($$aref->[$i], $array)) {
          splice(@{$$aref}, $i, 1);
          --$i;
          --$count;
        }
      }
    }
  }
}


sub parse_define_custom {
  my($self, $fh, $tag, $modify) = @_;

  ## Make the tag something _files
  $tag = lc($tag) . '_files';

  ## We can not have a custom type named "generic"
  return 0, "$tag is reserved" if ($tag eq $generic_key);

  if (defined $self->{'valid_components'}->{$tag}) {
    if (!$modify) {
      return 0, "$tag has already been defined";
    }
  }
  elsif ($modify) {
    return 0, "$tag has not yet been defined and can not be modified";
  }

  my $status      = 0;
  my $errorString = "Unable to process $tag";

  ## Update the custom_types assignment
  $self->process_assignment_add('custom_types', $tag) if (!$modify);

  if (!defined $self->{'matching_assignments'}->{$tag}) {
    my @keys = keys %custom;
    push(@keys, @default_matching_assignments);
    $self->{'matching_assignments'}->{$tag} = \@keys;
  }

  my $optname;
  my $inscope = 0;
  while(<$fh>) {
    my $line = $self->preprocess_line($fh, $_);

    if ($line eq '') {
    }
    elsif ($line =~ /optional\s*\(([^\)]+)\)\s*{/) {
      $optname = $1;
      $optname =~ s/^\s+//;
      $optname =~ s/\s+$//;
      if (defined $customDefined{$optname} &&
          ($customDefined{$optname} & 0x08) != 0) {
        ++$inscope;
        if ($inscope != 1) {
          $status = 0;
          $errorString = 'Can not nest \'optional\' sections';
          last;
        }
      }
      else {
        $status = 0;
        $errorString = "Invalid optional name: $optname";
        last;
      }
    }
    elsif ($inscope) {
      if ($line =~ /^}$/) {
        $optname = undef;
        --$inscope;
      }
      else {
        if ($line =~ /(\w+)\s*\(([^\)]+)\)\s*(\+)?=\s*(.*)/) {
          my $name = lc($1);
          my $opt  = $2;
          my $add  = $3;
          my @val  = split(/\s*,\s*/, $4);

          ## Fix $opt spacing
          $opt =~ s/(\&\&|\|\|)/ $1 /g;
          $opt =~ s/!\s+/!/g;

          ## Set up the 'optional' hash table
          if (!$add || !defined $self->{'generated_exts'}->{$tag}->
                              {'optional'}->{$optname}->{$name}->{$opt}) {
            $self->{'generated_exts'}->{$tag}->
                   {'optional'}->{$optname}->{$name}->{$opt} = \@val;
          }
          else {
            push(@{$self->{'generated_exts'}->{$tag}->{'optional'}->
                    {$optname}->{$name}->{$opt}}, @val);
          }
        }
        else {
          $status = 0;
          $errorString = "Unrecognized optional line: $line";
          last;
        }
      }
    }
    elsif ($line =~ /^}$/) {
      $status = 1;
      $errorString = undef;

      ## Propagate the custom defined values into the mapped values
      foreach my $key (keys %{$self->{'valid_names'}}) {
        if (UNIVERSAL::isa($self->{'valid_names'}->{$key}, 'ARRAY')) {
          my $value = $self->{'generated_exts'}->{$tag}->{
                                $self->{'valid_names'}->{$key}->[1]};

          ## Bypass the process_assignment() defined in this class
          ## to avoid unwanted keyword mapping.
          $self->SUPER::process_assignment($key, $value) if (defined $value);
        }
      }

      ## Set some defaults (if they haven't already been set)
      if (!defined $self->{'generated_exts'}->{$tag}->{'pre_filename'}) {
        $self->{'generated_exts'}->{$tag}->{'pre_filename'} = [ '' ];
      }
      if (!defined $self->{'generated_exts'}->{$tag}->{'pre_dirname'}) {
        $self->{'generated_exts'}->{$tag}->{'pre_dirname'} = [ '' ];
      }
      if (!defined $self->{'generated_exts'}->{$tag}->{'pre_extension'}) {
        $self->{'generated_exts'}->{$tag}->{'pre_extension'} = [ '' ];
      }
      if (!defined $self->{'generated_exts'}->{$tag}->{'automatic_in'}) {
        $self->{'generated_exts'}->{$tag}->{'automatic_in'} = 1;
      }
      if (!defined $self->{'generated_exts'}->{$tag}->{'automatic_out'}) {
        $self->{'generated_exts'}->{$tag}->{'automatic_out'} = 1;
      }
      if (!defined $self->{'generated_exts'}->{$tag}->{'output_follows_input'}) {
        $self->{'generated_exts'}->{$tag}->{'output_follows_input'} = 1;
      }
      if (!defined $self->{'valid_components'}->{$tag}) {
        $self->{'valid_components'}->{$tag} = [];
      }
      last;
    }
    else {
      my @values;
      ## If this returns true, then we've found an assignment
      if ($self->parse_assignment($line, \@values)) {
        my($type, $name, $value) = @values;
        ## The 'automatic' keyword has always contained two distinct
        ## functions.  The first is to automatically add input files of
        ## the specified extension.  And the second is to automatically
        ## add generated files to the right components.  It has now been
        ## split into separate functionality and we map the 'automatic'
        ## keyword to the two new ones here.
        my $ok = 1;
        my @names = $name eq 'automatic' ?
                        ('automatic_in', 'automatic_out') : $name;
        foreach $name (@names) {
          if (defined $customDefined{$name}) {
            if (($customDefined{$name} & 0x01) != 0) {
              $value = $self->escape_regex_special($value);
              my @array = split(/\s*,\s*/, $value);
              $self->process_array_assignment(
                        \$self->{'valid_components'}->{$tag}, $type, \@array);
            }
            else {
              if (!defined $self->{'generated_exts'}->{$tag}) {
                $self->{'generated_exts'}->{$tag} = {};
              }
              ## Try to convert the value into a relative path
              $value = $self->relative($value);

              if (($customDefined{$name} & 0x04) != 0) {
                if ($type == 0) {
                  $self->process_assignment(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
                elsif ($type == 1) {
                  $self->process_assignment_add(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
                elsif ($type == -1) {
                  $self->process_assignment_sub(
                                     $name, $value,
                                     $self->{'generated_exts'}->{$tag});
                }
              }
              else {
                if (($customDefined{$name} & 0x02) != 0) {
                  ## Transform the name from something outputext to
                  ## something files.  We expect this to match the
                  ## names of valid_assignments.
                  $name =~ s/outputext/files/g;
                }

                ## Get it ready for regular expressions
                $value = $self->escape_regex_special($value);

                ## Split the value into an array using a comma as the
                ## separator.  If there are no elements in the array we're
                ## going to add an empty element to the array.  This way,
                ## assignments of blank values are useful.
                my @array = split(/\s*,\s*/, $value);
                push(@array, '') if ($#array == -1);

                ## Process the array assignment after adjusting the values
                $self->process_array_assignment(
                            \$self->{'generated_exts'}->{$tag}->{$name},
                            $type, \@array);
              }
            }
          }
          else {
            $ok = 0;
            $status = 0;
            $errorString = "Invalid assignment name: '$name'";
            last;
          }
        }

        ## $status is zero until the end of the define custom block, so
        ## we can't use it for this check.
        last if (!$ok);
      }
      elsif ($line =~ /^keyword\s+(\w+)(?:\s*=\s*(\w+)?)?/) {
        ## Check for keyword mapping here
        my $newkey = $1;
        my $mapkey = $2;
        if (defined $self->{'valid_names'}->{$newkey}) {
          $status = 0;
          $errorString = "Cannot map $newkey onto an " .
                         "existing keyword";
          last;
        }
        elsif (!defined $mapkey) {
          $self->{'valid_names'}->{$newkey} = 1;
        }
        elsif ($newkey ne $mapkey) {
          if (defined $customDefined{$mapkey}) {
            $self->{'valid_names'}->{$newkey} = [ $tag, $mapkey ];
          }
          else {
            $status = 0;
            $errorString = "Cannot map $newkey to an " .
                           "undefined custom keyword: $mapkey";
            last;
          }
        }
        else {
          $status = 0;
          $errorString = "Cannot map $newkey to $mapkey";
          last;
        }
      }
      else {
        $status = 0;
        $errorString = "Unrecognized line: $line";
        last;
      }
    }
  }

  return $status, $errorString;
}


sub back_to_variable {
  my($self, $values) = @_;
  my $cwd = $self->getcwd();
  my $case_tolerant = $self->case_insensitive();
  my @values = ();

  ## Get both of the relative value hash maps and put them in an array
  my @rels = ();
  my($rel, $how) = $self->get_initial_relative_values();
  push(@rels, $rel);
  ($rel, $how) = $self->get_secondary_relative_values();
  push(@rels, $rel);

  ## Go through each value and try to convert it to a variable setting
  foreach my $ovalue (@$values) {
    ## Fix up the value, replacing '.' with the current working
    ## directory.
    my $value = $ovalue;
    $value =~ s/\\/\//g;
    if ($value eq '.') {
      $value = $cwd;
    }
    else {
      $value =~ s/^.\//$cwd\//;
    }
    my $valuelen = length($value);

    ## Go through each relative value hash map and see if any of the
    ## values match the value that we're currently inspecting.
    my $found = undef;
    foreach my $rel (@rels) {
      foreach my $key (keys %$rel) {
        ## Get the relative replacement value and convert back-slashes
        my $val = $$rel{$key};
        $val =~ s/\\/\//g;

        ## We only need to check for reverse replacement if the length
        ## of the value is greater than or equal to the length of our
        ## replacement value.
        my $vlen = length($val);
        if ($valuelen >= $vlen) {
          ## Cut the string down by the length of the replacement value
          my $lval = substr($value, 0, $vlen);

          ## Check for equivalence, taking into account file system
          ## case-insenitivity.
          if ($case_tolerant) {
            $found = (lc($lval) eq lc($val));
          }
          else {
            $found = ($lval eq $val);
          }

          ## If they match, replace the value and save it in our array.
          if ($found) {
            substr($value, 0, length($val)) = "\$($key)";
            push(@values, $value);
            last;
          }
        }
      }

      ## Once it's been found, there's no reason to continue on through
      ## the relative hash maps.
      last if ($found);
    }

    push(@values, $ovalue) if (!$found);
  }

  return @values;
}


sub remove_duplicate_addition {
  my($self, $name, $value, $nval) = @_;

  if (defined $nval) {
    ## If we are modifying the libs, libpaths, macros or includes
    ## assignment with either addition or subtraction, we are going to
    ## perform a little fix on the value to avoid multiple
    ## libraries and to try to insure the correct linking order
    if ($name eq 'macros' || $name eq 'libpaths' ||
        $name eq 'includes' || $name =~ /libs$/ ||
        index($name, $grouped_key) == 0) {
      my $allowed = '';
      my %parts;

      ## Convert the array into keys for a hash table
      @parts{@{$self->create_array($nval)}} = ();

      ## In order to ensure that duplicates are correctly removed, we
      ## need to get the modified assignment value before we attempt to
      ## do so.
      $value = $self->modify_assignment_value($name, $value);
      foreach my $val (@{$self->create_array($value)}) {
        if (!exists $parts{$val}) {
          ## We need to supply quotes if there is a space in the value or
          ## a variable.  The variable may contain spaces.
          my $qt = ($val =~ /\s/ || $val =~ /\$\(.+\)/ ? '"' : '');
          $allowed .= $qt . $val . $qt . ' ';
        }
      }
      $allowed =~ s/\s+$//;
      return $allowed;
    }
  }

  return $value;
}


sub read_template_input {
  my($self, $tkey) = @_;
  my $status = 1;
  my $errorString;
  my $file;
  my $tag;
  my $ti = $self->get_ti_override();
  my $lang = $self->get_language();
  my $override;

  if ($self->exe_target()) {
    if ($self->get_static() == 1) {
      $tag = 'lib_exe_template_input';
      ## Check for the TemplateInputReader for the template key provided.
      if (!defined $self->{$tag}->{$lang}->{$tkey}) {
        if (defined $$ti{'lib_exe'}) {
          $file = $$ti{'lib_exe'};
          $override = 1;
        }
        else {
          $file = $self->get_lib_exe_template_input_file($tkey);
        }
      }
    }
    else {
      $tag = 'dll_exe_template_input';
      ## Check for the TemplateInputReader for the template key provided.
      if (!defined $self->{$tag}->{$lang}->{$tkey}) {
        if (defined $$ti{'dll_exe'}) {
          $file = $$ti{'dll_exe'};
          $override = 1;
        }
        else {
          $file = $self->get_dll_exe_template_input_file($tkey);
        }
      }
    }
  }
  else {
    if ($self->get_static() == 1) {
      $tag = 'lib_template_input';
      ## Check for the TemplateInputReader for the template key provided.
      if (!defined $self->{$tag}->{$lang}->{$tkey}) {
        if (defined $$ti{'lib'}) {
          $file = $$ti{'lib'};
          $override = 1;
        }
        else {
          $file = $self->get_lib_template_input_file($tkey);
        }
      }
    }
    else {
      $tag = 'dll_template_input';
      ## Check for the TemplateInputReader for the template key provided.
      if (!defined $self->{$tag}->{$lang}->{$tkey}) {
        if (defined $$ti{'dll'}) {
          $file = $$ti{'dll'};
          $override = 1;
        }
        else {
          $file = $self->get_dll_template_input_file($tkey);
        }
      }
    }
  }

  if (defined $self->{$tag}->{$lang}->{$tkey}) {
    ## We have a TemplateInputReader for this template key, so we need
    ## to set the entry corresponding to $tikey to it for use in the
    ## get_template_input() method.
    $self->{$tag}->{$lang}->{$tikey} = $self->{$tag}->{$lang}->{$tkey};
  }
  else {
    ## We haven't read this file yet, so we will create the template
    ## input reader and store it in the entry for the template key
    ## ($tkey) and the template input key ($tikey).
    my $ti = new TemplateInputReader($self->get_include_path());
    $self->{$tag}->{$lang}->{$tkey} = $ti;
    $self->{$tag}->{$lang}->{$tikey} = $ti;

    ## Process the template input file
    if (defined $file) {
      my $tfile = $self->search_include_path("$file.$TemplateInputExtension");
      if (defined $tfile) {
        ($status, $errorString) = $ti->read_file($tfile);
      }
      else {
        ## Not finding a template input file is only an error if the user
        ## specifically provided a template input file override.
        if ($override) {
          $status = 0;
          $errorString = "Unable to locate template input file: $file";
        }
      }
    }

    ## Now that we've read in the template input file, set up our
    ## automatic template variables.
    if ($self->{'make_coexistence'}) {
      $ti->parse_line(undef,
                      "make_coexistence = $self->{'make_coexistence'}");
    }
  }

  ## We do this regardless of whether or not this parser is cached or
  ## not.  If the features have changed (through a workspace cmdline
  ## setting), we need to reflect it.
  if ($status) {
    ## Put the features into the template input set
    my $features = $self->{'feature_parser'}->get_names();
    $self->{$tag}->{$lang}->{$tikey}->parse_line(undef,
                                                 "features = @$features");
  }

  return $status, $errorString;
}


sub already_added {
  my($self, $array, $name) = @_;

  ## This method expects that the file name will be unix style
  $name =~ s/\\/\//g if ($self->{'convert_slashes'});

  ## Remove the leading ./
  $name =~ s/^\.\///;
  my $dsname = "./$name";

  foreach my $file (@$array) {
    return 1 if ($file eq $name || $file eq $dsname);
  }

  return 0;
}


sub get_applied_custom_keyword {
  my($self, $name, $type, $file) = @_;

  if (defined $self->{'flag_overrides'}->{$type} &&
      defined $self->{'flag_overrides'}->{$type}->{$file} &&
      defined $self->{'flag_overrides'}->{$type}->{$file}->{$name}) {
    return $self->relative(
             $self->{'flag_overrides'}->{$type}->{$file}->{$name}, 1);
  }

  return $self->relative($self->get_assignment(
                                   $name,
                                   $self->{'generated_exts'}->{$type}), 1);
}


sub evaluate_optional_option {
  my($self, $opt, $value) = @_;

  if ($opt =~ /^!\s*(.*)/) {
    return (!exists $$value{$1} ? 1 : 0);
  }
  else {
    return (exists $$value{$opt} ? 1 : 0);
  }
}


sub process_optional_option {
  my($self, $opt, $value) = @_;
  my $status;
  my @parts  = grep(!/^$/, split(/\s+/, $opt));
  my $pcount = scalar(@parts);

  for(my $i = 0; $i < $pcount; $i++) {
    if ($parts[$i] eq '&&' || $parts[$i] eq '||') {
      if (defined $status) {
        if (defined $parts[$i + 1]) {
          if ($parts[$i] eq '&&') {
            $status &&= $self->evaluate_optional_option($parts[$i + 1],
                                                        $value);
          }
          else {
            ## We are coming into an '||', if status is already true
            ## then we can leave immediately
            last if ($status);

            $status ||= $self->evaluate_optional_option($parts[$i + 1],
                                                        $value);
          }
        }
        else {
          $self->warning("Expected token in optional after $parts[$i]");
        }
      }
      else {
        $self->warning("Unexpected token in optional: $parts[$i]");
      }
      ++$i;
    }
    else {
      if (!defined $status) {
        $status = $self->evaluate_optional_option($parts[$i], $value);
      }
      else {
        $self->warning("Unexpected token in optional: $parts[$i]");
      }
    }
  }

  return $status;
}


sub add_optional_filename_portion {
  my($self, $gentype, $tag, $file, $array) = @_;

  if (defined $self->{'generated_exts'}->{$gentype}->{'optional'}->{$tag}) {
    foreach my $name (keys %{$self->{'generated_exts'}->{$gentype}->{'optional'}->{$tag}}) {
      foreach my $opt (keys %{$self->{'generated_exts'}->{$gentype}->{'optional'}->{$tag}->{$name}}) {
        ## Get the name value
        my $value = $self->get_applied_custom_keyword($name,
                                                      $gentype, $file);

        ## Convert the value into a hash map for easy lookup
        my %values;
        @values{split(/\s+/, $value)} = () if (defined $value);

        ## See if the option or options are contained in the value.  We
        ## need to call this even if $value is not defined due to the
        ## ability to negate optional parameters.
        if ($self->process_optional_option($opt, \%values)) {
          ## Add the optional portion
          push(@$array, @{$self->{'generated_exts'}->{$gentype}->{'optional'}->{$tag}->{$name}->{$opt}});
        }
      }
    }
  }
}


sub get_pre_keyword_array {
  my($self, $keyword, $gentype, $tag, $file) = @_;

  ## Get the general pre extension array.
  ## $self->{'generated_exts'}->{$gentype}->{$keyword} is guaranteed to
  ## be defined due to the defaulting that is done in
  ## parse_define_custom() and the only three calls to this method use
  ## valid $keyword values.
  my @array = @{$self->{'generated_exts'}->{$gentype}->{$keyword}};

  ## Add the component specific pre extension array
  my @additional;
  $tag =~ s/files$/$keyword/;
  if (defined $self->{'generated_exts'}->{$gentype}->{$tag}) {
    push(@additional, @{$self->{'generated_exts'}->{$gentype}->{$tag}});
  }

  ## Add in any optional portion to the array
  foreach my $itag ($keyword, $tag) {
    $self->add_optional_filename_portion($gentype, $itag,
                                         $file, \@additional);
  }

  ## If the current array only has the default,
  ## then we need to remove it
  if (defined $additional[0]) {
    if ($#array == 0 && $array[0] eq '') {
      pop(@array);
    }
    push(@array, @additional);
  }

  return @array;
}


sub add_explicit_output {
  my($self, $file, $type, $tag, $array, $arrs) = @_;

  if (defined $self->{'custom_special_output'}->{$type} &&
      defined $self->{'custom_special_output'}->{$type}->{$file}) {
    if (defined $self->{'valid_components'}->{$tag}) {
      my @files;
      foreach my $check (@{$self->{'custom_special_output'}->{$type}->{$file}}) {
        foreach my $regext (@{$self->{'valid_components'}->{$tag}}) {
          if ($check =~ /$regext$/) {
            my $add = 1;
            if ($tag eq 'source_files') {
              foreach my $tregext (@{$self->{'valid_components'}->{'template_files'}}) {
                if ($check =~ /$tregext$/) {
                  $add = undef;
                  last;
                }
              }
            }
            if ($add) {
              ## If gendir was specified, then we need to account for that
              my $dir = '';
              if (defined $self->{'flag_overrides'}->{$type} &&
                  defined $self->{'flag_overrides'}->{$type}->{$file} &&
                  defined $self->{'flag_overrides'}->{$type}->{$file}->{'gendir'} &&
                  $self->{'flag_overrides'}->{$type}->{$file}->{'gendir'} ne '.') {
                $dir = $self->{'flag_overrides'}->{$type}->{$file}->{'gendir'} . '/';
                $dir =~ s/\\/\//g if ($self->{'convert_slashes'});
              }

              push(@files, "$dir$check");
              last;
            }
          }
        }
      }
      if (defined $files[0]) {
        if ($arrs) {
          push(@$array, \@files);
        }
        else {
          push(@$array, @files);
        }
      }
    }
  }
}

sub generated_filenames {
  my($self, $part, $type, $tag, $file, $noext, $arrs) = @_;

  ## A custom type is not allowed to generate it's own input files
  return () if ($type eq $tag);

  ## See if the type for which we are generating ($tag) is also a custom
  ## file type.  If it is, we need to do some massaging.
  my $otag = $tag;
  if (defined $self->{'generated_exts'}->{$tag}) {
    ## If the custom type ($type) doesn't specify that it generates
    ## generic files, we need to see if there is a command helper for
    ## this type and see what sort of output it knows about.
    my $inputexts = $self->{'generated_exts'}->{$type}->{$generic_key};
    if (!defined $inputexts) {
      my $cmdHelper = CommandHelper::get($type);
      $inputexts = $cmdHelper->get_outputexts() if (defined $cmdHelper);
    }

    ## We will need to use 'generic_files' instead of $tag if $tag is
    ## defined in 'generated_exts', but only for the type that will
    ## actually generate the right type of generic file.
    my $good;
    if (defined $inputexts) {
      foreach my $inputext (@$inputexts) {
        my $ext = $inputext;
        $ext =~ s/\\//g;
        foreach my $extreg (@{$self->{'valid_components'}->{$tag}}) {
          if ($ext =~ /$extreg$/) {
            $tag = $generic_key;
            $good = 1;
            last;
          }
        }
        last if ($good);
      }
    }
    return () if (!$good);
  }

  my @pearr = $self->get_pre_keyword_array('pre_extension',
                                           $type, $tag, $file);
  my @pfarr = $self->get_pre_keyword_array('pre_filename',
                                           $type, $tag, $file);
  my @pdarr = $self->get_pre_keyword_array('pre_dirname',
                                           $type, $tag, $file);
  my @exts  = (defined $self->{'generated_exts'}->{$type}->{$tag} ?
                 @{$self->{'generated_exts'}->{$type}->{$tag}} : ());

  if (!defined $exts[0]) {
    my $backtag = $tag;
    if ($backtag =~ s/files$/outputext/) {
      $self->add_optional_filename_portion($type, $backtag,
                                           $file, \@exts);
    }
  }

  my @array;
  if (!defined $exts[0] && $#pearr == 0 && $#pfarr == 0 && $#pdarr == 0 &&
      $pearr[0] eq '' && $pfarr[0] eq '' && $pdarr[0] eq '') {
    ## If both arrays are defined to be the defaults, then there
    ## is nothing for us to do.
  }
  else {
    my $dir  = '';
    my $base;

    ## Correctly deal with pre filename and directories
    if ($part =~ /(.*[\/\\])([^\/\\]+)$/) {
      ## Split the directory and base name of the file.  Only set the
      ## directory if the output follows the input directory.
      $dir = $1
        if ($self->{'generated_exts'}->{$type}->{'output_follows_input'});
      $base = $2;
    }
    else {
      $base = $part;
    }

    ## If gendir was specified, then we need to account for that
    if (defined $self->{'flag_overrides'}->{$type} &&
        defined $self->{'flag_overrides'}->{$type}->{$file} &&
        defined $self->{'flag_overrides'}->{$type}->{$file}->{'gendir'}) {
      if ($self->{'flag_overrides'}->{$type}->{$file}->{'gendir'} eq '.') {
        $dir = '';
      }
      else {
        $dir = $self->{'flag_overrides'}->{$type}->{$file}->{'gendir'} . '/';
        $dir =~ s/\\/\//g if ($self->{'convert_slashes'});
      }
    }

    ## Loop through creating all of the possible file names
    foreach my $pe (@pearr) {
      my @genfile;
      $pe =~ s/\\\././g;
      foreach my $pf (@pfarr) {
        $pf =~ s/\\\././g;
        foreach my $pd (@pdarr) {
          if ($noext) {
            push(@genfile, "$pd$dir$pf$base$pe");
          }
          else {
            foreach my $ext (@exts) {
              $ext =~ s/\\\././g;
              push(@genfile, "$pd$dir$pf$base$pe$ext");
            }
          }
        }
      }
      if ($arrs) {
        push(@array, \@genfile);
      }
      else {
        push(@array, @genfile);
      }
    }
  }

  ## Now add the explicit output.  We need to use the original tag value
  ## ($otag) so that we can find the custom output files.
  $self->add_explicit_output($file, $type, $otag, \@array, $arrs);
  return @array;
}


sub add_generated_files {
  my($self, $gentype, $tag, $group, $arr) = @_;

  ## This method is called by list_default_generated.  It performs the
  ## actual file insertion and grouping.

  ## Get the generated filenames
  my @added;
  foreach my $file (keys %$arr) {
    foreach my $gen ($self->generated_filenames($$arr{$file}, $gentype,
                                                $tag, $file, 1)) {
      $self->list_generated_file($gentype, $tag, \@added, $gen, $$arr{$file});
    }
  }

  if (defined $added[0]) {
    my $names = $self->{$tag};

    ## Get all files in one list and save the directory
    ## and component group in a hashed array.
    my @all;
    my %dircomp;
    foreach my $name (keys %$names) {
      foreach my $key (keys %{$$names{$name}}) {
        push(@all, @{$$names{$name}->{$key}});
        foreach my $file (@{$$names{$name}->{$key}}) {
          $dircomp{$self->mpc_dirname($file)} = $key;
        }
      }
    }

    ## Create a small array of only the files we want to add.
    ## We put them all together so we can keep them in order when
    ## we put them at the front of the main file list.
    my @oktoadd;
    foreach my $file (@added) {
      push(@oktoadd, $file) if (!$self->already_added(\@all, $file));
    }

    ## If we have files to add, make sure we add them to a group
    ## that has the same directory location as the files we're adding.
    if (defined $oktoadd[0]) {
      my $key = (defined $group ? $group :
                         $dircomp{$self->mpc_dirname($oktoadd[0])});
      if (!defined $key) {
        my $check = $oktoadd[0];
        foreach my $regext (@{$self->{'valid_components'}->{$tag}}) {
          last if ($check =~ s/$regext$//);
        }
        foreach my $vc (keys %{$self->{'valid_components'}}) {
          ## If this component name does not match the component name for
          ## which we are adding files and there are components defined
          ## for it, we will look to see if we can find a matching group
          ## name.  We have to make sure that we do not use the hash map
          ## ($self->{$vc}) unless it's defined.  Doing so will
          ## automatically create the map and that will cause MPC to
          ## think that the user provided the empty setting (when it
          ## wasn't).
          if ($vc ne $tag && defined $self->{$vc}) {
            foreach my $name (keys %{$self->{$vc}}) {
              foreach my $ckey (keys %{$self->{$vc}->{$name}}) {
                if ($ckey ne $defgroup) {
                  foreach my $ofile (@{$self->{$vc}->{$name}->{$ckey}}) {
                    my $file = $ofile;
                    foreach my $regext (@{$self->{'valid_components'}->{$vc}}) {
                      last if ($file =~ s/$regext$//);
                    }
                    if ($file eq $check) {
                      $key = $ckey;
                      last;
                    }
                  }
                }
                last if (defined $key);
              }
            }
            last if (defined $key);
          }
        }
        $key = $defgroup if (!defined $key);
      }
      foreach my $name (keys %$names) {
        if (!defined $$names{$name}->{$key}) {
          if ($key ne $defgroup &&
              defined $$names{$name}->{$defgroup} &&
              defined $$names{$name}->{$defgroup}->[0]) {
            $self->process_assignment_add($grouped_key . $tag, $defgroup);
          }
          $$names{$name}->{$key} = [];
          $self->process_assignment_add($grouped_key . $tag, $key);
        }
        unshift(@{$$names{$name}->{$key}}, @oktoadd);
      }
    }
  }
}


sub search_for_entry {
  my($self, $file, $marray, $preproc) = @_;
  my $name;
  my $fh = new FileHandle();

  if (open($fh, $file)) {
    my $poundifed = 0;
    my $commented = 0;

    while(<$fh>) {
      ## Remove c++ style comments
      $_ =~ s/\/\/.*// if (!$commented);

      ## Remove one line c style comments
      $_ =~ s/\/\*.*\*\///g;

      if ($commented) {
        if (/\*\//) {
          ## Found the end of a multi-line c style comment
          --$commented;
        }
      }
      else {
        if (/\/\*/) {
          ## Found the beginning of a multi-line c style comment
          ++$commented;
        }
        elsif ($preproc) {
          ## If the current language supports a c preprocessor, we
          ## will perform a minimal check for #if 0
          if (/#\s*if\s+0/) {
            ## Found the beginning of a #if 0
            ++$poundifed;
          }
          elsif ($poundifed) {
            if (/#\s*if/) {
              ## We need to keep track of any other #if directives
              ## to be sure that when we see an #endif we don't
              ## count the wrong one.
              ++$poundifed;
            }
            elsif (/#\s*endif/) {
              ## Found a #endif, so decrement our count
              --$poundifed;
            }
          }
        }
      }

      ## Check for main; Make sure it's not #if 0'ed and not commented out
      if (!$poundifed && !$commented) {
        my $found = undef;
        foreach my $main (@$marray) {
          if (/\s+$main\s*\(/ || /^\s*$main\s*\(/) {
            ## If we've found a main, set the exename to the basename
            ## of the cpp file with the extension removed
            $name = $self->mpc_basename($file);
            $name =~ s/\.[^\.]+$//;
            $found = 1;
            last;
          }
          last if ($found);
        }
      }
    }
    close($fh);
  }
  return $name;
}


sub find_main_file {
  my($self, $sources) = @_;
  my $lang    = $self->get_language();
  my @main    = $language{$lang}->[3];
  my $preproc = $language{$lang}->[4];

  ## If additional main's have been supplied by the user for this
  ## language type, then just push them onto the array.
  push(@main, @{$mains{$lang}}) if (defined $mains{$lang});

  ## Now search each source file until we've found a main function.
  foreach my $file (@$sources) {
    my $exename = $self->search_for_entry($file, \@main, $preproc);
    return $exename if (defined $exename);
  }

  return undef;
}


sub generate_default_target_names {
  my $self = shift;

  ## If this is a custom_only project, we need not waste time setting the
  ## sharedname, staticname or exename.  Searching all of the files for a
  ## main function is very time consuming and unnecessary.
  return undef if ($self->get_assignment('custom_only'));

  if (!$self->exe_target()) {
    my $sharedname = $self->get_assignment('sharedname');
    my $staticname = $self->get_assignment('staticname');
    my $shared_empty;

    if (defined $sharedname) {
      if ($sharedname eq '') {
        $shared_empty = 1;
        $sharedname = undef;
        $self->process_assignment('sharedname', $sharedname);
      }
      elsif (!defined $staticname) {
        $staticname = $sharedname;
        $self->process_assignment('staticname', $staticname);
      }
    }
    if (defined $staticname && !$shared_empty && !defined $sharedname) {
      $sharedname = $staticname;
      $self->process_assignment('sharedname', $sharedname);
    }

    ## If it's neither an exe or library target, we will search
    ## through the source files for a main()
    if (!$self->lib_target()) {
      ## Set the exename assignment
      my @sources = $self->get_component_list('source_files', 1);
      my $exename = $self->find_main_file(\@sources);
      $self->process_assignment('exename', $exename) if (defined $exename);

      ## If we still don't have a project type, then we will
      ## default to a library if there are source or resource files
      if (!defined $exename) {
        if (!defined $sources[0]) {
          @sources = $self->get_component_list($self->get_resource_tag(), 1);
        }
        if (defined $sources[0]) {
          if (!$shared_empty) {
            $self->process_assignment('sharedname',
                                      $self->{'unmodified_project_name'});
          }
          $self->process_assignment('staticname',
                                    $self->{'unmodified_project_name'});
        }
      }
    }
  }

  ## If we are generating only static projects, then we need to
  ## unset the sharedname, so that we can insure that projects of
  ## various types only generate static targets.
  if ($self->get_static() == 1) {
    my $sharedname = $self->get_assignment('sharedname');
    if (defined $sharedname) {
      $self->process_assignment('sharedname', undef);
    }
  }

  ## Check for the use of an asterisk in the name
  foreach my $key ('exename', 'sharedname', 'staticname') {
    my $value = $self->get_assignment($key);
    if (defined $value && index($value, '*') >= 0) {
      $value = $self->fill_type_name($value,
                                     $self->{'unmodified_project_name'});
      $self->process_assignment($key, $value);
    }
  }
}


sub generate_default_pch_filenames {
  my($self, $files) = @_;
  my $pchhdef = (defined $self->get_assignment('pch_header'));
  my $pchcdef = (defined $self->get_assignment('pch_source'));

  if (!$pchhdef || !$pchcdef) {
    my $pname     = $self->get_assignment('project_name');
    my $hcount    = 0;
    my $ccount    = 0;
    my $hmatching;
    my $cmatching;
    foreach my $file (@$files) {
      ## If the file doesn't even contain _pch, then there's no point
      ## in looping through all of the extensions
      if (index($file, '_pch') >= 0) {
        if (!$pchhdef) {
          foreach my $ext (@{$self->{'valid_components'}->{'header_files'}}) {
            if ($file =~ /(.*_pch$ext)$/) {
              $self->process_assignment('pch_header', $1);
              ++$hcount;
              $hmatching = $file if (index($file, $pname) >= 0);
              last;
            }
          }
        }
        if (!$pchcdef) {
          foreach my $ext (@{$self->{'valid_components'}->{'source_files'}}) {
            if ($file =~ /(.*_pch$ext)$/) {
              $self->process_assignment('pch_source', $1);
              ++$ccount;
              $cmatching = $file if (index($file, $pname) >= 0);
              last;
            }
          }
        }
      }
    }
    if (!$pchhdef && $hcount > 1 && defined $hmatching) {
      $self->process_assignment('pch_header', $hmatching);
    }
    if (!$pchcdef && $ccount > 1 && defined $cmatching) {
      $self->process_assignment('pch_source', $cmatching);
    }
  }
}


sub fix_pch_filenames {
  my $self = shift;

  ## Unset the precompiled header settings if they are set but empty
  foreach my $type ('pch_header', 'pch_source') {
    my $pch = $self->get_assignment($type);
    $self->process_assignment($type, undef) if (defined $pch && $pch eq '');
  }
}


sub remove_extra_pch_listings {
  my $self = shift;
  my @pchs = ('pch_header', 'pch_source');
  my @tags = ('header_files', 'source_files');

  for(my $j = 0; $j < 2; ++$j) {
    my $pch = $self->get_assignment($pchs[$j]);

    if (defined $pch) {
      ## If we are converting slashes, then we need to
      ## convert the pch file back to forward slashes
      $pch =~ s/\\/\//g if ($self->{'convert_slashes'});

      ## Find out which files are duplicated
      my $names = $self->{$tags[$j]};
      foreach my $name (keys %$names) {
        my $comps = $$names{$name};
        foreach my $key (keys %$comps) {
          my $array = $$comps{$key};
          my $count = scalar(@$array);
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


sub sift_files {
  my($self, $files, $exts, $pchh, $pchc, $tag, $array, $alldir) = @_;
  my @saved;
  my $havec  = (defined $self->{'exclude_components'}->{$tag});

  ## The special actions taken based on $saverc only applies to
  ## C++ resource files.
  my $saverc = (!$alldir && $tag eq $self->get_resource_tag() &&
                $self->languageIs(Creator::cplusplus));

  foreach my $ext (@$exts) {
    foreach my $file (grep(/$ext$/, @$files)) {
      ## Always exclude the precompiled header and cpp
      if ((!defined $pchh || $file ne $pchh) &&
          (!defined $pchc || $file ne $pchc)) {
        if ($havec) {
          my $exclude = 0;
          foreach my $exc (@{$self->{'exclude_components'}->{$tag}}) {
            if ($file =~ /$exc$/) {
              $exclude = 1;
              last;
            }
          }
          next if ($exclude);
        }
        elsif ($saverc) {
          ## Save these files for later.  There may
          ## be more than one and we want to try and
          ## find the one that corresponds to this project
          push(@saved, $file);
          next;
        }

        push(@$array, $file) if (!$self->already_added($array, $file));
      }
    }
  }

  ## Now deal with the saved files
  if (defined $saved[0]) {
    if (!defined $saved[1]) {
      ## Theres only one rc file, take it
      push(@$array, $saved[0]);
    }
    else {
      my $pjname = $self->escape_regex_special(
                             $self->transform_file_name(
                                 $self->get_assignment('project_name')));
      ## Use a case insensitive search.
      ## After all, this is a Windows specific file type.
      foreach my $save (@saved) {
        if ($save =~ /$pjname/i) {
          if (!$self->already_added($array, $save)) {
            push(@$array, $save);
          }
        }
      }
    }
  }
}


sub sift_default_file_list {
  my($self, $tag, $file, $built, $exts, $recurse, $pchh, $pchc) = @_;
  my $alldir = $recurse ||
               (defined $self->{'flag_overrides'}->{$tag} &&
                defined $self->{'flag_overrides'}->{$tag}->{$file} &&
                $self->{'flag_overrides'}->{$tag}->{$file}->{'recurse'});
  my @gen    = $self->generate_default_file_list($file, [], undef, $alldir);

  $self->sift_files(\@gen, $exts, $pchh, $pchc, $tag, $built, $alldir);
}


sub correct_generated_files {
  my($self, $defcomp, $exts, $tag, $array) = @_;

  if (defined $sourceComponents{$tag}) {
    my $grtag = $grouped_key . $tag;
    foreach my $gentype (keys %{$self->{'generated_exts'}}) {
      ## If we are not automatically adding generated output, then we
      ## need to skip this component type.
      next if (!$self->{'generated_exts'}->{$gentype}->{'automatic_out'});

      ## If we are auto-generating the source_files, then
      ## we need to make sure that any generated source
      ## files that are added are put at the front of the list.
      my $newgroup;
      my @input;

      ## If I call keys %{$self->{$gentype}} using perl 5.6.1
      ## it returns nothing.  I have to put it in an
      ## intermediate variable to ensure that I get the keys.
      my $names = $self->{$gentype};
      foreach my $name (keys %$names) {
        foreach my $key (keys %{$$names{$name}}) {
          push(@input, @{$$names{$name}->{$key}});
          $newgroup = $key if ($key ne $defgroup);
        }
      }

      if (defined $input[0]) {
        my @front;
        my @copy = @$array;

        @$array = ();
        foreach my $input (@input) {
          my $part = $self->remove_wanted_extension(
                        $input,
                        $self->{'valid_components'}->{$gentype});

          my @files = $self->generated_filenames($part, $gentype,
                                                 $tag, $input);
          if (defined $copy[0]) {
            my $found = 0;
            foreach my $file (@files) {
              for(my $i = 0; $i < scalar(@copy); $i++) {
                my $re = $self->escape_regex_special($copy[$i]);
                if ($file eq $copy[$i] || $file =~ /[\/\\]$re$/) {
                  ## No need to check for previously added files
                  ## here since there are none.
                  $found = 1;
                  push(@front, $file);
                  splice(@copy, $i, 1);
                  last;
                }
              }
              last if ($found);
            }
            if (!$found) {
              ## The first file listed in @files is the preferred
              ## extension for the custom command.  Take the first
              ## file extension and see if it matches one in the accepted
              ## extensions.
              if (defined $files[0]) {
                my $ext;
                if ($files[0] =~ /.*(\.[^\.]+)$/) {
                  $ext = $self->escape_regex_special($1);
                }
                if (defined $ext) {
                  ## If it doesn't match one of the accepted extensions,
                  ## then just use the first extension from the type for
                  ## which we are generating.
                  $ext = $$exts[0] if (!StringProcessor::fgrep($ext, $exts));
                }

                ## Add all the files that match the chosen extension
                foreach my $file (@files) {
                  push(@front, $file) if ($file =~ /$ext$/);
                }
              }
            }
          }
          else {
            my $ext = $$exts[0];
            foreach my $file (@files) {
              push(@front, $file) if ($file =~ /$ext$/);
            }
          }
        }
        if (defined $copy[0]) {
          ## No need to check for previously added files
          ## here since there are none.
          push(@$array, @copy);
          if (defined $self->get_assignment($grtag)) {
            $self->process_assignment_add($grtag, $defgroup);
          }
        }
        if (defined $front[0]) {
          if (defined $newgroup) {
            if (defined $copy[0]) {
              $self->process_assignment_add($grtag, $defgroup);
            }
            if (!defined $self->{$tag}->{$defcomp}->{$newgroup}) {
              $self->{$tag}->{$defcomp}->{$newgroup} = \@front;
            }
            else {
              push(@{$self->{$tag}->{$defcomp}->{$newgroup}}, @front);
            }
            $self->process_assignment_add($grtag, $newgroup);
          }
          else {
            unshift(@$array, @front);
          }
        }
      }
    }
  }
}


sub generate_default_components {
  my($self, $files, $passed) = @_;
  my $genext   = $self->{'generated_exts'};
  my @gc       = reverse sort { $self->sort_generated_types($a, $b)
                              } keys %$genext;
  my @tags     = (defined $passed ? $passed :
                    (@gc, keys %{$language{$self->get_language()}->[0]}));
  my $pchh     = $self->get_assignment('pch_header');
  my $pchc     = $self->get_assignment('pch_source');
  my $recurse  = $self->get_assignment('recurse');
  my $defcomp  = $self->get_default_component_name();
  my $flo      = $self->{'flag_overrides'};
  my $cmdflags = 'commandflags';

  ## The order of @tags does make a difference in the way that generated
  ## files get added.  Hence the sort call on the generate_exts keys to
  ## ensure that user defined types come first.  They are reverse sorted
  ## using the custom sort function to ensure that user defined types
  ## that rely on other user defined types for input files are processed
  ## first.
  foreach my $tag (@tags) {
    if (!defined $genext->{$tag} ||
        $genext->{$tag}->{'automatic_in'}) {
      my $exts = $self->{'valid_components'}->{$tag};
      if (defined $$exts[0]) {
        if (defined $self->{$tag}) {
          ## If the tag is defined, then process directories
          my $names = $self->{$tag};
          foreach my $name (keys %$names) {
            my $comps = $$names{$name};
            foreach my $comp (keys %$comps) {
              my $array = $$comps{$comp};
              if (defined $passed) {
                $self->sift_files($files, $exts, $pchh, $pchc, $tag, $array);
              }
              else {
                my @built;
                my $alldirs = 1;
                foreach my $file (@$array) {
                  if (-d $file) {
                    my @portion;
                    $self->sift_default_file_list($tag, $file, \@portion,
                                                  $exts, $recurse, $pchh, $pchc);

                    ## Since the file was actually a directory, we will
                    ## need to propagate the flag overrides (if there are
                    ## any) to the newly located files.
                    if (defined $flo->{$tag} &&
                        defined $flo->{$tag}->{$file}) {
                      foreach my $built (@portion) {
                        $flo->{$tag}->{$built} = $flo->{$tag}->{$file};
                      }
                    }

                    ## Always push the @portion array onto the back of
                    ## @built.
                    push(@built, @portion);
                  }
                  else {
                    $alldirs = undef;
                    if (!$self->already_added(\@built, $file)) {
                      push(@built, $file);
                    }
                  }
                }
                if ($alldirs) {
                  $self->correct_generated_files($defcomp, $exts,
                                                 $tag, \@built);
                }
                $$comps{$comp} = \@built;
              }
            }
          }
        }
        else {
          ## Generate default values for undefined tags
          $self->{$tag} = {};
          my $comps = {};
          $self->{$tag}->{$defcomp} = $comps;
          $$comps{$defgroup} = [];
          my $array = $$comps{$defgroup};

          $self->{'defaulted'}->{$tag} = 1;

          if (!defined $specialComponents{$tag}) {
            $self->sift_files($files, $exts, $pchh, $pchc, $tag, $array);
            $self->correct_generated_files($defcomp, $exts, $tag, $array);
          }
        }

        ## If the type that we're generating defaults for ($tag) is a
        ## custom type, then we need to see if other custom types
        ## ($gentype) will generate files that will be used as input.  It
        ## has to be done here so that the built-in types will have all
        ## of the possible input files that they can.
        if (defined $genext->{$tag}) {
          foreach my $gentype (keys %{$genext}) {
            if ($gentype ne $tag) {
              $self->list_default_generated($gentype, [$tag]);
            }
          }

          ## Now that we have the files for this type ($tag), we need to
          ## locate a command helper for the custom command and see if it
          ## knows about any additional output files based on the file
          ## name.
          my $cmdHelper = CommandHelper::get($tag);
          if (defined $cmdHelper) {
            my $names = $self->{$tag};
            foreach my $name (keys %$names) {
              my $comps = $$names{$name};
              foreach my $comp (keys %$comps) {
                my $array = $$comps{$comp};
                foreach my $file (@$array) {
                  my $flags = defined $flo->{$tag}->{$file} ?
                                $flo->{$tag}->{$file}->{$cmdflags} :
                                $genext->{$tag}->{$cmdflags};
                  my $add_out = $cmdHelper->get_output($file, $flags);
                  push(@{$self->{'custom_special_output'}->{$tag}->{$file}},
                       @$add_out);
                }
              }
            }
          }
        }
      }
    }
  }
}


sub remove_duplicated_files {
  my($self, $dest, $source) = @_;
  my @slist = $self->get_component_list($source, 1);

  ## There's no point in going on if there's nothing in this component
  ## list.
  return undef if ($#slist == -1);

  ## Convert the array into keys for a hash table
  my %shash;
  @shash{@slist} = ();

  ## Find out which source files are listed
  my $names = $self->{$dest};
  foreach my $name (keys %$names) {
    foreach my $key (keys %{$$names{$name}}) {
      my $array = $$names{$name}->{$key};
      my $count = scalar(@$array);
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


sub generated_source_listed {
  my($self, $gent, $tag, $arr, $sext) = @_;
  my $names = $self->{$tag};

  ## Find out which generated source files are listed
  foreach my $name (keys %$names) {
    my $comps = $$names{$name};
    foreach my $key (keys %$comps) {
      foreach my $val (@{$$comps{$key}}) {
        foreach my $i (keys %$arr) {
          ## If $gent doesn't cause $tag files to be generated, then we
          ## can just return a non-zero value to short-circuit attempting
          ## to add generated files after the caller continues.
          my @gfiles = $self->generated_filenames($$arr{$i}, $gent, $tag, $i);
          return 2 if ($#gfiles == -1);

          foreach my $re (@gfiles) {
            $re = $self->escape_regex_special($re);
            return 1 if ($val =~ /$re$/);
          }
        }
      }
    }
  }

  return 0;
}


sub list_default_generated {
  my($self, $gentype, $tags) = @_;

  ## This method is called when the user has custom input files and has
  ## provided source files.  If the user defaults the component (i.e.
  ## source_files, resource_files, etc.) they are filled in by the
  ## generate_default_components method.

  if (defined $self->{'generated_exts'}->{$gentype} &&
      $self->{'generated_exts'}->{$gentype}->{'automatic_out'}) {
    ## After all source and headers have been defaulted, see if we
    ## need to add the generated files
    if (defined $self->{$gentype}) {
      ## Build up the list of files
      my %arr;
      my $names = $self->{$gentype};
      my $group;
      foreach my $name (keys %$names) {
        foreach my $key (keys %{$$names{$name}}) {
          my $array = $$names{$name}->{$key};

          ## Take the last group name we encounter
          $group = $key if ($key ne $defgroup);

          foreach my $val (@$array) {
            $arr{$val} = $self->remove_wanted_extension(
                              $val,
                              $self->{'valid_components'}->{$gentype});
          }
        }
      }

      foreach my $type (@$tags) {
        ## Do not add generated files if they are "special"
        ## unless they haven't been explicitly supplied.
        if ($gentype ne $type &&
            (!$specialComponents{$type} ||
             (!$self->{'special_supplied'}->{$type} ||
              UNIVERSAL::isa($self->{'special_supplied'}->{$type}, 'ARRAY')))) {
          if (!$self->generated_source_listed(
                                $gentype, $type, \%arr,
                                $self->{'valid_components'}->{$gentype})) {
            $self->add_generated_files($gentype, $type, $group, \%arr);
          }
        }
      }
    }
  }
}


sub prepend_gendir {
  my($self, $created, $ofile, $gentype) = @_;
  my $key;

  if (defined $self->{'flag_overrides'}->{$gentype}) {
    foreach my $ext (@{$self->{'valid_components'}->{$gentype}}) {
      my $e = $ext;
      $e =~ s/\\//g;
      $key = "$ofile$e";

      last if (defined $self->{'flag_overrides'}->{$gentype}->{$key});
      $key = undef;
    }

    if (defined $key) {
      if (StringProcessor::fgrep('gendir',
                                 $self->{'matching_assignments'}->{$gentype})) {
        my $dir = $self->{'flag_overrides'}->{$gentype}->{$key}->{'gendir'};
        if (defined $dir) {
          ## Convert the file to unix style for basename
          if ($self->{'convert_slashes'}) {
            $created =~ s/\\/\//g;
            $dir =~ s/\\/\//g;
          }
          return ($dir eq '.' ? '' : "$dir/") . $self->mpc_basename($created);
        }
      }
    }
  }

  return $created;
}


sub list_generated_file {
  my($self, $gentype, $tag, $array, $file, $ofile) = @_;
  my $count = 0;

  ## Go through each file listed in our original type and attempt to find
  ## out if it is the generated file we may need to add ($file).
  foreach my $gen ($self->get_component_list($gentype, 1)) {
    my $input = $gen;

    ## Take the file and see if it contains an extension that our
    ## generating type ($gentype) knows about.  If it does, remove it and
    ## stop looking for the extension.
    foreach my $ext (@{$self->{'valid_components'}->{$gentype}}) {
      ## Remove the extension.
      ## If it works, then we can exit this loop.
      last if ($gen =~ s/$ext$//);
    }

    ## If the user provided file does not match any of the
    ## extensions specified by the custom definition, we need
    ## to remove the extension or else this file will not be
    ## added to the project.
    $gen =~ s/\.[^\.]+$// if ($gen eq $input);

    ## See if we need to add the file.  We always need to check since the
    ## output file may have absolutely nothing in common with the input
    ## file.
    foreach my $created ($self->generated_filenames($gen, $gentype,
                                                    $tag, $input)) {
      ## $gen is a file that has a custom definition that generates
      ## files of the type $tag.  The $file passed in is of type
      ## $gentype and, as far as I can tell, $created will always be
      ## longer or of the same length of $file.  It doesn't really
      ## matter if $file contains a '.' or not.
      if (index($created, $file) != -1) {
        if (defined $ofile) {
          $created = $self->prepend_gendir($created, $ofile, $gentype);
        }
        if (!$self->already_added($array, $created)) {
          push(@$array, $created);
          ++$count;
        }
        last;
      }
    }
  }

  return $count;
}


sub add_corresponding_component_files {
  my($self, $filecomp, $tag) = @_;
  my $grname   = $grouped_key . $tag;

  ## Create a hash array keyed off of the existing files of the type
  ## that we plan on adding.
  my $fexist  = 0;
  my %scfiles;
  my $names   = $self->{$tag};
  foreach my $name (keys %$names) {
    ## Check to see if files exist in the default group
    if (defined $$names{$name}->{$defgroup} &&
        defined $$names{$name}->{$defgroup}->[0]) {
      $fexist = 1;
    }
    foreach my $comp (keys %{$$names{$name}}) {
      @scfiles{@{$$names{$name}->{$comp}}} = ();
    }
  }

  ## Create an array of extensions for the files we want to add
  my @exts;
  foreach my $ext (@{$self->{'valid_components'}->{$tag}}) {
    push(@exts, $ext);
    $exts[$#exts] =~ s/\\//g;
  }

  ## Check each file against a possible new file addition
  my $adddefaultgroup = 0;
  my $oktoadddefault  = 0;
  foreach my $sfile (keys %$filecomp) {
    my $found = 0;
    foreach my $ext (@exts) {
      if (exists $scfiles{"$sfile$ext"}) {
        $found = 1;
        last;
      }
    }

    if (!$found) {
      ## Get the array of files for the selected component name
      my $array = [];
      my $comp  = $$filecomp{$sfile};
      foreach my $name (keys %$names) {
        if (defined $$names{$name}->{$comp}) {
          $array = $$names{$name}->{$comp};
        }
      }

      ## First, see if it will be generated so that we can correctly
      ## deal with 'gendir' settings.
      foreach my $gentype (keys %{$self->{'generated_exts'}}) {
        $found += $self->list_generated_file($gentype, $tag, $array, $sfile);
      }

      ## Next check to see if the file exists
      if (!$found) {
        foreach my $ext (@exts) {
          if (-r "$sfile$ext") {
            my $file = "$sfile$ext";
            if (!$self->already_added($array, $file)) {
              push(@$array, $file);
              ++$found;
            }
            last;
          }
        }
      }

      ## If we have any files at all in the component array, check
      ## to see if we need to add a new group name
      if (defined $$array[0]) {
        if ($comp eq $defgroup) {
          $adddefaultgroup = 1;
        }
        else {
          my $grval = $self->get_assignment($grname);
          if (!defined $grval ||
              !StringProcessor::fgrep($comp, $self->create_array($grval))) {
            $self->process_assignment_add($grname, $comp);
          }
          $oktoadddefault = 1;
          $adddefaultgroup |= $fexist;
        }

        ## Put the array back into the component list
        if ($found) {
          foreach my $name (keys %$names) {
            $$names{$name}->{$comp} = $array;
          }
        }
      }
    }
  }

  ## We only need to add the default group name if we wanted to
  ## add the default group when adding new files and we added a group
  ## by some other name.  Otherwise, defaulted files would always be
  ## in a group, which is not what we want.
  if ($adddefaultgroup && $oktoadddefault) {
    $self->process_assignment_add($grname, $defgroup);
  }
}


sub get_default_project_name {
  my $self = shift;
  my $name = $self->{'current_input'};

  if ($name eq '') {
    $name = $self->transform_file_name($self->base_directory());
  }
  else {
    ## Since files on UNIX can have back slashes, we transform them
    ## into underscores.
    $name =~ s/\\/_/g;

    ## Convert the name to a usable name
    $name = $self->transform_file_name($name);

    ## Take off the extension
    $name =~ s/\.[^\.]+$//;
  }

  return $name;
}


sub remove_excluded {
  my $self = shift;
  my @tags = @_;

  ## Process each file type and remove the excluded files
  foreach my $tag (@tags) {
    my $names = $self->{$tag};
    foreach my $name (keys %$names) {
      foreach my $comp (keys %{$$names{$name}}) {
        my $count = scalar(@{$$names{$name}->{$comp}});
        for(my $i = 0; $i < $count; ++$i) {
          my $file = $$names{$name}->{$comp}->[$i];
          if (defined $self->{'remove_files'}->{$tag}->{$file}) {
            splice(@{$$names{$name}->{$comp}}, $i, 1);
            --$i;
            --$count;
          }
          else {
            ## The file does not match exactly with one of the files to
            ## remove.  Look for wildcard specifications in the files to
            ## be removed and perform the removal if one of them matches
            ## the current file.
            foreach my $key (keys %{$self->{'remove_files'}->{$tag}}) {
              if ($key =~ /[\*\?\[\]]/) {
                my $regex = $key;
                $regex =~ s/\./\\./g;
                $regex =~ s/\*/\.\*/g;
                $regex =~ s/\?/\./g;
                if ($file =~ /^$regex$/) {
                  splice(@{$$names{$name}->{$comp}}, $i, 1);
                  --$i;
                  --$count;
                  last;
                }
              }
            }
          }
        }
      }
    }
    delete $self->{'remove_files'}->{$tag};
  }
}


sub sort_generated_types {
  ## We need to sort the custom component types such that a custom type
  ## that generates input for another custom type comes first in the
  ## list.
  my($self, $left, $right, $norecurse) = @_;
  foreach my $key (keys %{$self->{'generated_exts'}->{$left}}) {
    if ($key =~ /_files$/) {
      foreach my $regex (@{$self->{'generated_exts'}->{$left}->{$key}}) {
        my $ext = $regex;
        $ext =~ s/\\//g;
        foreach my $vreg (@{$self->{'valid_components'}->{$right}}) {
          return -1 if ($ext =~ /$vreg$/);
        }
      }
    }
  }
  if (!$norecurse && $self->sort_generated_types($right, $left, 1) == -1) {
    return 1;
  }

  return 0;
}

sub generate_defaults {
  my $self = shift;

  ## Generate default project name
  if (!defined $self->get_assignment('project_name')) {
    $self->set_project_name($self->get_default_project_name());
  }

  ## Generate the default pch file names (if needed)
  my @files = $self->generate_default_file_list(
                                '.', [],
                                undef, $self->get_assignment('recurse'));
  $self->generate_default_pch_filenames(\@files);

  ## If the pch file names are empty strings then we need to fix that
  $self->fix_pch_filenames();

  ## Generate default components, but %specialComponents
  ## are skipped in the initial default components generation
  $self->generate_default_components(\@files);

  ## Remove source files that are also listed in the template files
  ## If we do not do this, then generated projects can be invalid.
  $self->remove_duplicated_files('source_files', 'template_files');

  ## If pch files are listed in header_files or source_files more than
  ## once, we need to remove the extras
  $self->remove_extra_pch_listings();

  ## Generate the default generated list of files only if we defaulted
  ## the generated file list.  I want to ensure that source_files comes
  ## first in the list to pick up group information (since source_files
  ## are most likely going to be grouped than anything else).
  my @vc = sort { return -1 if $a eq 'source_files';
                  return  1 if $b eq 'source_files';
                  return $b cmp $a; } keys %{$self->{'valid_components'}};
  my @gvc = sort { $self->sort_generated_types($a, $b)
                 } keys %{$self->{'generated_exts'}};
  foreach my $gentype (@gvc) {
    $self->list_default_generated($gentype, \@vc);
  }

  ## Now that all of the source files have been added
  ## we need to remove those that have need to be removed
  $self->remove_excluded('source_files');

  ## Collect up all of the source files that have already been listed
  ## with the extension removed for use directly below.
  my %sourcecomp;
  foreach my $sourcetag (keys %sourceComponents) {
    my $names = $self->{$sourcetag};
    foreach my $name (keys %$names) {
      foreach my $comp (keys %{$$names{$name}}) {
        foreach my $sfile (@{$$names{$name}->{$comp}}) {
          my $mod = $sfile;
          $mod =~ s/\.[^\.]+$//;
          $sourcecomp{$mod} = $comp;
        }
      }
    }
  }

  ## Add %specialComponents files based on the
  ## source_components (i.e. .h and .i or .inl based on .cpp)
  foreach my $tag (keys %specialComponents) {
    $self->add_corresponding_component_files(\%sourcecomp, $tag);
  }

  ## Now, if the %specialComponents are still empty
  ## then take any file that matches the components extension
  foreach my $tag (keys %specialComponents) {
    if (!$self->{'special_supplied'}->{$tag} ||
        UNIVERSAL::isa($self->{'special_supplied'}->{$tag}, 'ARRAY')) {
      my $names = $self->{$tag};
      if (defined $names) {
        ## We only want to generate default components if we have
        ## defaulted the source files or we have no files listed
        ## in the current special component.
        my $ok = $self->{'defaulted'}->{'source_files'};
        if (!$ok) {
          my @all;
          foreach my $name (keys %$names) {
            foreach my $key (keys %{$$names{$name}}) {
              push(@all, @{$$names{$name}->{$key}});
            }
          }
          $ok = (!defined $all[0]);
        }
        if ($ok) {
          ## If the "special" type was supplied and it was all
          ## directories, we need to use those directories to generate
          ## the default components instead of the current directory.
          my $fileref = \@files;
          if (defined $self->{'special_supplied'}->{$tag} &&
              UNIVERSAL::isa($self->{'special_supplied'}->{$tag}, 'ARRAY')) {
            my @special;
            foreach my $dir (@{$self->{'special_supplied'}->{$tag}}) {
              push(@special, $self->generate_default_file_list(
                                         $dir, [], undef,
                                         $self->get_assignment('recurse')));
            }
            $fileref = \@special;
          }
          $self->generate_default_components($fileref, $tag);
        }
      }
    }
  }

  ## Now that all of the other files have been added
  ## we need to remove those that have need to be removed
  my @rmkeys = keys %{$self->{'remove_files'}};
  $self->remove_excluded(@rmkeys) if (defined $rmkeys[0]);

  ## Tie custom files together if need be.  This currently only applies
  ## to types with command helpers.  At some point, if it is found to be
  ## desirous, we could extend the MPC syntax somehow to support this
  ## sort of thing manually.
  my $dep = 'dependent';
  foreach my $gentype (@gvc) {
    my $cmdHelper = CommandHelper::get($gentype);
    if (defined $cmdHelper) {
      ## There has to be at least two files files in order for
      ## something to be tied together.
      my @files = $self->get_component_list($gentype, 1);
      if ($#files >= 1) {
        foreach my $file (@files) {
          my $part = $self->remove_wanted_extension(
                            $file, $self->{'valid_components'}->{$gentype});
          my($tied, $vc) = $cmdHelper->get_tied($file, \@files);
          foreach my $tie (@$tied) {
            my @gen;
            if (!defined $vc) {
              foreach $vc (@vc) {
                @gen = $self->generated_filenames($part, $gentype,
                                                  $vc, $file);
                last if ($#gen >= 0);
              }
            }

            ## We have a tied file, now we need to actually perform
            ## the tieing of the two.  We will do this by saying that
            ## the output of the original is necessary for the
            ## processing of the tied file.
            @gen = $self->generated_filenames($part, $gentype,
                                              $vc, $file) if (!$gen[0]);

            ## We have found a set of files that are generated
            ## based on the component type of the original file
            ## ($gentype), so we just add the first one and
            ## we're done.
            my $first = $gen[0];
            $self->{'flag_overrides'}->{$gentype}->{$tie}->{$dep} =
              $self->{'generated_exts'}->{$gentype}->{$dep}
              if (!defined $self->{'flag_overrides'}->{$gentype}->{$tie}->{$dep});

            $self->{'flag_overrides'}->{$gentype}->{$tie}->{$dep} .= " $first"
            if (!defined $self->{'flag_overrides'}->{$gentype}->{$tie}->{$dep} ||
                $self->{'flag_overrides'}->{$gentype}->{$tie}->{$dep} !~ /\b$first\b/);
          }
        }
      }
    }
  }
}


sub set_project_name {
  my($self, $name) = @_;

  ## Save the unmodified project name so that when we
  ## need to determine the default target name, we can use
  ## what is expected by the user.
  $self->{'unmodified_project_name'} = $name;

  ## If we are applying the name modifier to the project
  ## then we will modify the project name
  if ($self->get_apply_project()) {
    my $nmod = $self->get_name_modifier();

    if (defined $nmod) {
      $nmod =~ s/\*/$name/g;
      $name = $nmod;
    }
  }

  ## Set the project_name assignment so that the TemplateParser
  ## can get the project name.
  $self->process_assignment('project_name', $name);
}


sub project_name {
  return $_[0]->get_assignment('project_name');
}


sub lib_target {
  my $self = shift;
  return (defined $self->get_assignment('sharedname') ||
          defined $self->get_assignment('staticname'));
}


sub exe_target {
  return (defined $_[0]->get_assignment('exename'));
}


sub get_component_list {
  my($self, $tag, $noconvert) = @_;
  my $names = $self->{$tag};
  my @list;

  foreach my $name (keys %$names) {
    foreach my $key (keys %{$$names{$name}}) {
      push(@list, @{$$names{$name}->{$key}});
    }
  }

  ## By default, if 'convert_slashes' is true, then we convert slashes
  ## to backslashes.  There are cases where we do not want to convert
  ## the slashes, in that case get_component_list() was called with
  ## an additional parameter indicating this.
  if (!$noconvert && $self->{'convert_slashes'}) {
    foreach my $item (@list) {
      $item =~ s/\//\\/g;
    }
  }

  if ($self->{'sort_files'}) {
    @list = sort { $self->file_sorter($a, $b) } @list;
  }

  return @list;
}


sub check_custom_output {
  my($self, $based, $cinput, $ainput, $type, $comps) = @_;
  my @outputs;

  foreach my $array ($self->generated_filenames($cinput, $based,
                                                $type, $ainput, 0, 1)) {
    foreach my $built (@$array) {
      if (@$comps == 0) {
        push(@outputs, $built);
        last;
      }
      elsif (defined $specialComponents{$type} &&
             (!$self->{'special_supplied'}->{$type} ||
              UNIVERSAL::isa($self->{'special_supplied'}->{$type}, 'ARRAY'))) {
        push(@outputs, $built);
        last;
      }
      else {
        my $base = $built;
        $base =~ s/\\/\//g if ($self->{'convert_slashes'});
        my $re = $self->escape_regex_special($self->mpc_basename($base));
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

  return @outputs;
}


sub get_special_value {
  my $self   = shift;
  my $type   = shift;
  my $cmd    = shift;
  my $based  = shift;
  my @params = @_;

  ## These names (held in $type) are variables that contain various
  ## commands that will be used in templates within the context of a
  ## foreach (e.g., <%custom_type->input_files%> or <%feature->value%>).
  if ($type eq 'feature') {
    return $self->get_feature_value($cmd, $based);
  }
  elsif (index($type, 'custom_type') == 0) {
    return $self->get_custom_value($cmd, $based, @params);
  }
  elsif (index($type, $grouped_key) == 0) {
    return $self->get_grouped_value($type, $cmd, $based);
  }

  return undef;
}


sub get_feature_value {
  my($self, $cmd, $based) = @_;

  if ($cmd eq 'value') {
    my $val = $self->{'feature_parser'}->get_value($based);
    if (defined $val && $val != 0) {
      return 1;
    }
  }

  return undef;
}


sub get_grouped_value {
  my($self, $type, $cmd, $based) = @_;
  my $value;

  ## Make it all lower case
  $type = lc($type);

  ## Remove the grouped_ part
  $type =~ s/^$grouped_key//;

  ## Add the s if it isn't there
  $type .= 's' if ($type !~ /s$/);

  my $names = $self->{$type};
  if ($cmd eq 'files') {
    foreach my $name (keys %$names) {
      my $comps = $$names{$name};
      my @keys = keys %$comps;
      if (StringProcessor::fgrep($based, \@keys)) {
        if ($self->{'convert_slashes'}) {
          my @converted;
          foreach my $file (@{$$comps{$based}}) {
            push(@converted, $self->slash_to_backslash($file));
          }
          $value = \@converted;
        }
        else {
          $value = $$comps{$based};
        }
        if ($self->{'sort_files'}) {
          my @sorted = sort { $self->file_sorter($a, $b) } @$value;
          $value = \@sorted;
        }
      }
    }
  }
  elsif ($cmd eq 'component_name') {
    ## If there is more than one name, then we will need
    ## to deal with that at a later time.
    foreach my $name (keys %$names) {
      $value = $name;
    }
  }

  return $value;
}


sub get_command_subs {
  my $self = shift;
  my %valid;

  ## Add the built-in OS compatibility commands
  if (UNIVERSAL::isa($self, 'WinProjectBase') ||
      $self->use_win_compatibility_commands()) {
    $valid{'cat'}   = 'type';
    $valid{'cmp'}   = 'fc';
    $valid{'cp'}    = 'copy /y';
    $valid{'mkdir'} = 'mkdir';
    $valid{'mv'}    = 'move /y';
    $valid{'os'}    = 'win32';
    $valid{'rm'}    = 'del /f/s/q';
    $valid{'rmdir'} = 'rmdir /s/q';
    $valid{'nul'}   = 'nul';
    $valid{'slash'} = '\\';
    $valid{'bat'}   = '.bat';
    $valid{'cmd'}   = '.cmd';
    $valid{'exe'}   = '.exe';
  }
  else {
    $valid{'cat'}   = 'cat';
    $valid{'cmp'}   = 'cmp';
    $valid{'cp'}    = 'cp -f';
    $valid{'mkdir'} = 'mkdir -p';
    $valid{'mv'}    = 'mv -f';
    $valid{'os'}    = 'unix';
    $valid{'rm'}    = 'rm -rf';
    $valid{'rmdir'} = 'rm -rf';
    $valid{'nul'}   = '/dev/null';
    $valid{'slash'} = '/';
    $valid{'bat'}   = '';
    $valid{'cmd'}   = '';
    $valid{'exe'}   = '';
  }

  ## Add the project specific compatibility commands
  $valid{'gt'}        = $self->get_gt_symbol();
  $valid{'lt'}        = $self->get_lt_symbol();
  $valid{'and'}       = $self->get_and_symbol();
  $valid{'or'}        = $self->get_or_symbol();
  $valid{'quote'}     = $self->get_quote_symbol();
  $valid{'equote'}    = $self->get_escaped_quote_symbol();
  $valid{'crlf'}      = $self->crlf();
  $valid{'cmdsep'}    = $self->get_cmdsep_symbol();
  $valid{'temporary'} = 'temp.$$$$.' . int(rand(0xffffffff));
  $valid{'prj_type'}  = $self->{'pctype'};

  return \%valid;
}


sub replace_parameters {
  my($self, $str, $valid, $nowarn, $input, $output, $always_clear) = @_;

  my %saved;
  my $count = 0;
  while ($str =~ /<%(\w+)(\(\w+\))?%>/) {
    my $name     = $1;
    my $modifier = $2;
    if (defined $modifier) {
      my $tmp = $name;
      $name = $modifier;
      $name =~ s/[\(\)]//g;
      $modifier = $tmp;
    }

    ## Support both pseudo variables and project settings
    if (defined $$valid{$name} || $self->is_keyword($name)) {
      ## If the pseudo variable is defined or the project setting has a
      ## value, then we'll need to do the replacement.  However, if it's
      ## a project keyword and it's not defined, we will need to delay
      ## the replacement until later (unless $always_clear is true).
      my $replace;
      my $clear = $always_clear;
      if (defined $$valid{$name}) {
        $replace = $$valid{$name};
      }
      elsif ($self->is_keyword($name)) {
        $replace = $self->get_assignment($name);
      }

      ## Perform the modification and replacement here
      if (defined $replace) {
        if (defined $modifier) {
          if ($modifier eq 'noextension') {
            $replace =~ s/\.[^\.]+$//;
          }
          else {
            $self->warning("Unknown parameter modifier $modifier.");
          }
        }
        $str =~ s/<%\w+(\(\w+\))?%>/$replace/;
      }
      elsif ($clear) {
        ## We need to clear out this variable usage.
        $str =~ s/<%\w+(\(\w+\))?%>//;
      }
      else {
        ## Save this variable usage to be put back after we're done
        ## processing the string.
        my $key = "\1" . $count++ . "\1";
        if ($str =~ s/(<%\w+(\(\w+\))?%>)/$key/) {
          $saved{$key} = $1;
        }
      }
    }
    else {
      $str =~ s/<%\w+(\(\w+\))?%>//;

      ## We only want to warn the user that we did not recognize the
      ## pseudo template parameter if there was an input and an output
      ## file passed to this function.  If this variable was used
      ## without the parenthesis (as in an if statement), then we don't
      ## want to warn the user.
      if (defined $input && defined $output) {
        if (!defined $$nowarn{$name}) {
          $self->warning("<%$name%> was not recognized.");
        }

        ## If we didn't recognize the pseudo template parameter then
        ## we don't want to return anything back.
        return undef;
      }
    }
  }

  ## Replace the saved variables so that they may be replaced (or
  ## removed) later on.
  foreach my $key (keys %saved) {
    $str =~ s/$key/$saved{$key}/;
  }
  return $str;
}


sub convert_command_parameters {
  my($self, $ktype, $str, $input, $output) = @_;
  my %nowarn;
  my %valid = %{$self->{'command_subs'}};

  ## Add in the values that change for every call to this function
  $valid{'temporary'} = 'temp.$$$$.' . int(rand(0xffffffff));

  if (defined $input) {
    $valid{'input'}          = $input;
    $valid{'input_basename'} = $self->mpc_basename($input);
    $valid{'input_dirname'}  = $self->mpc_dirname($input);
    $valid{'input_noext'}    = $input;

    ## An input file doesn't always have an extension.  If there isn't
    ## one, then we need to set the 'input_ext' field to an empty string
    ## ($1 will not necessarily have a valid value).
    if ($valid{'input_noext'} =~ s/(\.[^\.]+)$//) {
      $valid{'input_ext'} = $1;
    }
    else {
      $valid{'input_ext'} = '';
    }

    ## Check for the gendir setting associated with this input file.  We
    ## have to check at so many levels so we don't inadvertantly create
    ## intermediate hash tables.
    if (defined $self->{'flag_overrides'}->{$ktype} &&
        defined $self->{'flag_overrides'}->{$ktype}->{$input} &&
        $self->{'flag_overrides'}->{$ktype}->{$input}->{'gendir'}) {
      $valid{'gendir'} = $self->{'flag_overrides'}->{$ktype}->{$input}->{'gendir'};
    }
  }

  ## If there is no gendir setting, just set it to the current directory.
  $valid{'gendir'} = '.' if (!defined $valid{'gendir'});

  if (defined $output) {
    my $first = 1;
    $valid{'output'} = "@$output";
    foreach my $out (@$output) {
      ## An output file doesn't always have an extension.  If there isn't
      ## one, then we need to set the 'output_ext' field to an empty
      ## string ($1 will not necessarily have a valid value).
      my $noext = $out;
      if ($noext =~ s/(\.[^\.]+)$//) {
        $valid{'output_ext'} = $1;
      }
      else {
        $valid{'output_ext'} = '';
      }
      $valid{'output_noext'} .= (!$first ? ' ' : '') . $noext;

      ## In order to call basename or dirname, we must make sure that the
      ## directory separators are forward slashes.
      my $file = $out;
      $file =~ s/\\/\//g if ($self->{'convert_slashes'});
      $valid{'output_basename'} .= (!$first ? ' ' : '') .
                                   $self->mpc_basename($file);
      $valid{'output_dirname'}  .= (!$first ? ' ' : '') .
                                   $self->mpc_dirname($file);
      $first = 0;
    }
  }

  ## Add in the specific types of output files
  if (defined $output) {
    foreach my $type (keys %{$self->{'valid_components'}}) {
      my $key = $type;
      $key =~ s/s$//gi;
      $nowarn{$key} = 1;
      $nowarn{$key . '_noext'} = 1;
      foreach my $ext (@{$self->{'valid_components'}->{$type}}) {
        foreach my $out (@$output) {
          if ($out =~ /$ext$/) {
            $valid{$key} = $out;
            $valid{$key . '_noext'} = $out;
            $valid{$key . '_noext'} =~ s/$ext$//;
            last;
          }
        }
      }
    }
  }

  return $self->replace_parameters($str, \%valid, \%nowarn, $input, $output, 1);
}


sub get_custom_value {
  my $self   = shift;
  my $cmd    = shift;
  my $based  = shift;
  my @params = @_;
  my $value;

  if ($cmd eq 'input_files') {
    ## Get the component list for the component type
    my @array = $self->get_component_list($based);

    ## Check for directories in the component list.  If the component
    ## type is not automatic, we may have directories here and will need
    ## to get the file list for that type.
    my $once;
    for(my $i = 0; $i < scalar(@array); ++$i) {
      if (-d $array[$i]) {
        if (!defined $once) {
          $once = {'recurse' => $self->get_assignment('recurse'),
                   'pchh'    => $self->get_assignment('pch_header'),
                   'pchc'    => $self->get_assignment('pch_source'),
                  };
        }
        my @built;
        $self->sift_default_file_list($based, $array[$i], \@built,
                                      $self->{'valid_components'}->{$based},
                                      $$once{'recurse'},
                                      $$once{'pchh'}, $$once{'pchc'});
        splice(@array, $i, 1, @built);
        $i += $#built;
      }
    }

    $value = \@array;

    $self->{'custom_output_files'} = {};
    my %vcomps;
    foreach my $vc (keys %{$self->{'valid_components'}}) {
      my @comps = $self->get_component_list($vc);
      $vcomps{$vc} = \@comps;
    }
    $vcomps{$generic_key} = [];

    foreach my $input (@array) {
      my @outputs;
      my $ainput = $input;
      my $cinput = $input;

      ## Remove the extension
      $cinput =~ s/\.[^\.]+$//;

      ## If we are converting slashes,
      ## change them back for this parameter
      $ainput =~ s/\\/\//g if ($self->{'convert_slashes'});

      ## Add all of the output files.  We can not add $generic_key to the
      ## list here (as it used to be).  It may have been handled by
      ## generated_filenames.
      foreach my $vc (keys %{$self->{'valid_components'}}) {
        ## The output of multiple components could be input for the
        ## current component type ($based).  We need to avoid adding
        ## duplicates here.
        foreach my $file ($self->check_custom_output(
                            $based, $cinput, $ainput, $vc, $vcomps{$vc})) {
          push(@outputs, $file) if (!StringProcessor::fgrep($file, \@outputs));
        }
      }
      foreach my $file ($self->check_custom_output($based, $cinput,
                                                   $ainput, $generic_key,
                                                   $vcomps{$generic_key})) {
        push(@outputs, $file) if (!StringProcessor::fgrep($file, \@outputs));
      }

      ## Add specially listed files avoiding duplicates.  We don't want
      ## to add these files if gendir is set to something besides .
      if (defined $self->{'custom_special_output'}->{$based} &&
          defined $self->{'custom_special_output'}->{$based}->{$ainput} &&
          (!defined $self->{'flag_overrides'}->{$based} ||
           !defined $self->{'flag_overrides'}->{$based}->{$ainput} ||
           !defined $self->{'flag_overrides'}->{$based}->{$ainput}->{'gendir'} ||
           $self->{'flag_overrides'}->{$based}->{$ainput}->{'gendir'} eq '.')) {
        foreach my $file (@{$self->{'custom_special_output'}->{$based}->{$ainput}}) {
          push(@outputs, $file) if (!StringProcessor::fgrep($file, \@outputs));
        }
      }

      if ($self->{'convert_slashes'}) {
        foreach my $output (@outputs) {
          $output =~ s/\//\\/g;
        }
      }
      if ($self->{'sort_files'}) {
        @outputs = sort { $self->file_sorter($a, $b) } @outputs;
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
  elsif ($cmd eq 'source_output_files') {
    # Generate source output files based on $based
    if (defined $self->{'custom_output_files'}) {
      $value = [];
      foreach my $file (@{$self->{'custom_output_files'}->{$based}}) {
        foreach my $ext (@{$self->{'valid_components'}->{'source_files'}}) {
          if ($file =~ /$ext$/) {
            ## We've found a file that matches one of the source file
            ## extensions.  Now we have to make sure that it doesn't
            ## match a template file extension.
            my $matched = 0;
            foreach my $text (@{$self->{'valid_components'}->{'template_files'}}) {
              if ($file =~ /$text$/) {
                $matched = 1;
                last;
              }
            }
            push(@$value, $file) if (!$matched);
            last;
          }
        }
      }
    }
  }
  elsif ($cmd eq 'non_source_output_files') {
    # Generate non source output files based on $based
    if (defined $self->{'custom_output_files'}) {
      $value = [];
      foreach my $file (@{$self->{'custom_output_files'}->{$based}}) {
        my $source = 0;
        foreach my $ext (@{$self->{'valid_components'}->{'source_files'}}) {
          if ($file =~ /$ext$/) {
            $source = 1;
            ## We've found a file that matches one of the source file
            ## extensions.  Now we have to make sure that it doesn't
            ## match a template file extension.
            foreach my $text (@{$self->{'valid_components'}->{'template_files'}}) {
              if ($file =~ /$text$/) {
                $source = 0;
                last;
              }
            }
            last if ($source);
          }
        }
        push(@$value, $file) if (!$source);
      }
    }
  }
  elsif ($cmd eq 'inputexts') {
    my @array = @{$self->{'valid_components'}->{$based}};
    foreach my $val (@array) {
      $val =~ s/\\\.//g;
    }
    $value = \@array;
  }
  elsif ($cmd eq 'dependencies') {
    ## If we are converting slashes, change them back for this parameter
    $based =~ s/\\/\//g if ($self->{'convert_slashes'});
    $value = $self->{'custom_special_depend'}->{$based};
  }
  elsif (defined $customDefined{$cmd}) {
    $value = $self->get_assignment($cmd,
                                   $self->{'generated_exts'}->{$based});
    if (defined $value && ($customDefined{$cmd} & 0x14) != 0) {
      $value = $self->convert_command_parameters($based, $value, @params);
    }
  }

  return $value;
}


sub check_features {
  my($self, $requires, $avoids, $info) = @_;
  my $status = 1;
  my $why;

  if (defined $requires) {
    foreach my $require (split(/\s+/, $requires)) {
      my $fval = $self->{'feature_parser'}->get_value($require);

      ## By default, if the feature is not listed, then it is enabled.
      if (defined $fval && !$fval) {
        $why = "requires $require";
        $status = 0;
        last;
      }

      ## For automakes sake, if we're to this point the feature is
      ## enabled and we will set it in the feature parser explicitly
      if (!defined $fval) {
        $self->{'feature_parser'}->parse_line(undef, "$require = 1");
      }
    }
  }

  ## If it passes the requires, then check the avoids
  if ($status) {
    if (defined $avoids) {
      foreach my $avoid (split(/\s+/, $avoids)) {
        my $fval = $self->{'feature_parser'}->get_value($avoid);

        ## By default, if the feature is not listed, then it is enabled.
        if (!defined $fval || $fval) {
          $why = "avoids $avoid";
          $status = 0;
          last;
        }
      }
    }
  }

  if ($info && !$status) {
    $self->details("Skipping " . $self->get_assignment('project_name') .
                   " ($self->{'current_input'}), it $why.");
  }

  return $status;
}


sub need_to_write_project {
  my $self  = shift;
  my $count = 0;

  ## We always write a project if the user has provided a verbatim.
  ## We have no idea what that verbatim clause does, so we need to just
  ## do what the user tells us to do.
  return 1 if (defined $self->{'verbatim'}->{$self->{'pctype'}});

  ## The order here is important, we must check for source or resource
  ## files first and then for custom input files.
  foreach my $key ('source_files', $self->get_resource_tag(),
                   keys %{$self->{'generated_exts'}}) {
    my $names = $self->{$key};
    foreach my $name (keys %$names) {
      foreach my $key (keys %{$names->{$name}}) {
        ## See if the project contains a file that corresponds to this
        ## component name.
        if (defined $names->{$name}->{$key}->[0]) {
          if ($count >= 2) {
            ## Return 2 if we have found a custom input file (and thus no
            ## source or resource files due to the foreach order).
            return 2;
          }
          ## We have either source files or resource files, we need to
          ## see if this project creator supports the current language.
          ## If it doesn't then we don't need to create the project.
          elsif ($self->languageSupported()) {
            ## Return 1 if we have found a source file or a resource file.
            return 1;
          }
        }
      }
    }
    $count++;
  }

  ## Indicate that there is no need to write the project
  return 0;
}


sub write_output_file {
  my($self, $webapp) = @_;
  my $status = 0;
  my $error;
  my $tover = $self->get_template_override();
  my @templates = $self->get_template();

  ## The template override will override all templates
  @templates = ($tover) if (defined $tover);

  foreach my $template (@templates) {
    ## Save the template name for use as a key for various function calls
    $self->{'current_template'} = $template;

    ## Create the output file name based on the project name and the
    ## template that we're currently using.
    my $name = $self->transform_file_name(
                  $self->project_file_name(undef,
                                           $self->{'current_template'}));

    ## If the template files does not end in the template extension
    ## then we will add it on.
    if ($template !~ /$TemplateExtension$/) {
      $template .= '.' . $TemplateExtension;
    }

    ## If the template file does not contain a path, then we
    ## will search through the include paths for it.
    my $tfile;
    if ($template =~ /[\/\\]/i) {
      $tfile = $template;
    }
    else {
      $tfile = $self->search_include_path($template);
    }

    if (defined $tfile) {
      ## Read in the template values for the specific target and project
      ## type.  The template input file we get may depend upon the
      ## current template that we're using.
      ($status, $error) = $self->read_template_input(
                                   $self->{'current_template'});
      last if (!$status);

      my $tp = new TemplateParser($self);

      ## Set the project_file assignment for the template parser
      $self->process_assignment('project_file', $name);

      ($status, $error) = $tp->parse_file($tfile);
      last if (!$status);

      if (defined $self->{'source_callback'}) {
        my $cb     = $self->{'source_callback'};
        my $pjname = $self->get_assignment('project_name');
        my @list   = $self->get_component_list('source_files');
        if (UNIVERSAL::isa($cb, 'ARRAY')) {
          my @copy = @$cb;
          my $s = shift(@copy);
          &$s(@copy, $name, $pjname, \@list);
        }
        elsif (UNIVERSAL::isa($cb, 'CODE')) {
          &$cb($name, $pjname, \@list);
        }
        else {
          $self->warning("Ignoring callback: $cb.");
        }
      }

      if ($self->get_toplevel()) {
        my $outdir = $self->get_outdir();
        my $oname  = $name;

        $name = "$outdir/$name";

        my $fh  = new FileHandle();
        my $dir = $self->mpc_dirname($name);

        mkpath($dir, 0, 0777) if ($dir ne '.');

        if ($webapp) {
          ## At this point in time, webapps do not get a project file,
          ## but they do appear in the workspace
        }
        elsif ($self->compare_output()) {
          ## First write the output to a temporary file
          my $tmp = "$outdir/MPC$>.$$";
          my $different = 1;
          if (open($fh, ">$tmp")) {
            my $lines = $tp->get_lines();
            foreach my $line (@$lines) {
              print $fh $line;
            }
            close($fh);

            $different = 0 if (!$self->files_are_different($name, $tmp));
          }
          else {
            $error = "Unable to open $tmp for output.";
            $status = 0;
            last;
          }

          ## If they are different, then rename the temporary file
          if ($different) {
            unlink($name);
            if (rename($tmp, $name)) {
              $self->post_file_creation($name);
            }
            else {
              $error = "Unable to open $name for output.";
              $status = 0;
              last;
            }
          }
          else {
            ## We will pretend that we wrote the file
            unlink($tmp);
          }
        }
        else {
          if (open($fh, ">$name")) {
            my $lines = $tp->get_lines();
            foreach my $line (@$lines) {
              print $fh $line;
            }
            close($fh);
            $self->post_file_creation($name);
          }
          else {
            $error = "Unable to open $name for output.";
            $status = 0;
            last;
          }
        }

        ## There may be more than one template associated with this
        ## project creator.  If there is, we can only add one generated
        ## file and we rely on the project creator to tell us which
        ## template generates the file that we need to track.
        $self->add_file_written($oname)
                   if ($self->file_visible($self->{'current_template'}));
      }
    }
    else {
      $error = "Unable to locate the template file: $template.";
      $status = 0;
      last;
    }
  }
  return $status, $error;
}


sub write_install_file {
  my $self = shift;
  my $fh = new FileHandle();
  my $insfile = $self->transform_file_name(
                          $self->get_assignment('project_name')) .
                '.ins';
  my $outdir = $self->get_outdir();

  $insfile = "$outdir/$insfile";

  unlink($insfile);
  if (open($fh, ">$insfile")) {
    foreach my $vc (keys %{$self->{'valid_components'}}) {
      my $names = $self->{$vc};
      foreach my $name (keys %$names) {
        foreach my $key (keys %{$$names{$name}}) {
          my $array = $$names{$name}->{$key};
          if (defined $$array[0]) {
            print $fh "$vc:\n";
            foreach my $file (@$array) {
              print $fh "$file\n";
            }
            print $fh "\n";
          }
        }
      }
    }
    if ($self->exe_target()) {
      my $exeout = $self->get_assignment('exeout');
      print $fh "exe_output:\n",
                (defined $exeout ? $self->relative($exeout) : ''),
                ' ', $self->get_assignment('exename'), "\n";
    }
    elsif ($self->lib_target()) {
      my $shared = $self->get_assignment('sharedname');
      my $static = $self->get_assignment('staticname');
      my $dllout = $self->relative($self->get_assignment('dllout'));
      my $libout = $self->relative($self->get_assignment('libout'));

      print $fh "lib_output:\n";

      if (defined $shared && $shared ne '') {
        print $fh (defined $dllout ? $dllout : $libout), " $shared\n";
      }
      if ((defined $static && $static ne '') &&
          (defined $dllout || !defined $shared ||
               (defined $shared && $shared ne $static))) {
        print $fh "$libout $static\n";
      }
    }

    close($fh);
    return 1, undef;
  }

  return 0, 'Unable write to ' . $insfile;
}


sub write_project {
  my $self = shift;
  my $status = 2;
  my $error;
  my $progress = $self->get_progress_callback();

  &$progress() if (defined $progress);

  if ($self->check_features($self->get_assignment('requires'),
                            $self->get_assignment('avoids'),
                            1)) {
    my $webapp = $self->get_assignment('webapp');
    my $ntwp = $self->need_to_write_project();
    if ($webapp || $ntwp) {
      if ($webapp && !$self->webapp_supported()) {
        $self->warning("Web Applications are not supported by this type.");
      }
      else {
        ## A return value of 2 from need_to_write_project() indicates
        ## that the only reason that we need to write the project is that
        ## there are custom input files (i.e., no source or resource
        ## files).
        $self->process_assignment('custom_only', '1') if ($ntwp == 2);

        if ($self->get_assignment('custom_only')) {
          $self->remove_non_custom_settings();
        }

        if ($self->{'escape_spaces'}) {
          foreach my $name ('exename', 'sharedname', 'staticname',
                            'exeout', 'dllout', 'libout') {
            my $value = $self->get_assignment($name);
            if (defined $value && $value =~ s/(\s)/\\$1/g) {
              $self->process_assignment($name, $value);
            }
          }
          foreach my $key (keys %{$self->{'valid_components'}}) {
            my $names = $self->{$key};
            foreach my $name (keys %$names) {
              foreach my $key (keys %{$$names{$name}}) {
                foreach my $file (@{$$names{$name}->{$key}}) {
                  $file =~ s/(\s)/\\$1/g;
                }
              }
            }
          }
        }

        ## We don't need to pass a file name here.  write_output_file()
        ## will determine the file name for itself.
        ($status, $error) = $self->write_output_file($webapp);

        ## Write the .ins file if the user requested it and we were
        ## successful.
        if ($self->{'generate_ins'} && $status) {
          ($status, $error) = $self->write_install_file();
        }
      }
    }
    elsif ($self->warn_useless_project()) {
      my $msg = $self->transform_file_name($self->project_file_name()) .
                " has no useful targets.";

      if ($self->{'current_input'} eq '') {
        $self->information($msg);
      }
      else {
        $self->warning($msg);
      }
    }
  }

  return $status, $error;
}


sub get_project_info {
  return $_[0]->{'project_info'};
}


sub get_lib_locations {
  return $_[0]->{'lib_locations'};
}


sub get_inheritance_tree {
  return $_[0]->{'inheritance_tree'};
}


sub set_component_extensions {
  my $self = shift;
  my $vc = $self->{'valid_components'};
  my $ec = $self->{'exclude_components'};

  foreach my $key (keys %$vc) {
    my $ov = $self->override_valid_component_extensions($key,
                                                        @{$$vc{$key}});
    $$vc{$key} = $ov if (defined $ov);
  }

  foreach my $key (keys %$ec) {
    my $ov = $self->override_exclude_component_extensions($key,
                                                          @{$$ec{$key}});
    $$ec{$key} = $ov if (defined $ov);
  }
}


sub get_component_extensions {
  my($self, $comp) = @_;
  my @ext;
  if (defined $self->{'valid_components'}->{$comp}) {
    ## Build up an array of extensions.  Since they are stored as regular
    ## expressions, we need to remove the escaped period to provide the
    ## minimal amount of text for each extension to provide maximum
    ## flexibility within the project template.
    foreach my $re (@{$self->{'valid_components'}->{$comp}}) {
      push(@ext, $re);
      $ext[$#ext] =~ s/\\\.//;
    }
  }
  return @ext;
}


sub set_source_listing_callback {
  my($self, $cb) = @_;
  $self->{'source_callback'} = $cb;
}


sub reset_values {
  my $self = shift;

  ## Only put data structures that need to be cleared
  ## out when the mpc file is done being read, not at the
  ## end of each project within the mpc file.  Those go in
  ## the closing curly brace section of parse_line().
  $self->{'project_info'}  = [];
  $self->{'lib_locations'} = {};
  $self->reset_generating_types();
}


sub add_default_matching_assignments {
  my $self = shift;
  my $lang = $self->get_language();

  foreach my $key (keys %{$language{$lang}->[0]}) {
    push(@{$language{$lang}->[2]->{$key}}, @default_matching_assignments)
      if (!StringProcessor::fgrep($default_matching_assignments[0],
                                  $language{$lang}->[2]->{$key}));
  }
}


sub reset_generating_types {
  my $self  = shift;
  my $lang  = $self->get_language();
  my %reset = ('valid_components'     => $language{$lang}->[0],
               'custom_only_removed'  => $language{$lang}->[0],
               'exclude_components'   => $language{$lang}->[1],
               'matching_assignments' => $language{$lang}->[2],
               'generated_exts'       => {},
               'valid_names'          => \%validNames,
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
  my $self = shift;
  my $lang = $self->get_language();

  ## This follows along the same logic as read_template_input() by
  ## checking for exe target and then defaulting to a lib target
  if ($self->exe_target()) {
    if ($self->get_static() == 1) {
      return $self->{'lib_exe_template_input'}->{$lang}->{$tikey};
    }
    else {
      return $self->{'dll_exe_template_input'}->{$lang}->{$tikey};
    }
  }

  if ($self->get_static() == 1) {
    return $self->{'lib_template_input'}->{$lang}->{$tikey};
  }

  return $self->{'dll_template_input'}->{$lang}->{$tikey};
}


sub update_project_info {
  my($self, $tparser, $append, $names, $sep) = @_;
  my $value = '';
  $sep = '' if (!defined $sep);

  ## Append the values of all names into one string
  my $ncount = scalar(@$names) - 1;
  for(my $i = 0; $i <= $ncount; $i++) {
    $value .= $self->translate_value(
                               $$names[$i],
                               $tparser->get_value_with_default($$names[$i]));
    $value .= $sep if ($i != $ncount);
  }

  ## There may be more than one template associated with this project
  ## creator.  If there is, we can only add one generated file and we
  ## rely on the project creator to tell us which template generates the
  ## file that we need to track.
  if ($self->file_visible($self->{'current_template'})) {
    ## If we already have an array, take the one off the top.  Otherwise,
    ## create a new one which will be added below.
    my $arr = ($append && defined $self->{'project_info'}->[0] ?
                            pop(@{$self->{'project_info'}}) : []);

    ## Set up the hash table when we are starting a new project_info
    $self->{'project_info_hash_table'} = {}  if (!$append);

    ## If we haven't seen this value yet, put it on the array
    if (!defined $self->{'project_info_hash_table'}->{"@$names $value"}) {
      $self->{'project_info_hash_table'}->{"@$names $value"} = 1;
      push(@$arr, $value);
    }

    ## Always push the array back onto the project_info
    push(@{$self->{'project_info'}}, $arr);
  }

  return $value;
}


sub adjust_value {
  my($self, $names, $value, $tp) = @_;
  my $atemp = $self->get_addtemp();

  ## Perform any additions, subtractions
  ## or overrides for the template values.
  foreach my $name (@$names) {
    if (defined $name && defined $atemp->{lc($name)}) {
      my $lname = lc($name);
      my $base  = $lname;
      $base =~ s/.*:://;

      ## If the template variable is a complex name, then we need to make
      ## sure that the mapped value belongs to the correct type based on
      ## the base of the complex name.  The $tp (TemplateParser) variable
      ## will, in the majority of all calls to this method, be defined so
      ## it is checked second to avoid checking it if the name isn't
      ## complex.
      if ($base =~ /(.+)\->/ && defined $tp) {
        my $v = $tp->get_value($1);
        if (defined $v) {
          my $found = undef;
          foreach my $val (@{$atemp->{$lname}}) {
            if (defined $$val[3]) {
              my $mapped = $self->{'valid_names'}->{$$val[3]};
              if (defined $mapped && UNIVERSAL::isa($mapped, 'ARRAY')) {
                $found = 1 if ($v ne $$mapped[0]);
              }
              last;
            }
          }
          next if ($found);
        }
      }

      my $replace = (defined $self->{'valid_names'}->{$base} &&
                     ($self->{'valid_names'}->{$base} & 0x04) == 0);
      foreach my $val (@{$atemp->{$lname}}) {
        if ($replace && index($$val[1], '<%') >= 0) {
          $$val[1] = $self->replace_parameters($$val[1],
                                               $self->{'command_subs'});
        }
        my $arr = $self->create_array($$val[1]);
        if ($$val[0] > 0) {
          if (!defined $value) {
            $value = '';
          }
          if (UNIVERSAL::isa($value, 'ARRAY')) {
            ## Avoid adding duplicates.  If the existing array contains
            ## the value already, remove it from the newly created array.
            for(my $i = 0; $i < scalar(@$value); $i++) {
              if (StringProcessor::fgrep($$value[$i], $arr)) {
                splice(@$value, $i, 1);
                $i--;
              }
            }

            ## We need to make $value a new array reference ($arr)
            ## to avoid modifying the array reference pointed to by $value
            unshift(@$arr, @$value);
            $value = $arr;
          }
          else {
            $value .= " $$val[1]";
          }
        }
        elsif ($$val[0] < 0) {
          if (defined $value) {
            my $parts;
            if (UNIVERSAL::isa($value, 'ARRAY')) {
              $parts = $value;
            }
            else {
              $parts = $self->create_array($value);
            }

            $value = [];
            foreach my $part (@$parts) {
              if ($part ne '') {
                push(@$value, $part) if (!StringProcessor::fgrep($part, $arr));
              }
            }
          }
        }
        else {
          ## If the user set the variable to empty, then we need to
          ## set the value to undef
          $value = (defined $$arr[0] ? $arr : undef);
        }
      }
      last;
    }
  }

  return $value;
}


sub get_verbatim {
  my($self, $marker) = @_;
  my $str;
  my $thash = $self->{'verbatim'}->{$self->{'pctype'}};

  if (defined $thash) {
    if (defined $thash->{$marker}) {
      my $crlf = $self->crlf();
      foreach my $line (@{$thash->{$marker}}) {
        $str = '' if (!defined $str);
        $str .= $self->process_special($line) . $crlf;
      }
      if (defined $str) {
        $str .= $crlf;
        $self->{'verbatim_accessed'}->{$self->{'pctype'}}->{$marker} = 1;
      }
    }
  }

  return $str;
}


sub generate_recursive_input_list {
  my($self, $dir, $exclude) = @_;
  return $self->extension_recursive_input_list($dir,
                                               $exclude,
                                               $ProjectCreatorExtension);
}


sub get_modified_project_file_name {
  my($self, $name, $ext) = @_;
  my $nmod = $self->get_name_modifier();

  ## We don't apply the name modifier to the project file
  ## name if we have already applied it to the project name
  ## since the project file name comes from the project name.
  if (defined $nmod && !$self->get_apply_project()) {
    $nmod =~ s/\*/$name/g;
    $name = $nmod;
  }
  return "$name$ext";
}


sub get_valid_names {
  return $_[0]->{'valid_names'};
}


sub get_feature_parser {
  return $_[0]->{'feature_parser'};
}


sub preserve_assignment_order {
  my($self, $name) = @_;
  my $mapped = $self->{'valid_names'}->{$name};

  ## Only return the value stored in the valid_names hash map if it's
  ## defined and it's not an array reference.  The array reference is
  ## a keyword mapping and all mapped keywords should have preserved
  ## assignment order.
  if (defined $mapped && !UNIVERSAL::isa($mapped, 'ARRAY')) {
    return ($mapped & 1);
  }

  return 1;
}


sub add_to_template_input_value {
  my($self, $name) = @_;
  my $mapped = $self->{'valid_names'}->{$name};

  ## Only return the value stored in the valid_names hash map if it's
  ## defined and it's not an array reference.  The array reference is
  ## a keyword mapping and no mapped keywords should be added to
  ## template input variables.
  if (defined $mapped && !UNIVERSAL::isa($mapped, 'ARRAY')) {
    return ($mapped & 2);
  }

  return 0;
}


sub dependency_combined_static_library {
  #my $self = shift;
  return defined $ENV{MPC_DEPENDENCY_COMBINED_STATIC_LIBRARY};
}


sub translate_value {
  my($self, $key, $val) = @_;

  if ($key eq 'after' && $val ne '') {
    my $arr = $self->create_array($val);
    $val = '';

    if ($self->require_dependencies()) {
      foreach my $entry (@$arr) {
        if ($self->get_apply_project()) {
          my $nmod = $self->get_name_modifier();
          if (defined $nmod) {
            $nmod =~ s/\*/$entry/g;
            $entry = $nmod;
          }
        }
        $val .= '"' . ($self->dependency_is_filename() ?
                          $self->project_file_name($entry) : $entry) . '" ';
      }
      $val =~ s/\s+$//;
    }
  }
  return $val;
}


sub requires_parameters {
  #my $self = shift;
  #my $name = shift;
  return $custom{$_[1]};
}


sub project_file_name {
  my($self, $name, $template) = @_;

  ## Fill in the name if one wasn't provided
  $name = $self->get_assignment('project_name') if (!defined $name);

  return $self->get_modified_project_file_name(
                                     $self->project_file_prefix() . $name,
                                     $self->project_file_extension());
}


sub remove_non_custom_settings {
  my $self = shift;

  ## Remove any files that may have automatically been added
  ## to this project
  foreach my $key (keys %{$self->{'custom_only_removed'}}) {
    $self->{$key} = {};
  }

  ## Unset the exename, sharedname and staticname
  $self->process_assignment('exename',    undef);
  $self->process_assignment('sharedname', undef);
  $self->process_assignment('staticname', undef);
}


sub remove_wanted_extension {
  my($self, $name, $array) = @_;

  foreach my $wanted (@$array) {
    return $name if ($name =~ s/$wanted$//);
  }

  ## If the user provided file does not match any of the
  ## extensions specified by the custom definition, we need
  ## to remove the extension or else this file will not be
  ## added to the project.
  $name =~ s/\.[^\.]+$//;
  return $name;
}


sub resolve_alias {
  if (index($_[1], 'install') >= 0) {
    my $resolved = $_[1];
    if ($resolved =~ s/(.*::)install$/$1exeout/) {
    }
    elsif ($resolved eq 'install') {
      $resolved = 'exeout';
    }
    return $resolved;
  }
  return $_[1];
}


sub create_feature_parser {
  my($self, $features, $feature) = @_;
  my $gfeature     = $self->{'gfeature_file'};
  my $typefeaturef = (defined $gfeature ?
                              $self->mpc_dirname($gfeature) . '/' : '') .
                     $self->{'pctype'} . '.features';
  $typefeaturef = undef if (! -r $typefeaturef);
  if (defined $feature && $feature !~ /[\/\\]/i) {
    my $searched = $self->search_include_path($feature);
    $feature = $searched if (defined $searched);
  }
  my $fp = new FeatureParser($features,
                             $gfeature,
                             $typefeaturef,
                             $feature);

  my $slo = $fp->get_value($static_libs_feature);
  if (!defined $slo) {
    my $sval = $self->get_static() || 0;
    $fp->parse_line(undef,
                    $static_libs_feature . ' = ' . $sval);
  }

  return $fp;
}


sub restore_state_helper {
  my($self, $skey, $old, $new) = @_;

  if ($skey eq 'feature_file') {
    if ($self->{'features_changed'} ||
        !(!defined $old && !defined $new ||
          (defined $old && defined $new && $old eq $new))) {
      ## Create a new feature parser.  This relies on the fact that
      ## 'features' is restored first in restore_state().
      $self->{'feature_parser'} = $self->create_feature_parser(
                                           $self->get_features(), $new);
      $self->{'features_changed'} = undef;
    }
  }
  elsif ($skey eq 'ti') {
    my $lang = $self->get_language();
    my @keys = keys %$old;
    @keys = keys %$new if (!defined $keys[0]);
    foreach my $key (@keys) {
      if (!defined $$old{$key} || !defined $$new{$key} ||
          $$old{$key} ne $$new{$key}) {
        ## Clear out the template input reader that we're currently set
        ## to use.
        $self->{$key . '_template_input'}->{$lang}->{$tikey} = undef;
      }
    }
  }
  elsif ($skey eq 'features') {
    ## If the user has changed the 'features' setting, then we need to
    ## make sure that we create a new feature parser regardless of
    ## whether or not the feature file has changed.
    $self->{'features_changed'} = ("@$old" ne "@$new");
  }
  elsif ($skey eq 'language') {
    if ($old ne $new) {
      $self->add_default_matching_assignments();
    }
  }
}


sub get_initial_relative_values {
  return $_[0]->{'expanded'}, 1;
}

sub add_main_function {
  my $langmain = shift;

  ## See if a language was supplied.
  if ($langmain =~ /([^:]+):(.+)/) {
    ## If the language supplied is not one that we know about, return an
    ## error message.
    return 'Invalid language: ' . $1 if (!defined $language{$1});

    ## Otherwise, add it to the list for the language.
    push(@{$mains{$1}}, $2);
  }
  else {
    ## No language was supplied, so add the main to all of the languages
    ## that we support.
    foreach my $lang (keys %language) {
      push(@{$mains{$lang}}, $langmain);
    }
  }

  ## Return no error message.
  return undef;
}

sub get_resource_tag {
  my $self = shift;
  my $lang = $self->get_language();

  ## Not all entries in the %language map have a resource tag.
  ## For this, we will just return the tag for C++ since it probably
  ## doesn't really matter anyway.
  return defined $language{$lang}->[5] ? $language{$lang}->[5] : $cppresource;
}

# ************************************************************
# Accessors used by support scripts
# ************************************************************

sub getKeywords {
  return \%validNames;
}

sub getValidComponents {
  my $language = shift;
  return (defined $language{$language} ? $language{$language}->[0] : undef);
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub languageSupported {
  #my $self = shift;
  return $_[0]->get_language() eq Creator::cplusplus;
}

sub file_visible {
  #my($self, $template) = @_;
  return 1;
}

sub webapp_supported {
  #my $self = shift;
  return 0;
}


sub use_win_compatibility_commands {
  #my $self = shift;
  return $ENV{MPC_USE_WIN_COMMANDS};
}


sub post_file_creation {
  #my $self = shift;
  #my $file = shift;
}


sub escape_spaces {
  #my $self = shift;
  return 0;
}


sub validated_directory {
  my($self, $dir) = @_;
  return $dir;
}

sub get_quote_symbol {
  #my $self = shift;
  return '"';
}

sub get_escaped_quote_symbol {
  #my $self = shift;
  return '\\\"';
}

sub get_gt_symbol {
  #my $self = shift;
  return '>';
}


sub get_lt_symbol {
  #my $self = shift;
  return '<';
}


sub get_and_symbol {
  #my $self = shift;
  return '&&';
}


sub get_or_symbol {
  #my $self = shift;
  return '||';
}


sub get_cmdsep_symbol {
  #my $self = shift;
  return ';';
}


sub dollar_special {
  #my $self = shift;
  return 0;
}


sub expand_variables_from_template_values {
  #my $self = shift;
  return 1;
}


sub require_dependencies {
  #my $self = shift;
  return 1;
}


sub dependency_is_filename {
  #my $self = shift;
  return 1;
}


sub fill_value {
  #my $self = shift;
  #my $name = shift;
  return undef;
}


sub project_file_prefix {
  #my $self = shift;
  return '';
}


sub project_file_extension {
  #my $self = shift;
  return '';
}


sub override_valid_component_extensions {
  #my $self = shift;
  #my $comp = shift;
  return undef;
}


sub override_exclude_component_extensions {
  #my $self = shift;
  #my $comp = shift;
  return undef;
}


sub get_dll_exe_template_input_file {
  #my($self, $tkey) = @_;
  return undef;
}


sub get_lib_exe_template_input_file {
  my($self, $tkey) = @_;
  return $self->get_dll_exe_template_input_file($tkey);
}


sub get_lib_template_input_file {
  my($self, $tkey) = @_;
  return $self->get_dll_template_input_file($tkey);
}


sub get_dll_template_input_file {
  #my($self, $tkey) = @_;
  return undef;
}


sub get_template {
  return $_[0]->{'pctype'};
}

sub requires_forward_slashes {
  return 0;
}

sub warn_useless_project {
  return 1;
}

sub get_properties {
  my $self = shift;
  return {'static' => $self->get_static(),
          $self->get_language() => 1};
}

1;
