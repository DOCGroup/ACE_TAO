package Creator;

# ************************************************************
# Description   : Base class for workspace and project creators
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Compare;

use Parser;

use vars qw(@ISA);
@ISA = qw(Parser);

# ************************************************************
# Data Section
# ************************************************************

## Constants for use throughout the project
use constant cplusplus => 'cplusplus';
use constant csharp    => 'csharp';
use constant java      => 'java';
use constant vb        => 'vb';
use constant website   => 'website';

## The default language for MPC
my $deflang = 'cplusplus';

## A map of all of the allowed languages.  The 'website' value
## is not here because it isn't really a language.  It is used
## as a language internally by some project types though.
my %languages = (cplusplus => 1,
                 csharp    => 1,
                 java      => 1,
                 vb        => 1,
                );

my $assign_key  = 'assign';
my $gassign_key = 'global_assign';
my %non_convert = ('prebuild'  => 1,
                   'postbuild' => 1,
                   'postclean' => 1,
                  );
my @statekeys = ('global', 'include', 'template', 'ti',
                 'dynamic', 'static', 'relative', 'addtemp',
                 'addproj', 'progress', 'toplevel', 'baseprojs',
                 'features', 'feature_file', 'hierarchy',
                 'name_modifier', 'apply_project', 'into', 'use_env',
                 'expand_vars', 'language',
                );

my %all_written;
my $onVMS = DirectoryManager::onVMS();

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $global, $inc, $template, $ti, $dynamic, $static, $relative, $addtemp, $addproj, $progress, $toplevel, $baseprojs, $feature, $features, $hierarchy, $nmodifier, $applypj, $into, $language, $use_env, $expandvars, $type) = @_;
  my $self = Parser::new($class, $inc);

  $self->{'relative'}        = $relative;
  $self->{'template'}        = $template;
  $self->{'ti'}              = $ti;
  $self->{'global'}          = $global;
  $self->{'grammar_type'}    = $type;
  $self->{'type_check'}      = $type . '_defined';
  $self->{'global_read'}     = 0;
  $self->{'current_input'}   = '';
  $self->{'progress'}        = $progress;
  $self->{'addtemp'}         = $addtemp;
  $self->{'addproj'}         = $addproj;
  $self->{'toplevel'}        = $toplevel;
  $self->{'files_written'}   = {};
  $self->{'real_fwritten'}   = [];
  $self->{'reading_global'}  = 0;
  $self->{$gassign_key}      = {};
  $self->{$assign_key}       = {};
  $self->{'baseprojs'}       = $baseprojs;
  $self->{'dynamic'}         = $dynamic;
  $self->{'static'}          = $static;
  $self->{'feature_file'}    = $feature;
  $self->{'features'}        = $features;
  $self->{'hierarchy'}       = $hierarchy;
  $self->{'name_modifier'}   = $nmodifier;
  $self->{'apply_project'}   = $applypj;
  $self->{'into'}            = $into;
  $self->{'language'}        = defined $language ? $language : $deflang;
  $self->{'use_env'}         = $use_env;
  $self->{'expand_vars'}     = $expandvars;
  $self->{'convert_slashes'} = $self->convert_slashes();
  $self->{'requires_forward_slashes'} = $self->requires_forward_slashes();
  $self->{'case_tolerant'}   = $self->case_insensitive();

  return $self;
}


sub preprocess_line {
  my($self, $fh, $line) = @_;

  $line = $self->strip_line($line);
  while ($line =~ /\\$/) {
    $line =~ s/\s*\\$/ /;
    my $next = $fh->getline();
    $line .= $self->strip_line($next) if (defined $next);
  }
  return $line;
}


sub generate_default_input {
  my $self  = shift;
  my($status,
     $error) = $self->parse_line(undef, "$self->{'grammar_type'} {");

  ## Parse the finish line if there was no error
  ($status, $error) = $self->parse_line(undef, '}') if ($status);

  ## Display the error if there was one
  $self->error($error) if (!$status);

  return $status;
}


