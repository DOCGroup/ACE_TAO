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

my($BaseClassExtension)      = "mpb";
my($ProjectCreatorExtension) = "mpc";
my($TemplateExtension)       = "mpd";
my($TemplateInputExtension)  = "mpt";

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
                   'pch_header'      => 1,
                   'pch_source'      => 1,
                   'tao'             => 1,
                   'dllout'          => 1,
                   'libout'          => 1,
                   'dllflags'        => 1,
                   'libflags'        => 1,
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
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, 'project');
  my($typecheck) = $self->{'type_check'};

  $self->{$typecheck}             = 0;
  $self->{'global_assign'}        = {};
  $self->{'files_written'}        = [];
  $self->{'project_info'}         = [];
  $self->{'reading_global'}       = 0;
  $self->{'reading_parent'}       = [];
  $self->{'exe_template_input'}   = undef;
  $self->{'lib_template_input'}   = undef;
  $self->{'dll_template_input'}   = undef;
  $self->{'idl_defaulted'}        = 0;
  $self->{'writing_type'}         = 0;

  ## Valid component names within a project along with the valid file extensions
  my(%vc) = ('source_files'        => [ "\\.cpp", "\\.cxx", "\\.cc", "\\.c", "\\.C", ],
             'template_files'      => [ "_T\\.cpp", "_T\\.cxx", "_T\\.cc", "_T\\.c", "_T\\.C", ],
             'header_files'        => [ "\\.h", "\\.hxx", "\\.hh", ],
             'inline_files'        => [ "\\.i", "\\.inl", ],
             'idl_files'           => [ "\\.idl", ],
             'documentation_files' => [ "\\.doc", "\\.txt", ],
             'resource_files'      => [ "\\.rc", ],
            );

  ## Exclude these extensions when auto generating the component values
  my(%ec) = ('source_files' => [ "_T\\.cpp", "_T\\.cxx", "_T\\.cc", "_T\\.C", ],
            );

  $self->{'valid_components'}   = \%vc;
  $self->{'exclude_components'} = \%ec;
  $self->{'skeleton_endings'}   = [ "C", "S" ];

  ## Allow subclasses to override the default extensions
  $self->set_component_extensions();

  return $self;
}


