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
use File::Basename;

use Creator;
use TemplateInputReader;
use TemplateParser;

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
                  );

## Deal with these components in a special way
my(@specialComponents) = ('header_files', 'inline_files');

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
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, $dynamic, $static,
                                $relative, $addtemp, $addproj,
                                $progress, $toplevel, $baseprojs,
                                'project');

  $self->{$self->{'type_check'}}   = 0;
  $self->{'project_info'}          = [];
  $self->{'reading_parent'}        = [];
  $self->{'dexe_template_input'}   = undef;
  $self->{'lexe_template_input'}   = undef;
  $self->{'lib_template_input'}    = undef;
  $self->{'dll_template_input'}    = undef;
  $self->{'idl_defaulted'}         = 0;
  $self->{'source_defaulted'}      = 0;
  $self->{'writing_type'}          = 0;
  $self->{'flag_overrides'}        = {};
  $self->{'special_supplied'}      = {};

  ## Set up the verbatim constructs
  $self->{'verbatim'} = {};

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
  my(%ma) = ('source_files' => [ 'includes' ],
             'idl_files'    => [ 'idlgendir', 'idlflags' ],
            );
  $self->{'matching_assignments'} = \%ma;
  $self->{'valid_components'}     = \%vc;
  $self->{'exclude_components'}   = \%ec;
  $self->{'skeleton_endings'}     = [ 'C', 'S' ];
  $self->{'type_specific_assign'} = {};
  $self->{'pctype'}               = $self->extractType("$self");

  ## Allow subclasses to override the default extensions
  $self->set_component_extensions();

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
            $self->write_project();

            foreach my $key (keys %{$self->{'valid_components'}}) {
              delete $self->{$key};
            }
            $self->{'assign'}   = {};
            $self->{'verbatim'} = {};
            $self->{'type_specific_assign'} = {};
          }
        }
        $self->{$typecheck}         = 0;
        $self->{'idl_defaulted'}    = 0;
        $self->{'flag_overrides'}   = {};
        $self->{'source_defaulted'} = 0;
        $self->{'special_supplied'} = {};
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
              push(@{$self->{'reading_parent'}}, 1);
              $status = $self->parse_file($file);
              pop(@{$self->{'reading_parent'}});

              if (!$status) {
                $errorString = "ERROR: Invalid parent: $parent";
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
          $name =~ s/^\(\s*//;
          $name =~ s/\s*\)$//;
          if ($name =~ /[\/\\]/) {
            $status = 0;
            $errorString = 'ERROR: Projects can not have a slash ' .
                           'or a back slash in the name';
          }
          else {
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
        my($over) = {};
        if (defined $self->{'flag_overrides'}->{$tag}) {
          $over = $self->{'flag_overrides'}->{$tag};
        }
        else {
          $self->{'flag_overrides'}->{$tag} = $over;
        }

        if ($values[0] eq 'assignment') {
          $self->process_assignment($values[1],
                                    $values[2], \%flags);
        }
        elsif ($values[0] eq 'assign_add') {
          ## If there is no value in %flags, then we need to get
          ## the outer scope value and put it in there.
          if (!defined $self->get_assignment($values[1], \%flags)) {
            my($outer) = $self->get_assignment($values[1]);
            $self->process_assignment($values[1], $outer, \%flags);
          }
          $self->process_assignment_add($values[1],
                                        $values[2], \%flags);
        }
        elsif ($values[0] eq 'assign_sub') {
          ## If there is no value in %flags, then we need to get
          ## the outer scope value and put it in there.
          if (!defined $self->get_assignment($values[1], \%flags)) {
            my($outer) = $self->get_assignment($values[1]);
            $self->process_assignment($values[1], $outer, \%flags);
          }
          $self->process_assignment_sub($values[1],
                                        $values[2], \%flags);
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


sub add_idl_generated {
  my($self)    = shift;
  my($tag)     = shift;
  my($idl)     = shift;
  my($names)   = $self->{$tag};
  my($vc)      = $self->{'valid_components'};
  my($wanted)  = $$vc{$tag}->[0];
  my(@added)   = ();

  $wanted =~ s/\\//;
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my($array) = $$comps{$key};
      foreach my $i (@$idl) {
        my($file) = $i;
        $file =~ s/\.idl$//;
        foreach my $ending (@{$self->{'skeleton_endings'}}) {
          my($created) = "$file$ending$wanted";
          if (!$self->already_added($array, $created)) {
            push(@added, $created);
          }
        }
      }
      ## Put the generated files at the front
      if (defined $added[0]) {
        unshift(@$array, @added);
      }
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
    if (!$self->lib_target()) {
      my($fh)      = new FileHandle();
      my($exename) = undef;
      foreach my $file ($self->get_component_list('source_files')) {
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
      ## default to a library
      if (!$self->exe_target()) {
        my($base) = $self->base_directory();
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
      if ($self->convert_slashes()) {
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


sub escape_regex_special {
  my($self) = shift;
  my($name) = shift;

  $name =~ s/\\/\\\\/g;
  $name =~ s/\$/\\\$/g;
  $name =~ s/\[/\\\[/g;
  $name =~ s/\]/\\\]/g;
  $name =~ s/\(/\\\(/g;
  $name =~ s/\)/\\\)/g;

  return $name;
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
      my($pjname) = $self->escape_regex_special(
                             $self->transform_file_name(
                               $self->get_assignment('project_name')));
      foreach my $save (@saved) {
        my($file) = $self->escape_regex_special($save);
        if ($pjname =~ /$file/ || $file =~ /$pjname/) {
          if (!$self->already_added($array, $file)) {
            push(@$array, $file);
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
          if ($tag eq 'idl_files' && defined $$array[0]) {
            $self->{'idl_defaulted'} = 1;
          }
          elsif ($tag eq 'source_files') {
            ## If we are auto-generating the source_files, then
            ## we need to make sure that any idl generated source
            ## files that are added are put at the front of the list.
            my(@front) = ();
            my(@copy)  = @$array;
            my(@exts)  = $self->generated_source_extensions($tag);

            $self->{'source_defaulted'} = 1;
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


sub remove_duplicated_files {
  my($self)   = shift;
  my($dest)   = shift;
  my($source) = shift;
  my($names)  = $self->{$dest};
  my(@slist)  = $self->get_component_list($source);

  ## Find out which source files are listed
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my($array) = $$comps{$key};
      my($count) = scalar(@$array);
      for(my $i = 0; $i < $count; ++$i) {
        foreach my $sfile (@slist) {
          ## Is the source file is in the component array?
          if ($$array[$i] eq $sfile) {
            ## Remove the element and fix the index and count
            splice(@$array, $i, 1);
            --$count;
            --$i;
            last;
          }
        }
      }
    }
  }
}


sub generated_source_extensions {
  my($self) = shift;
  my($tag)  = shift;
  my($vc)   = $self->{'valid_components'};
  my($gc)   = $$vc{$tag};
  my(@gen)  = ();

  foreach my $e (@$gc) {
    foreach my $ending (@{$self->{'skeleton_endings'}}) {
      push(@gen, "$ending$e");
    }
  }
  return @gen;
}


sub generated_source_listed {
  my($self)  = shift;
  my($tag)   = shift;
  my($idl)   = shift;
  my($names) = $self->{$tag};
  my(@gen)   = $self->generated_source_extensions($tag);
  my(@found) = ();

  ## Find out which generated source files are listed
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $key (keys %$comps) {
      my($array) = $$comps{$key};
      foreach my $val (@$array) {
        foreach my $ext (@gen) {
          foreach my $i (@$idl) {
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


sub generate_default_idl_generated {
  my($self) = shift;
  my($tags) = shift;

  if ($self->{'idl_defaulted'}) {
    ## After all source and headers have been defaulted, see if we
    ## need to add the idl generated .h, .i and .cpp files
    if (defined $self->{'idl_files'}) {
      ## Build up the list of idl files
      my(@idl) = ();
      my($names) = $self->{'idl_files'};
      foreach my $name (keys %$names) {
        my($comps) = $$names{$name};
        foreach my $key (keys %$comps) {
          my($array) = $$comps{$key};
          foreach my $val (@$array) {
            my($f) = $val;
            $f =~ s/\.idl$//;
            push(@idl, $f);
          }
        }
      }

      foreach my $type (@$tags) {
        if (!$self->generated_source_listed($type, \@idl)) {
          $self->add_idl_generated($type, \@idl);
        }
      }
    }
  }
}


sub add_source_corresponding_component_files {
  my($self)  = shift;
  my($tag)   = shift;
  my(@all)   = ();
  my($vc)    = $self->{'valid_components'};

  foreach my $filetag ('source_files', 'template_files') {
    my($names) = $self->{$filetag};
    foreach my $name (keys %$names) {
      my($comps) = $$names{$name};
      foreach my $comp (keys %$comps) {
        push(@all, @{$$comps{$comp}});
      }
    }
  }

  ## We need to cross-check the idl files.  But we need to remove
  ## the idl extension first.
  my(@idl) = $self->get_component_list('idl_files');
  for(my $i = 0; $i <= $#idl; $i++) {
    $idl[$i] =~ s/\.idl$//;
  }

  ## for each cpp file, we add a corresponding header or inline file
  ## if it exists and is not already in the list of headers
  my($names) = $self->{$tag};
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $comp (keys %$comps) {
      my($array) = $$comps{$comp};
      foreach my $cpp (@all) {
        my($found) = 0;
        my($c) = $cpp;
        $c =~ s/\.[^\.]+$//;
        foreach my $file (@$array) {
          my($added) = $c;
          if ($file =~ /(\.[^\.]+)$/) {
            $added .= $1;
          }

          if ($added eq $file) {
            $found = 1;
            last;
          }
        }

        if (!$found) {
          my($added) = 0;
          foreach my $e (@{$$vc{$tag}}) {
            my($ext) = $e;
            $ext =~ s/\\//g;

            ## If the file is readable
            my($file) = "$c$ext";
            if (-r $file) {
              if (!$self->already_added($array, $file)) {
                push(@$array, $file);
              }
              $added = 1;
              last;
            }
          }
          if (!$added) {
            ## If we did not add the file in the above loop,
            ## we must check to see if the file *would be* generated
            ## from idl.  If so, we will add the file with the default
            ## (i.e. first) file extension.
            foreach my $idlfile (@idl) {
              my($idl) = $self->escape_regex_special($idlfile);
              if ($c =~ /^$idl/) {
                foreach my $ending (@{$self->{'skeleton_endings'}}) {
                  if ($c =~ /^$idl$ending$/) {
                    my($ext) = $$vc{$tag}->[0];
                    $ext =~ s/\\//g;
                    my($file) = "$c$ext";
                    if (!$self->already_added($array, $file)) {
                      push(@$array, $file);
                    }
                    $added = 1;
                    last;
                  }
                }
              }
              last;
            }
          }
        }
      }
    }
  }
}


sub generate_defaults {
  my($self) = shift;

  ## Generate default project name
  if (!defined $self->get_assignment('project_name')) {
    my($current) = $self->get_current_input();
    if ($current eq '') {
      $self->process_assignment('project_name', $self->base_directory());
    }
    else {
      ## Since files on UNIX can have back slashes, we transform them
      ## into underscores.
      $current =~ s/\\/_/g;

      ## Take off the extension
      $current =~ s/\.[^\.]+$//;
      $self->process_assignment('project_name', $current);
    }
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

  ## Generate the default idl generated list of source files
  ## only if we defaulted the idl file list
  $self->generate_default_idl_generated(['source_files']);

  ## Add @specialComponents files based on the
  ## source_components (i.e. .h and .i or .inl based on .cpp)
  foreach my $tag (@specialComponents) {
    $self->add_source_corresponding_component_files($tag);
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
            if (!defined $$array[0] || $self->{'source_defaulted'}) {
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

  if ($self->convert_slashes()) {
    for(my $i = 0; $i <= $#list; $i++) {
      $list[$i] = $self->slash_to_backslash($list[$i]);
    }
  }

  if ($self->sort_files()) {
    @list = sort { $self->file_sorter($a, $b) } @list;
  }

  return @list;
}


sub need_to_write_project {
  my($self)  = shift;
  foreach my $key (keys %{$self->{'valid_components'}}) {
    my($names) = $self->{$key};
    foreach my $name (keys %$names) {
      foreach my $key (sort keys %{$names->{$name}}) {
        if (defined $names->{$name}->{$key}->[0]) {
          return 1;
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

          if (open($fh, ">$name")) {
            my($lines) = $tp->get_lines();
            foreach my $line (@$lines) {
              print $fh $line;
            }
            close($fh);

            $self->add_file_written($name);
          }
          else {
            $error = "ERROR: Unable to open $name for output.";
            $status = 0;
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
  my($prjname)  = $self->get_assignment('project_name');
  my($progress) = $self->get_progress_callback();

  if (defined $progress) {
    &$progress();
  }

  if ($self->need_to_write_project()) {
    ## Writing the non-static file so set it to 0
    if ($self->get_dynamic()) {
      $self->{'writing_type'} = 0;
      $self->process_assignment('project_name',
                                $prjname . $self->get_type_append());
      ($status, $error) = $self->write_output_file($name);
    }

    if ($status && $self->get_static() && $self->separate_static_project()) {
      ## Set the project name back to what it originally was
      $self->process_assignment('project_name', $prjname);
      $name = $self->transform_file_name($self->static_project_file_name());

      ## Writing the static file so set it to 1
      $self->{'writing_type'} = 1;
      $self->process_assignment('project_name',
                                $prjname . $self->get_type_append());
      ($status, $error) = $self->write_output_file($name);
    }

    if (!$status) {
      print STDERR "$error\n";
    }
  }

  return $status;
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


sub get_template_input {
  my($self) = shift;

  if ($self->lib_target()) {
    if ($self->{'writing_type'} == 1) {
      return $self->{'lib_template_input'};
    }
    else {
      return $self->{'dll_template_input'};
    }
  }

  if ($self->{'writing_type'} == 1) {
    return $self->{'lexe_template_input'};
  }
  else {
    return $self->{'dexe_template_input'};
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


sub get_type_append {
  #my($self) = shift;
  return '';
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