sub parse_file {
  my($self, $input) = @_;

  ## Save the last line number so we can put it back later
  my $oline = $self->get_line_number();

  ## Read the input file
  my($status, $errorString) = $self->read_file($input);

  if (!$status) {
    $self->error($errorString,
                 "$input: line " . $self->get_line_number() . ':');
  }
  elsif ($self->{$self->{'type_check'}}) {
    ## If we are at the end of the file and the type we are looking at
    ## is still defined, then we have an error
    $self->error("Did not " .
                 "find the end of the $self->{'grammar_type'}",
                 "$input: line " . $self->get_line_number() . ':');
    $status = 0;
  }
  $self->set_line_number($oline);

  return $status;
}


sub generate {
  my($self, $input) = @_;
  my $status = 1;

  ## Reset the files_written hash array between processing each file
  $self->{'files_written'} = {};
  $self->{'real_fwritten'} = [];

  ## Allow subclasses to reset values before
  ## each call to generate().
  $self->reset_values();

  ## Read the global configuration file
  if (!$self->{'global_read'}) {
    $status = $self->read_global_configuration();
    $self->{'global_read'} = 1;
  }

  if ($status) {
    $self->{'current_input'} = $input;

    ## An empty input file name says that we
    ## should generate a default input file and use that
    if ($input eq '') {
      $status = $self->generate_default_input();
    }
    else {
      $status = $self->parse_file($input);
    }
  }

  return $status;
}


sub parse_known {
  my($self, $line) = @_;
  my $status = 1;
  my $errorString;
  my $type = $self->{'grammar_type'};
  my @values;

  ##
  ## Each regexp that looks for the '{' looks for it at the
  ## end of the line.  It is purposely this way to decrease
  ## the amount of extra lines in each file.  This
  ## allows for the most compact file as human readably
  ## possible.
  ##
  if ($line eq '') {
  }
  elsif ($line =~ /^$type\s*(\([^\)]+\))?\s*(:.*)?\s*{$/) {
    my $name    = $1;
    my $parents = $2;
    if ($self->{$self->{'type_check'}}) {
      $errorString = "Did not find the end of the $type";
      $status = 0;
    }
    else {
      if (defined $parents) {
        $parents =~ s/^:\s*//;
        $parents =~ s/\s+$//;
        my @parents = split(/\s*,\s*/, $parents);
        if (!defined $parents[0]) {
          ## The : was used, but no parents followed.  This
          ## is an error.
          $errorString = 'No parents listed';
          $status = 0;
        }
        $parents = \@parents;
      }
      push(@values, $type, $name, $parents);
    }
  }
  elsif ($line =~ /^}$/) {
    if ($self->{$self->{'type_check'}}) {
      push(@values, $type, $line);
    }
    else {
      $errorString = "Did not find the beginning of the $type";
      $status = 0;
    }
  }
  elsif ($line =~ /^(feature)\s*\(([^\)]+)\)\s*(:.*)?\s*{$/) {
    my $type    = $1;
    my $name    = $2;
    my $parents = $3;
    my @names   = split(/\s*,\s*/, $name);

    if (defined $parents) {
      $parents =~ s/^:\s*//;
      $parents =~ s/\s+$//;
      my @parents = split(/\s*,\s*/, $parents);
      if (!defined $parents[0]) {
        ## The : was used, but no parents followed.  This
        ## is an error.
        $errorString = 'No parents listed';
        $status = 0;
      }
      $parents = \@parents;
    }
    push(@values, $type, \@names, $parents);
  }
  elsif (!$self->{$self->{'type_check'}}) {
    $errorString = "No $type was defined";
    $status = 0;
  }
  elsif ($self->parse_assignment($line, \@values)) {
    ## If this returns true, then we've found an assignment
  }
  elsif ($line =~ /^(\w+)\s*(\([^\)]+\))?\s*{$/) {
    my $comp = lc($1);
    my $name = $2;

    if (defined $name) {
      $name =~ s/^\(\s*//;
      $name =~ s/\s*\)$//;
    }
    else {
      $name = $self->get_default_component_name();
    }
    push(@values, 'component', $comp, $name);
  }
  else {
    $errorString = "Unrecognized line: $line";
    $status = -1;
  }

  return $status, $errorString, @values;
}