sub read_global_configuration {
  my($self)        = shift;
  my($input)       = $self->get_global_cfg();
  my($status)      = 0;
  my($errorString) = "";

  $self->{'reading_global'} = 1;
  if (defined $input) {
    $status = $self->parse_file($input);
  }
  $self->{'reading_global'} = 0;

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
      if (defined $name && $name eq "}") {
        ## Project Ending
        my($rp) = $self->{'reading_parent'};
        if (!defined $$rp[0] && !$self->{'reading_global'}) {
          ## Fill in all the default values
          $self->generate_defaults();

          ## End of project; Write out the file.
          $self->write_project();

          foreach my $key (keys %{$self->{'valid_components'}}) {
            delete $self->{$key};
          }
          $self->{'assign'} = {};
        }
        $self->{$typecheck}      = 0;
        $self->{'idl_defaulted'} = 0;
      }
      else {
        ## Project Beginning
        ## Deal with the inheritance hiearchy first
        my($parents) = $values[2];
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
              my($rp) = $self->{'reading_parent'};
              push(@$rp, 1);
              $status = $self->parse_file($file);
              pop(@$rp);

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
          $self->process_assignment('project_name', $name);
        }
        $self->{$typecheck} = 1;

        ## Copy each value from global_assign into assign
        if (!$self->{'reading_global'}) {
          foreach my $key (keys %{$self->{'global_assign'}}) {
            $self->{'assign'}->{$key} = $self->{'global_assign'}->{$key};
          }
        }
      }
    }
    elsif ($values[0] eq "assignment") {
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
    elsif ($values[0] eq "assign_add") {
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
    elsif ($values[0] eq "assign_sub") {
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
    elsif ($values[0] eq "component") {
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
        if ($self->parse_components($ih, $comp, $name)) {
        }
        else {
          $errorString = "ERROR: Unable to process $comp";
          $status = 0;
        }
      }
      else {
        $errorString = "ERROR: Invalid component name: $comp";
        $status = 0;
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

  while(<$fh>) {
    my($line) = $self->strip_line($_);

    if ($line eq "") {
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
      my($array) = $$comps{$current};
      push(@$array, $line);
    }
    else {
      $status = 0;
      last;
    }
  }

  return $status;
}


sub process_assignment {
  my($self)   = shift;
  my($name)   = shift;
  my($value)  = shift;
  my($tag)    = ($self->{'reading_global'} ? 'global_assign' : 'assign');
  my($assign) = $self->{$tag};

  if (!defined $assign) {
    $assign = {};
    $self->{$tag} = $assign;
  }

  if (defined $value) {
    $value =~ s/^\s+//;
    $value =~ s/\s+$//;

    if ($self->convert_slashes()) {
      $value =~ s/\//\\/g;
    }
  }

  $$assign{$name} = $value;
}


sub process_assignment_add {
  my($self)   = shift;
  my($name)   = shift;
  my($value)  = shift;
  my($nval)   = $self->get_assignment($name);
  if (defined $nval) {
    $nval = "$value $nval";
  }
  else {
    $nval = $value;
  }
  $self->process_assignment($name, $nval);
}


sub process_assignment_sub {
  my($self)   = shift;
  my($name)   = shift;
  my($value)  = shift;
  my($nval)   = $self->get_assignment($name);

  if (defined $nval) {
    my($parts) = $self->create_array($nval);
    $nval = "";
    foreach my $part (@$parts) {
      if ($part ne $value && $part ne "") {
        $nval .= "$part ";
      }
    }
    $self->process_assignment($name, $nval);
  }
}


sub read_template_input {
  my($self)        = shift;
  my($status)      = 1;
  my($errorString) = "";
  my($file)        = undef;
  my($tag)         = undef;
  my($ti)          = $self->get_ti_override();
  my($override)    = 0;

  if ($self->exe_target()) {
    $tag = 'exe_template_input';
    if (!defined $self->{$tag}) {
      if (defined $$ti{'exe'}) {
        $file = $$ti{'exe'};
        $override = 1;
      }
      else {
        $file = $self->get_exe_template_input_file();
      }
    }
  }
  else {
    if ($self->{'writing_type'}) {
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
      $self->{$tag} = new TemplateInputReader();
      ($status, $errorString) = $self->{$tag}->read_file($file);
    }
    else {
      if ($override) {
        $status = 0;
        $errorString = "Unable to locate template input file.";
      }
    }
  }

  return $status, $errorString;
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
          push(@added, "$file$ending$wanted");
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
  my($self)   = shift;
  my($base)   = shift;

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
    if (!defined $sharedname) {
      $self->process_assignment('sharedname', $base);
    }
    if (!defined $staticname) {
      $self->process_assignment('staticname', $base);
    }
  }
}


sub generate_default_pch_filenames {
  my($self)   = shift;
  my($base)   = shift;
  my($files)  = shift;
  my($vc)     = $self->{'valid_components'};
  my($gc)     = $$vc{'header_files'};
  my($found)  = 0;

  if (!defined $self->get_assignment('pch_header')) {
    foreach my $file (@$files) {
      foreach my $ext (@$gc) {
        if ($file =~ /(.*_pch$ext)/) {
          $self->process_assignment('pch_header', $1);
          $found = 1;
          last;
        }
      }
      if ($found) {
        last;
      }
    }
  }

  if (!defined $self->get_assignment('pch_source')) {
    $gc    = $$vc{'source_files'};
    $found = 0;
    foreach my $file (@$files) {
      foreach my $ext (@$gc) {
        if ($file =~ /(.*_pch$ext)/) {
          $self->process_assignment('pch_source', $1);
          $found = 1;
          last;
        }
      }
      if ($found) {
        last;
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
          ## Save these files for later
          $exclude = 1;
          push(@saved, $file);
        }

        if (!$exclude) {
          push(@$array, $file);
        }
        last;
      }
    }
  }

  ## Now deal with the saved files
  if (defined $saved[0]) {
    my($pjname) = $self->get_assignment('project_name');
    foreach my $file (@saved) {
      if ($pjname =~ /$file/ || $file =~ /$pjname/) {
        push(@$array, $file);
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
                  push(@built, $file);
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
            $self->process_assignment('tao', 1);
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
            if ($val =~ /$i$ext$/) {
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
    ## need to add the idl generated .h, .inl and .cpp files
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
  my($names) = $self->{'source_files'};
  my(@all)   = ();
  my($vc)    = $self->{'valid_components'};

  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $comp (keys %$comps) {
      push(@all, @{$$comps{$comp}});
    }
  }

  ## for each cpp file, we add a corresponding header or inline file
  ## if it exists and is not already in the list of headers
  $names = $self->{$tag};
  foreach my $name (keys %$names) {
    my($comps) = $$names{$name};
    foreach my $comp (keys %$comps) {
      my($array) = $$comps{$comp};
      foreach my $cpp (@all) {
        my($found) = 0;
        my($c) = $cpp;
        $c =~ s/\.[^\.]+$//;
        foreach my $file (@$array) {
          my($w)     = $file;
          my($added) = $c;
          if ($w =~ /(\.[^\.]+)$/) {
            $added .= $1;
          }

          if ($added eq $w) {
            $found = 1;
            last;
          }
        }

        if (!$found) {
          my($added) = 0;
          foreach my $e (@{$$vc{$tag}}) {
            my($ext) = $e;
            $ext =~ s/\\//g;

            ## If the file is readable or
            if (-r "$c$ext") {
              push(@$array, "$c$ext");
              $added = 1;
              last;
            }
          }
          if (!$added) {
            ## If we did not add the file in the above loop,
            ## we must check to see if the file *would be* generated
            ## from idl.  If so, we will add the file with the default
            ## (i.e. first) file extension.
            foreach my $ending (@{$self->{'skeleton_endings'}}) {
              if ($c =~ /$ending$/) {
                my($ext) = $$vc{$tag}->[0];
                $ext =~ s/\\//g;
                push(@$array, "$c$ext");
                last;
              }
            }
          }
        }
      }
    }
  }
}


sub generate_defaults {
  my($self)   = shift;
  my($base)   = $self->base_directory();

  ## Generate default project name
  if (!defined $self->get_assignment('project_name')) {
    $self->process_assignment('project_name', $base);
  }

  $self->generate_default_target_names($base);

  my(@files) = $self->generate_default_file_list();
  $self->generate_default_pch_filenames($base, \@files);

  ## Generate default components, but @specialComponents
  ## are skipped in the initial default components generation
  $self->generate_default_components(\@files);

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
    my($names) = $self->{$tag};
    if (defined $names) {
      foreach my $name (keys %$names) {
        my($comps) = $$names{$name};
        foreach my $comp (keys %$comps) {
          my($array) = $$comps{$comp};
          if (!defined $$array[0]) {
            $self->generate_default_components(\@files, $tag);
          }
        }
      }
    }
  }
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


sub get_assignment {
  my($self) = shift;
  my($name) = shift;
  my($tag)  = ($self->{'reading_global'} ? 'global_assign' : 'assign');
  return $self->{$tag}->{$name};
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
      $list[$i] =~ s/\//\\/g;
    }
  }

  if ($self->sort_files()) {
    @list = sort { $self->file_sorter($a, $b) } @list;
  }

  return @list;
}


sub write_output_file {
  my($self)     = shift;
  my($name)     = shift;
  my($status)   = 0;
  my($error)    = "";
  my($dir)      = dirname($name);
  my($fh)       = new FileHandle();
  my($tover)    = $self->get_template_override();
  my($template) = (defined $tover ? $tover : $self->get_template()) .
                  ".$TemplateExtension";
  my($tfile)    = $self->search_include_path($template);

  if (defined $tfile) {
    if ($dir ne ".") {
      mkpath($dir, 0, 0777);
    }

    ## Read in the template values for the
    ## specific target and project type
    ($status, $error) = $self->read_template_input();

    if ($status) {
      my($tp) = new TemplateParser($self);

      ## Set the project_file assignment for the template parser
      $self->process_assignment('project_file', $name);

      ($status, $error) = $tp->parse_file($tfile);

      if ($status) {
        if (open($fh, ">$name")) {
          my($lines) = $tp->get_lines();
          foreach my $line (@$lines) {
            print $fh "$line";
          }
          close($fh);
          my($fw) = $self->{'files_written'};
          push(@$fw, $name);
        }
        else {
          $error = "ERROR: Unable to open $name for output.";
          $status = 0;
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
  my($error)    = "";
  my($name)     = $self->project_file_name();
  my($prjname)  = $self->get_assignment('project_name');

  ## Writing the non-static file so set it to 0
  $self->{'writing_type'} = 0;
  $self->process_assignment('project_name',
                            $prjname . $self->get_type_append());
  ($status, $error) = $self->write_output_file($name);

  if ($status &&
      $self->separate_static_project() && $self->lib_target()) {
    $name = $self->static_project_file_name();

    ## Writing the static file so set it to 1
    $self->{'writing_type'} = 1;
    $self->process_assignment('project_name',
                              $prjname . $self->get_type_append());
    ($status, $error) = $self->write_output_file($name);
  }

  if (!$status) {
    print STDERR "$error\n";
  }

  return $status;
}


sub get_files_written {
  my($self) = shift;
  return $self->{'files_written'};
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
  $self->{'files_written'}  = [];
  $self->{'project_info'}   = [];
}


sub get_template_input {
  my($self) = shift;

  if ($self->lib_target()) {
    if ($self->{'writing_type'}) {
      return $self->{'lib_template_input'};
    }
    else {
      return $self->{'dll_template_input'};
    }
  }

  return $self->{'exe_template_input'};
}


sub transform_file_name {
  my($self) = shift;
  my($name) = shift;

  $name =~ s/\s/_/g;
  return $name;
}


sub update_project_info {
  my($self)    = shift;
  my($tparser) = shift;
  my($append)  = shift;
  my($names)   = shift;
  my($sep)     = shift;
  my($pi)      = $self->get_project_info();
  my($value)   = "";
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
              (defined $sep && $i != $#narr ? $sep : "");
  }

  ## If we haven't seen this value yet, put it on the array
  if (!defined $self->{'project_info_hash_table'}->{$value}) {
    $self->{'project_info_hash_table'}->{$value} = 1;
    $self->save_project_value("@narr", $value);
    push(@$arr, $value);
  }

  ## Always push the array back onto the project_info
  push(@$pi, $arr);
}


# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub specific_lookup {
  my($self) = shift;
  my($key)  = shift;
  return undef;
}


sub save_project_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = shift;
}


sub get_type_append {
  my($self) = shift;
  return "";
}


sub translate_value {
  my($self) = shift;
  my($key)  = shift;
  my($val)  = shift;
  return $val;
}


sub convert_slashes {
  my($self) = shift;
  return 1;
}


sub crlf {
  my($self) = shift;
  return "\n";
}


sub fill_value {
  my($self) = shift;
  my($name) = shift;
  return undef;
}


sub separate_static_project {
  my($self) = shift;
  return 0;
}


sub project_file_name {
  my($self) = shift;
  return undef;
}


sub static_project_file_name {
  my($self) = shift;
  return undef;
}


sub override_valid_component_extensions {
  my($self) = shift;
  my($comp) = shift;
  return undef;
}


sub override_exclude_component_extensions {
  my($self) = shift;
  my($comp) = shift;
  return undef;
}


sub get_exe_template_input_file {
  my($self) = shift;
  return undef;
}


sub get_lib_template_input_file {
  my($self) = shift;
  return undef;
}


sub get_dll_template_input_file {
  my($self) = shift;
  return undef;
}


sub get_template {
  my($self) = shift;
  return undef;
}


1;