sub parse_scope {
  my($self, $fh, $name, $type, $validNames, $flags, $elseflags) = @_;
  my $status = 0;
  my $errorString = "Unable to process $name";

  ## Make sure $flags has a hash map reference
  $flags = {} if (!defined $flags);

  while(<$fh>) {
    my $line = $self->preprocess_line($fh, $_);

    if ($line eq '') {
    }
    elsif ($line =~ /^}$/) {
      ($status, $errorString) = $self->handle_scoped_end($type, $flags);
      last;
    }
    elsif ($line =~ /^}\s*else\s*{$/) {
      if (defined $elseflags) {
        ## From here on out anything after this goes into the $elseflags
        $flags = $elseflags;
        $elseflags = undef;

        ## We need to adjust the type also.  If there was a type
        ## then the first part of the clause was used.  If there was
        ## no type, then the first part was ignored and the second
        ## part will be used.
        if (defined $type) {
          $type = undef;
        }
        else {
          $type = $self->get_default_component_name();
        }
      }
      else {
        $status = 0;
        $errorString = 'An else is not allowed in this context';
        last;
      }
    }
    else {
      my @values;
      if (defined $validNames && $self->parse_assignment($line, \@values)) {
        if (defined $$validNames{$values[1]}) {
          ## If $type is not defined, we don't even need to bother with
          ## processing the assignment as we will be throwing the value
          ## away anyway.
          if (defined $type) {
            if ($values[0] == 0) {
              $self->process_assignment($values[1], $values[2], $flags);
            }
            elsif ($values[0] == 1) {
              $self->process_assignment_add($values[1], $values[2], $flags);
            }
            elsif ($values[0] == -1) {
              $self->process_assignment_sub($values[1], $values[2], $flags);
            }
          }
        }
        else {
          ($status,
           $errorString) = $self->handle_unknown_assignment($type,
                                                            @values);
          last if (!$status);
        }
      }
      else {
        ($status, $errorString) = $self->handle_scoped_unknown($fh,
                                                               $type,
                                                               $flags,
                                                               $line);
        last if (!$status);
      }
    }
  }
  return $status, $errorString;
}


sub base_directory {
  my $self = shift;
  return $self->mpc_basename($self->getcwd());
}


sub generate_default_file_list {
  my($self, $dir, $exclude, $fileexc, $recurse) = @_;
  my $dh = new FileHandle();
  my @files;

  if (opendir($dh, $dir)) {
    my $prefix   = ($dir ne '.' ? "$dir/" : '');
    my $have_exc = (defined $$exclude[0]);
    my $skip     = 0;
    foreach my $file (grep(!/^\.\.?$/,
                           ($onVMS ? map {$_ =~ s/\.dir$//; $_} readdir($dh) :
                                     readdir($dh)))) {
      ## Prefix each file name with the directory only if it's not '.'
      my $full = $prefix . $file;

      if ($have_exc) {
        foreach my $exc (@$exclude) {
          if ($full eq $exc) {
            $skip = 1;
            last;
          }
        }
      }

      if ($skip) {
        $skip = 0;
        $$fileexc = 1 if (defined $fileexc);
      }
      else {
        if ($recurse && -d $full) {
          push(@files,
               $self->generate_default_file_list($full, $exclude,
                                                 $fileexc, $recurse));
        }
        else {
          # Strip out ^ symbols
          $full =~ s/\^//g if ($onVMS);

          push(@files, $full);
        }
      }
    }

    if ($self->sort_files()) {
      @files = sort { $self->file_sorter($a, $b) } @files;
    }

    closedir($dh);
  }
  return @files;
}


sub transform_file_name {
  my($self, $name) = @_;

  $name =~ s/[\s\-]/_/g;
  return $name;
}


sub file_written {
  my($self, $file) = @_;
  return (defined $all_written{$self->getcwd() . '/' . $file});
}


sub add_file_written {
  my($self, $file) = @_;
  my $key = lc($file);

  if (defined $self->{'files_written'}->{$key}) {
    $self->warning("$self->{'grammar_type'} $file " .
                   ($self->{'case_tolerant'} ?
                           "has been overwritten." :
                           "of differing case has been processed."));
  }
  else {
    $self->{'files_written'}->{$key} = $file;
    push(@{$self->{'real_fwritten'}}, $file);
  }

  $all_written{$self->getcwd() . '/' . $file} = 1;
}


sub extension_recursive_input_list {
  my($self, $dir, $exclude, $ext) = @_;
  my $fh = new FileHandle();
  my @files;

  if (opendir($fh, $dir)) {
    my $prefix = ($dir ne '.' ? "$dir/" : '');
    my $skip   = 0;
    foreach my $file (grep(!/^\.\.?$/,
                           ($onVMS ? map {$_ =~ s/\.dir$//; $_} readdir($fh) :
                                     readdir($fh)))) {
      my $full = $prefix . $file;

      ## Check for command line exclusions
      if (defined $$exclude[0]) {
        foreach my $exc (@$exclude) {
          if ($full eq $exc) {
            $skip = 1;
            last;
          }
        }
      }

      ## If we are not skipping this directory or file, then check it out
      if ($skip) {
        $skip = 0;
      }
      else {
        if (-d $full) {
          push(@files, $self->extension_recursive_input_list($full,
                                                             $exclude,
                                                             $ext));
        }
        elsif ($full =~ /$ext$/) {
          push(@files, $full);
        }
      }
    }
    closedir($fh);
  }

  return @files;
}

sub recursive_directory_list {
  my($self, $dir, $exclude) = @_;
  my $directories = '';
  my $fh = new FileHandle();

  if (opendir($fh, $dir)) {
    my $prefix = ($dir ne '.' ? "$dir/" : '');
    my $skip   = 0;
    if (defined $$exclude[0]) {
      foreach my $exc (@$exclude) {
        if ($dir eq $exc) {
          $skip = 1;
          last;
        }
      }
    }
    if ($skip) {
      $skip = 0;
    }
    else {
      $directories .= ' ' . $dir;
    }

    foreach my $file (grep(!/^\.\.?$/,
                           ($onVMS ? map {$_ =~ s/\.dir$//; $_} readdir($fh) :
                                     readdir($fh)))) {
      my $full = $prefix . $file;

      if ($file eq '.svn' || $file eq 'CVS') {
        $skip = 1;
      }
      else {
        ## Check for command line exclusions
        if (defined $$exclude[0]) {
          foreach my $exc (@$exclude) {
            if ($full eq $exc) {
              $skip = 1;
              last;
            }
          }
        }
      }

      ## If we are not skipping this directory or file, then check it out
      if ($skip) {
        $skip = 0;
      }
      else {
        if (-d $full) {
          $directories .= $self->recursive_directory_list($full, $exclude);
        }
      }
    }
    closedir($fh);
  }

  return $directories;
}


sub modify_assignment_value {
  my($self, $name, $value) = @_;

  if ($self->{'convert_slashes'} &&
      index($name, 'flags') == -1 && !defined $non_convert{$name}) {
    $value =~ s/\//\\/g;
  }

  return $value;
}


sub get_assignment_hash {
  ## NOTE: If anything in this block changes, then you must make the
  ## same change in process_assignment.
  my $self = shift;
  return $self->{$self->{'reading_global'} ? $gassign_key : $assign_key};
}


sub process_assignment {
  my($self, $name, $value, $assign) = @_;

  ## If no hash table was passed in
  if (!defined $assign) {
    ## NOTE: If anything in this block changes, then you must make the
    ## same change in get_assignment_hash.
    $assign = $self->{$self->{'reading_global'} ?
                               $gassign_key : $assign_key};
  }

  if (defined $value) {
    $value =~ s/^\s+//;
    $value =~ s/\s+$//;

    ## Modify the assignment value before saving it
    $$assign{$name} = $self->modify_assignment_value($name, $value);
  }
  else {
    $$assign{$name} = undef;
  }
}


sub addition_core {
  my($self, $name, $value, $nval, $assign) = @_;

  if (defined $nval) {
    if ($self->preserve_assignment_order($name)) {
      $nval .= " $value";
    }
    else {
      $nval = "$value $nval";
    }
  }
  else {
    $nval = $value;
  }
  $self->process_assignment($name, $nval, $assign, 1);
}


sub process_assignment_add {
  my($self, $name, $value, $assign) = @_;
  my $nval = $self->get_assignment_for_modification($name, $assign);

  ## Remove all duplicate parts from the value to be added.
  ## Whether anything gets removed or not is up to the implementation
  ## of the sub classes.
  $value = $self->remove_duplicate_addition($name, $value, $nval);

  ## If there is anything to add, then do so
  $self->addition_core($name, $value, $nval, $assign) if ($value ne '');
}


sub subtraction_core {
  my($self, $name, $value, $nval, $assign) = @_;

  if (defined $nval) {
    my $last  = 1;
    my $found;

    ## Escape any regular expression special characters
    $value = $self->escape_regex_special($value);

    ## If necessary, split the value into an array
    my $elements = ($value =~ /\s/ ? $self->create_array($value) : [$value]);
    for(my $i = 0; $i <= $last; $i++) {
      if ($i == $last) {
        ## If we did not find the string to subtract in the original
        ## value, try again after expanding template variables for
        ## subtraction.
        $nval = $self->get_assignment_for_modification($name, $assign, 1);
      }
      for(my $j = 0; $j <= $last; $j++) {
        ## Try to remove each individual element and then set the new
        ## value if any of the elements were found in the original value
        foreach my $elem (@$elements) {
          ## First try with quotes, then try again without them
          my $re = ($j == 0 ? '"' . $elem . '"' : $elem);

          if ($nval =~ s/\s+$re\s+/ / || $nval =~ s/\s+$re$// ||
              $nval =~ s/^$re\s+//    || $nval =~ s/^$re$//) {
            $found = 1;
          }
        }
        if ($found) {
          $self->process_assignment($name, $nval, $assign, -1);
          last;
        }
      }
      last if ($found);
    }
  }
}


sub process_assignment_sub {
  my($self, $name, $value, $assign) = @_;
  my $nval = $self->get_assignment_for_modification($name, $assign);

  ## Remove double quotes if there are any
  $value =~ s/^\"(.*)\"$/$1/;

  $self->subtraction_core($name, $value, $nval, $assign);
}


sub fill_type_name {
  my($self, $names, $def) = @_;
  my $array = ($names =~ /\s/ ? $self->create_array($names) : [$names]);

  $names = '';
  foreach my $name (@$array) {
    if ($name =~ /\*/) {
      my $pre  = $def . '_';
      my $mid  = '_' . $def . '_';
      my $post = '_' . $def;

      ## Replace the beginning and end first then the middle
      $name =~ s/^\*/$pre/;
      $name =~ s/\*$/$post/;
      $name =~ s/\*/$mid/g;

      ## Remove any trailing underscore or any underscore that is followed
      ## by a space.  This value could be a space separated list.
      $name =~ s/_$//;
      $name =~ s/_\s/ /g;
      $name =~ s/\s_/ /g;

      ## If any one word is capitalized then capitalize each word
      if ($name =~ /[A-Z][0-9a-z_]+/) {
        ## Do the first word
        if ($name =~ /^([a-z])([^_]+)/) {
          my $first = uc($1);
          my $rest  = $2;
          $name =~ s/^[a-z][^_]+/$first$rest/;
        }
        ## Do subsequent words
        while($name =~ /(_[a-z])([^_]+)/) {
          my $first = uc($1);
          my $rest  = $2;
          $name =~ s/_[a-z][^_]+/$first$rest/;
        }
      }
    }

    $names .= $name . ' ';
  }
  $names =~ s/\s+$//;

  return $names;
}


sub clone {
  my($self, $obj) = @_;

  ## Check for various types of data.  Those that are not found to be
  ## types that we need to deep copy are just assigned to new values.
  ## All others are copied by recursively calling this method.
  if (UNIVERSAL::isa($obj, 'HASH')) {
    my $new = {};
    foreach my $key (keys %$obj) {
      $$new{$key} = $self->clone($$obj{$key});
    }
    return $new;
  }  
  elsif (UNIVERSAL::isa($obj, 'ARRAY')) {
    my $new = [];
    foreach my $o (@$obj) {
      push(@$new, $self->clone($o));
    }
    return $new;
  }  

  return $obj;
}                              


sub save_state {
  my($self, $selected) = @_;
  my %state;

  ## Make a deep copy of each state value.  That way our array
  ## references and hash references do not get accidentally modified.
  foreach my $skey (defined $selected ? $selected : @statekeys) {
    if (defined $self->{$skey}) {
      ## It is necessary to clone each value so that nested complex data
      ## types do not get unknowingly modified.
      $state{$skey} = $self->clone($self->{$skey});
    }
  }

  return %state;
}


sub restore_state {
  my($self, $state, $selected) = @_;

  ## Make a deep copy of each state value.  That way our array
  ## references and hash references do not get accidentally modified.
  ## It's not necessary to do a recursive deep copy (i.e., use the
  ## clone() method) because the value coming in will now be owned by
  ## this object and will not be modified unknowingly.
  foreach my $skey (defined $selected ? $selected : @statekeys) {
    my $old = $self->{$skey};
    if (defined $state->{$skey} &&
        UNIVERSAL::isa($state->{$skey}, 'ARRAY')) {
      my @arr = @{$state->{$skey}};
      $self->{$skey} = \@arr;
    }
    elsif (defined $state->{$skey} &&
           UNIVERSAL::isa($state->{$skey}, 'HASH')) {
      my %hash = %{$state->{$skey}};
      $self->{$skey} = \%hash;
    }
    else {
      $self->{$skey} = $state->{$skey};
    }
    $self->restore_state_helper($skey, $old, $self->{$skey});
  }
}


sub get_global_cfg {
  return $_[0]->{'global'};
}


sub get_template_override {
  return $_[0]->{'template'};
}


sub get_ti_override {
  return $_[0]->{'ti'};
}


sub get_relative {
  return $_[0]->{'relative'};
}


sub get_progress_callback {
  return $_[0]->{'progress'};
}


sub get_addtemp {
  return $_[0]->{'addtemp'};
}


sub get_addproj {
  return $_[0]->{'addproj'};
}


sub get_toplevel {
  return $_[0]->{'toplevel'};
}


sub get_into {
  return $_[0]->{'into'};
}


sub get_use_env {
  return $_[0]->{'use_env'};
}


sub get_expand_vars {
  return $_[0]->{'expand_vars'};
}


sub get_files_written {
  return $_[0]->{'real_fwritten'};
}


sub get_assignment {
  my $self   = shift;
  my $name = $self->resolve_alias(shift);
  my $assign = shift;

  ## If no hash table was passed in
  if (!defined $assign) {
    $assign = $self->{$self->{'reading_global'} ?
                              $gassign_key : $assign_key};
  }

  return $$assign{$name};
}


sub get_assignment_for_modification {
  my($self, $name, $assign, $subtraction) = @_;
  return $self->get_assignment($name, $assign);
}


sub get_baseprojs {
  return $_[0]->{'baseprojs'};
}


sub get_dynamic {
  return $_[0]->{'dynamic'};
}


sub get_static {
  return $_[0]->{'static'};
}


sub get_default_component_name {
  #my $self = shift;
  return 'default';
}


sub get_features {
  return $_[0]->{'features'};
}


sub get_hierarchy {
  return $_[0]->{'hierarchy'};
}


sub get_name_modifier {
  return $_[0]->{'name_modifier'};
}


sub get_apply_project {
  return $_[0]->{'apply_project'};
}


sub get_language {
  return $_[0]->{'language'};
}


sub get_outdir {
  my $self = shift;
  if (defined $self->{'into'}) {
    my $outdir = $self->getcwd();
    my $re = $self->escape_regex_special($self->getstartdir());

    $outdir =~ s/^$re//;
    return $self->{'into'} . $outdir;
  }
  else {
    return '.';
  }
}


sub expand_variables {
  my($self, $value, $rel, $expand_template, $scope, $expand, $warn) = @_;
  my $cwd = $self->getcwd();
  my $start = 0;
  my $forward_slashes  = $self->{'convert_slashes'} ||
                         $self->{'requires_forward_slashes'};

  ## Fix up the value for Windows switch the \\'s to /
  $cwd =~ s/\\/\//g if ($forward_slashes);

  while(substr($value, $start) =~ /(\$\(([^)]+)\))/) {
    my $whole = $1;
    my $name  = $2;
    if (defined $$rel{$name}) {
      my $val = $$rel{$name};
      if ($expand) {
        $val =~ s/\//\\/g if ($forward_slashes);
        substr($value, $start) =~ s/\$\([^)]+\)/$val/;
        $whole = $val;
      }
      else {
        ## Fix up the value for Windows switch the \\'s to /
        $val =~ s/\\/\//g if ($forward_slashes);

        my $icwd = ($self->{'case_tolerant'} ? lc($cwd) : $cwd);
        my $ival = ($self->{'case_tolerant'} ? lc($val) : $val);
        my $iclen = length($icwd);
        my $ivlen = length($ival);

        ## If the relative value contains the current working
        ## directory plus additional subdirectories, we must pull
        ## off the additional directories into a temporary where
        ## it can be put back after the relative replacement is done.
        my $append;
        if (index($ival, $icwd) == 0 && $iclen != $ivlen &&
            substr($ival, $iclen, 1) eq '/') {
          my $diff = $ivlen - $iclen;
          $append = substr($ival, $iclen);
          substr($ival, $iclen, $diff) = '';
          $ivlen -= $diff;
        }

        if (index($icwd, $ival) == 0 &&
            ($iclen == $ivlen || substr($icwd, $ivlen, 1) eq '/')) {
          my $current = $icwd;
          substr($current, 0, $ivlen) = '';

          my $dircount = ($current =~ tr/\///);
          if ($dircount == 0) {
            $ival = '.';
          }
          else {
            $ival = '../' x $dircount;
            $ival =~ s/\/$//;
          }
          $ival .= $append if (defined $append);

          ## We have to remove the leading ./ if there is one.
          ## Otherwise, if this value is used as an exclude value it will
          ## not match up correctly.
          $ival =~ s!^\./!!;

          ## Convert the slashes if necessary
          $ival =~ s/\//\\/g if ($self->{'convert_slashes'});
          substr($value, $start) =~ s/\$\([^)]+\)/$ival/;
          $whole = $ival;
        }
        elsif ($self->convert_all_variables() && $warn) {
          ## The user did not choose to expand $() variables directly,
          ## but we could not convert it into a relative path.  So,
          ## instead of leaving it we will expand it.  But, we will only
          ## get into this section if this is the secondary attempt to
          ## replace the variable (indicated by the $warn boolean).
          $val =~ s/\//\\/g if ($self->{'convert_slashes'});
          substr($value, $start) =~ s/\$\([^)]+\)/$val/;
          $whole = $val;
        }
        else {
          my $loc = index(substr($value, $start), $whole);
          $start += $loc if ($loc > 0);
        }
      }
    }
    elsif ($expand_template ||
           $self->expand_variables_from_template_values()) {
      my $ti = $self->get_template_input();
      my $val = (defined $ti ? $ti->get_value($name) : undef);
      my $sname = (defined $scope ? $scope . "::$name" : undef);
      my $arr = $self->adjust_value([$sname, $name],
                                    (defined $val ? $val : []));
      if (UNIVERSAL::isa($arr, 'HASH')) {
        $self->warning("$name conflicts with a template variable scope");
      }
      elsif (UNIVERSAL::isa($arr, 'ARRAY') && defined $$arr[0]) {
        $val = $self->modify_assignment_value(lc($name), "@$arr");
        substr($value, $start) =~ s/\$\([^)]+\)/$val/;

        ## We have replaced the template value, but that template
        ## value may contain a $() construct that may need to get
        ## replaced too.  However, if the name of the template variable
        ## is the same as the original $() variable name, we need to
        ## leave it alone to avoid looping infinitely.
        $whole = '' if ($whole ne $val);
      }
      else {
        $self->warning("Unable to expand $name.") if ($expand && $warn);
        my $loc = index(substr($value, $start), $whole);
        $start += $loc if ($loc > 0);
      }
    }
    elsif ($self->convert_all_variables() && $warn) {
      ## We could not find a value to correspond to the variable name.
      ## Instead of leaving it we will expand it.  But, we will only
      ## get into this section if this is the secondary attempt to
      ## replace the variable (indicated by the $warn boolean).
      substr($value, $start) =~ s/\$\([^)]+\)//;
      $whole = '';
    }
    else {
      my $loc = index(substr($value, $start), $whole);
      $start += $loc if ($loc > 0);
    }
    $start += length($whole);
  }

  $value =~ s/\\/\//g if ($self->{'requires_forward_slashes'});

  return $value;
}


sub replace_env_vars {
  my($self, $lref) = @_;
  my $one_empty = undef;

  ## Loop through the string until we find no more environment variables.
  while($$lref =~ /\$(\w+)/) {
    my $name = $1;
    my $val  = '';

    ## PWD is a special variable.  It isn't set on Windows, but in MPC we
    ## must guarantee that it is always there.
    if ($name eq 'PWD') {
      $val = $self->getcwd();
    }
    elsif (defined $ENV{$name}) {
      $val = $ENV{$name};
    }
    else {
      ## Keep track of an environment variable not being set.
      $one_empty = 1;
    }
    $$lref =~ s/\$\w+/$val/;
  }
  return $one_empty;
}


sub relative {
  my($self, $value, $expand_template, $scope) = @_;

  if (defined $value) {
    if (UNIVERSAL::isa($value, 'ARRAY')) {
      my @built;
      foreach my $val (@$value) {
        my $rel = $self->relative($val, $expand_template, $scope);
        if (UNIVERSAL::isa($rel, 'ARRAY')) {
          push(@built, @$rel);
        }
        else {
          push(@built, $rel);
        }
      }
      return \@built;
    }
    elsif (index($value, '$') >= 0) {
      ## A form of this code lives in
      ## ProjectCreator::create_recursive_settings.  If you are changing
      ## something in this area, please look at the method in
      ## ProjectCreator.pm to see if it needs changing too.

      my $ovalue = $value;
      my($rel, $how) = $self->get_initial_relative_values();
      $value = $self->expand_variables($value, $rel,
                                       $expand_template, $scope, $how);

      if ($ovalue eq $value || index($value, '$') >= 0) {
        ($rel, $how) = $self->get_secondary_relative_values();
        $value = $self->expand_variables($value, $rel,
                                         $expand_template, $scope,
                                         $how, 1);
      }
    }
  }

  ## Values that have two or more strings enclosed in double quotes are
  ## to be interpreted as elements of an array
  if (defined $value && $value =~ /^"[^"]+"(\s+"[^"]+")+$/) {
    $value = $self->create_array($value);
  }

  return $value;
}


## Static function.  Returns the default language for MPC.
sub defaultLanguage {
  return $deflang;
}


## Static function.  Returns an array of valid languages.
sub validLanguages {
  return keys %languages;
}


## Static function.  The one and only argument is the language
## string to check for validity.
sub isValidLanguage {
  return defined $languages{$_[0]};
}


sub languageIs {
  #my($self, $language) = @_;
  return $_[0]->{'language'} eq $_[1];
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub restore_state_helper {
  #my $self = shift;
  #my $skey = shift;
  #my $old  = shift;
  #my $new  = shift;
}


sub get_initial_relative_values {
  #my $self = shift;
  return {}, 0;
}


sub get_secondary_relative_values {
  my $self = shift;
  return ($self->{'use_env'} ? \%ENV :
                               $self->{'relative'}), $self->{'expand_vars'};
}


sub convert_all_variables {
  #my $self = shift;
  return 0;
}


sub expand_variables_from_template_values {
  #my $self = shift;
  return 0;
}


sub preserve_assignment_order {
  #my $self = shift;
  #my $name = shift;
  return 1;
}


sub compare_output {
  #my $self = shift;
  return 0;
}


sub files_are_different {
  my($self, $old, $new) = @_;
  return !(-r $old && -s $new == -s $old && compare($new, $old) == 0);
}


sub handle_scoped_end {
  #my $self  = shift;
  #my $type  = shift;
  #my $flags = shift;
  return 1, undef;
}

sub handle_unknown_assignment {
  my $self   = shift;
  my $type   = shift;
  my @values = @_;
  return 0, "Invalid assignment name: '$values[1]'";
}


sub handle_scoped_unknown {
  my($self, $fh, $type, $flags, $line) = @_;
  return 0, "Unrecognized line: $line";
}


sub remove_duplicate_addition {
  my($self, $name, $value, $current) = @_;
  return $value;
}


sub generate_recursive_input_list {
  #my $self    = shift;
  #my $dir     = shift;
  #my $exclude = shift;
  return ();
}


sub reset_values {
  #my $self = shift;
}


sub sort_files {
  #my $self = shift;
  return 1;
}


sub file_sorter {
  #my $self  = shift;
  #my $left  = shift;
  #my $right = shift;
  return $_[1] cmp $_[2];
}


sub read_global_configuration {
  #my $self  = shift;
  #my $input = shift;
  return 1;
}


sub set_verbose_ordering {
  #my $self  = shift;
  #my $value = shift;
}


1;
