package TemplateParser;

# ************************************************************
# Description   : Parses the template and fills in missing values
# Author        : Chad Elliott
# Create Date   : 5/17/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use Parser;

use vars qw(@ISA);
@ISA = qw(Parser);

# ************************************************************
# Data Section
# ************************************************************

my(%keywords) = ('if'              => 1,
                 'else'            => 1,
                 'endif'           => 1,
                 'noextension'     => 1,
                 'dirname'         => 1,
                 'basename'        => 1,
                 'basenoextension' => 1,
                 'foreach'         => 1,
                 'forfirst'        => 1,
                 'fornotfirst'     => 1,
                 'fornotlast'      => 1,
                 'forlast'         => 1,
                 'endfor'          => 1,
                 'comment'         => 1,
                 'flag_overrides'  => 1,
                 'marker'          => 1,
                );

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($prjc)  = shift;
  my($self)  = Parser::new($class);

  $self->{'prjc'}     = $prjc;
  $self->{'ti'}       = $prjc->get_template_input();
  $self->{'crlf'}     = undef;
  $self->{'values'}   = {};
  $self->{'defaults'} = {};
  $self->{'lines'}    = [];
  $self->{'built'}    = '';
  $self->{'sstack'}   = [];
  $self->{'lstack'}   = [];
  $self->{'if_skip'}  = 0;

  $self->{'foreach'}  = {};
  $self->{'foreach'}->{'count'}      = -1;
  $self->{'foreach'}->{'nested'}     = 0;
  $self->{'foreach'}->{'name'}       = [];
  $self->{'foreach'}->{'names'}      = [];
  $self->{'foreach'}->{'text'}       = [];
  $self->{'foreach'}->{'scope'}      = [];
  $self->{'foreach'}->{'temp_scope'} = [];
  $self->{'foreach'}->{'processing'} = 0;

  return $self;
}


sub basename {
  my($self) = shift;
  my($file) = shift;
  for(my $i = length($file) - 1; $i >= 0; --$i) {
    my($ch) = substr($file, $i, 1);
    if ($ch eq '/' || $ch eq '\\') {
      ## The template file may use this value (<%basename_found%>)
      ## to determine whether a basename removed the directory or not
      $self->{'values'}->{'basename_found'} = 1;
      return substr($file, $i + 1);
    }
  }
  delete $self->{'values'}->{'basename_found'};
  return $file;
}


sub dirname {
  my($self) = shift;
  my($file) = shift;
  for(my $i = length($file) - 1; $i != 0; --$i) {
    my($ch) = substr($file, $i, 1);
    if ($ch eq '/' || $ch eq '\\') {
      ## The template file may use this value (<%dirname_found%>)
      ## to determine whether a dirname removed the basename or not
      $self->{'values'}->{'dirname_found'} = 1;
      return substr($file, 0, $i);
    }
  }
  delete $self->{'values'}->{'dirname_found'};
  return '.';
}


sub strip_line {
  my($self) = shift;
  my($line) = shift;

  ## Override strip_line() from Parser.
  ## We need to preserve leading space and
  ## there is no comment string in templates.
  ++$self->{'line_number'};
  $line =~ s/\s+$//;

  return $line;
}


## Append the current value to the line that is being
## built.  This line may be a foreach line or a general
## line without a foreach.
sub append_current {
  my($self)  = shift;
  my($value) = shift;
  my($index) = $self->{'foreach'}->{'count'};

  if ($index >= 0) {
    $self->{'foreach'}->{'text'}->[$index] .= $value;
  }
  else {
    $self->{'built'} .= $value;
  }
}


sub adjust_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = shift;

  ## Perform any additions, subtractions
  ## or overrides for the template values.
  my($addtemp) = $self->{'prjc'}->get_addtemp();
  foreach my $at (keys %$addtemp) {
    if ($at eq $name) {
      my($val) = $$addtemp{$at};
      if ($$val[0] > 0) {
        if (UNIVERSAL::isa($value, 'ARRAY')) {
          $value = [ $$val[1], @$value ];
        }
        else {
          $value = "$$val[1] $value";
        }
      }
      elsif ($$val[0] < 0) {
        my($parts) = undef;
        if (UNIVERSAL::isa($value, 'ARRAY')) {
          my(@copy) = @$value;
          $parts = \@copy;
        }
        else {
          $parts = $self->create_array($value);
        }

        $value = '';
        foreach my $part (@$parts) {
          if ($part ne $$val[1] && $part ne '') {
            $value .= "$part ";
          }
        }
        $value =~ s/^\s+//;
        $value =~ s/\s+$//;
      }
      else {
        $value = $$val[1];
      }
    }
  }

  return $value;
}


sub set_current_values {
  my($self) = shift;
  my($name) = shift;
  my($set)  = 0;

  ## If any value within a foreach matches the name
  ## of a hash table within the template input we will
  ## set the values of that hash table in the current scope
  if (defined $self->{'ti'}) {
    my($counter) = $self->{'foreach'}->{'count'};
    if ($counter >= 0) {
      my($value) = $self->{'ti'}->get_value($name);
      if (defined $value && UNIVERSAL::isa($value, 'HASH')) {
        my(%copy) = ();
        foreach my $key (keys %$value) {
          $copy{$key} = $self->adjust_value($key, $$value{$key});
        }
        $self->{'foreach'}->{'temp_scope'}->[$counter] = \%copy;
        $set = 1;
      }
    }
  }
  return $set;
}


sub relative {
  my($self)  = shift;
  my($value) = shift;
  my($rel)   = $self->{'prjc'}->get_relative();
  my(@keys)  = keys %$rel;

  if (defined $value && defined $keys[0]) {
    if (UNIVERSAL::isa($value, 'ARRAY')) {
      my(@built) = ();
      foreach my $val (@$value) {
        push(@built, $self->relative($val));
      }
      $value = \@built;
    }
    elsif ($value =~ /\$/) {
      my($cwd)   = $self->getcwd();
      my($start) = 0;
      my($fixed) = 0;

      if ($cwd =~ /[a-z]:[\/\\]/) {
        substr($cwd, 0, 1) = uc(substr($cwd, 0, 1));
      }

      while(substr($value, $start) =~ /(\$\(([^)]+)\))/) {
        my($whole)  = $1;
        my($name)   = $2;
        my($val)    = $$rel{$name};

        if (defined $val) {
          if ($^O eq 'cygwin' && !$fixed &&
              $cwd !~ /[A-Za-z]:/ && $val =~ /[A-Za-z]:/) {
            my($cyg) = `cygpath -w $cwd`;
            if (defined $cyg) {
              $cyg =~ s/\\/\//g;
              chop($cwd = $cyg);
              $fixed = 1;
            }
          }

          ## Fix up the value for Windows (capitalize the drive and
          ## switch the \\'s to /
          $val =~ s/\\/\//g;
          if ($val =~ /[a-z]:\//) {
            substr($val, 0, 1) = uc(substr($val, 0, 1));
          }

          if (index($cwd, $val) == 0) {
            my($count)   = 0;
            my($current) = $cwd;
            substr($current, 0, length($val)) = '';
            while($current =~ /^\\/) {
              $current =~ s/^\///;
            }
            my($length) = length($current);
            for(my $i = 0; $i < $length; ++$i) {
              if (substr($current, $i, 1) eq '/') {
                ++$count;
              }
            }
            $val = '../' x $count;
            $val =~ s/\/$//;
            if ($self->{'prjc'}->convert_slashes()) {
              $val = $self->slash_to_backslash($val);
            }
            substr($value, $start) =~ s/\$\([^)]+\)/$val/;
            $whole = $val;
          }
        }
        $start += length($whole);
      }
    }
  }

  return $value;
}


sub get_nested_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name =~ /^(.*)\->(\w+)/) {
    my($pre)  = $1;
    my($post) = $2;
    my($base) = $self->get_value($pre);
    if (defined $base) {
      $value = $self->{'prjc'}->get_custom_value($post, $base);
    }
  }

  return $value;
}


sub get_value {
  my($self)    = shift;
  my($name)    = shift;
  my($value)   = undef;
  my($counter) = $self->{'foreach'}->{'count'};

  ## First, check the temporary scope (set inside a foreach)
  if ($counter >= 0) {
    while(!defined $value && $counter >= 0) {
      $value = $self->{'foreach'}->{'temp_scope'}->[$counter]->{$name};
      --$counter;
    }
    $counter = $self->{'foreach'}->{'count'};
  }

  if (!defined $value) {
    ## Next, check for a template value
    if (defined $self->{'ti'}) {
      $value = $self->{'ti'}->get_value($name);
      if (defined $value) {
        $value = $self->adjust_value($name, $value);
      }
    }

    if (!defined $value) {
      ## Next, check the inner to outer foreach
      ## scopes for overriding values
      while(!defined $value && $counter >= 0) {
        $value = $self->{'foreach'}->{'scope'}->[$counter]->{$name};
        --$counter;
      }

      ## Then get the value from the project creator
      if (!defined $value) {
        $value = $self->{'prjc'}->get_assignment($name);

        ## Then get it from our known values
        if (!defined $value) {
          $value = $self->{'values'}->{$name};
          if (!defined $value) {
            ## Call back onto the project creator to allow
            ## it to fill in the value before defaulting to undef.
            $value = $self->{'prjc'}->fill_value($name);
            if (!defined $value && $name =~ /\->/) {
              $value = $self->get_nested_value($name);
            }
          }
        }
      }
    }
  }

  return $self->relative($value);
}


sub get_value_with_default {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = $self->get_value($name);

  if (defined $value) {
    if (UNIVERSAL::isa($value, 'ARRAY')) {
      $value = "@$value";
    }
  }
  else {
    $value = $self->{'defaults'}->{$name};
    if (!defined $value) {
#      print "DEBUG: WARNING: $name defaulting to empty string\n";
      $value = '';
    }
    else {
#      print "DEBUG: WARNING: $name using default value of $value\n";
      $value = $self->adjust_value($name, $value);
    }
    $value = $self->relative($value);
  }

  return $value;
}


sub process_foreach {
  my($self)   = shift;
  my($index)  = $self->{'foreach'}->{'count'};
  my($text)   = $self->{'foreach'}->{'text'}->[$index];
  my($status) = 1;
  my($errorString) = '';
  my(@values) = ();
  my($names)  = $self->create_array($self->{'foreach'}->{'names'}->[$index]);
  my($name)   = $self->{'foreach'}->{'name'}->[$index];

  foreach my $n (@$names) {
    my($vals) = $self->get_value($n);
    if (defined $vals && $vals ne '') {
      if (!UNIVERSAL::isa($vals, 'ARRAY')) {
        $vals = $self->create_array($vals);
      }
      push(@values, @$vals);
    }
    if (!defined $name) {
      $name = $n;
      $name =~ s/s$//;
    }
  }

  ## Reset the text (it will be regenerated by calling parse_line
  $self->{'foreach'}->{'text'}->[$index] = '';

  if (defined $values[0]) {
    my($scope) = $self->{'foreach'}->{'scope'}->[$index];

    $$scope{'forlast'}     = 0;
    $$scope{'fornotlast'}  = 1;
    $$scope{'forfirst'}    = 1;
    $$scope{'fornotfirst'} = 0;

    ## If the foreach values are mixed (HASH and SCALAR), then
    ## remove the SCALAR values.
    my($pset) = undef;
    for(my $i = 0; $i <= $#values; ++$i) {
      my($set) = $self->set_current_values($values[$i]);
      if (!defined $pset) {
        $pset |= $set;
      }
      else {
        if ($pset && !$set) {
          splice(@values, $i, 1);
          $i = 0;
          $pset = undef;
        }
      }
    }

    for(my $i = 0; $i <= $#values; ++$i) {
      my($value) = $values[$i];

      ## Set the corresponding values in the temporary scope
      $self->set_current_values($value);

      ## Set the special values that only exist
      ## within a foreach
      if ($i != 0) {
        $$scope{'forfirst'}    = 0;
        $$scope{'fornotfirst'} = 1;
      }
      if ($i == $#values) {
        $$scope{'forlast'}    = 1;
        $$scope{'fornotlast'} = 0;
      }
      ## We don't use adjust_value here because these names
      ## are generated from a foreach and should not be adjusted.
      $$scope{$name} = $value;

      ## A tiny hack for VC7
      if ($name eq 'configuration') {
        $self->{'prjc'}->update_project_info($self, 1,
                                             ['configuration', 'platform'],
                                             '|');
      }

      ## Now parse the line of text, each time
      ## with different values
      ++$self->{'foreach'}->{'processing'};
      ($status, $errorString) = $self->parse_line(undef, $text);
      --$self->{'foreach'}->{'processing'};
      if (!$status) {
        last;
      }
    }
  }

  return $status, $errorString;
}


sub handle_end {
  my($self)        = shift;
  my($name)        = shift;
  my($status)      = 1;
  my($errorString) = '';
  my($end)         = pop(@{$self->{'sstack'}});
  pop(@{$self->{'lstack'}});

  if (!defined $end) {
    $status = 0;
    $errorString = "ERROR: Unmatched $name\n";
  }
  elsif ($end eq 'endif') {
    $self->{'if_skip'} = 0;
  }
  elsif ($end eq 'endfor') {
    my($index) = $self->{'foreach'}->{'count'};
    ($status, $errorString) = $self->process_foreach();
    if ($status) {
      --$self->{'foreach'}->{'count'};
      $self->append_current($self->{'foreach'}->{'text'}->[$index]);
    }
  }

  return $status, $errorString;
}


sub get_flag_overrides {
  my($self)  = shift;
  my($name)  = shift;
  my($type)  = shift;
  my($value) = undef;
  my($file)  = $self->get_value($name);
  my($prjc)  = $self->{'prjc'};
  my($fo)    = $prjc->{'flag_overrides'};

  if (defined $file) {
    ## Replace the custom_type key with the actual custom type
    if ($name =~ /^custom_type\->/) {
      my($ct) = $self->get_value('custom_type');
      if (defined $ct) {
        $name = $ct;
      }
    }

    foreach my $key (keys %$fo) {
      if ($key =~ /^$name/) {
        foreach my $of (keys %{$$fo{$key}}) {
          my($cv) = $of;
          if ($prjc->convert_slashes()) {
            $cv = $prjc->slash_to_backslash($of);
          }
          if ($cv eq $file) {
            foreach my $ma (keys %{$prjc->{'matching_assignments'}}) {
              if ($ma eq $key) {
                foreach my $aname (@{$prjc->{'matching_assignments'}->{$ma}}) {
                  if ($aname eq $type &&
                      defined $$fo{$key}->{$of}->{$aname}) {
                    $value = $$fo{$key}->{$of}->{$aname};
                    last;
                  }
                }
                last;
              }
            }
            last;
          }
        }
        last;
      }
    }
  }

  return $value;
}


sub handle_if {
  my($self)   = shift;
  my($val)    = shift;
  my($name)   = 'endif';

  push(@{$self->{'lstack'}}, $self->line_number() . " $val");
  if (!$self->{'if_skip'}) {
    my($true)  = 1;
    push(@{$self->{'sstack'}}, $name);
    if ($val =~ /^!(.*)/) {
      $val = $1;
      $val =~ s/^\s+//;
      $true = 0;
    }

    if ($val =~ /flag_overrides\(([^\)]+),\s*([^\)]+)\)/) {
      $val = $self->get_flag_overrides($1, $2);
    }
    else {
      $val = $self->get_value($val)
    }

    if (defined $val) {
      if (UNIVERSAL::isa($val, 'ARRAY')) {
        my($empty) = 1;
        foreach my $v (@$val) {
          if ($v ne '') {
            $empty = 0;
            last;
          }
        }
        if ($empty) {
          $val = undef;
        }
      }
      elsif ($val eq '') {
        $val = undef;
      }
    }

    if (!defined $val) {
      $self->{'if_skip'} = $true;
    }
    else {
      $self->{'if_skip'} = !$true;
    }
  }
  else {
    push(@{$self->{'sstack'}}, "*$name");
  }
}


sub handle_else {
  my($self)  = shift;
  my(@scopy) = @{$self->{'sstack'}};

  ## This method does not take into account that
  ## multiple else clauses could be supplied to a single if.
  ## Someday, this may be fixed.
  if (defined $scopy[$#scopy] && $scopy[$#scopy] eq 'endif') {
    $self->{'if_skip'} ^= 1;
  }
}


sub handle_foreach {
  my($self)        = shift;
  my($val)         = shift;
  my($name)        = 'endfor';
  my($status)      = 1;
  my($errorString) = '';

  push(@{$self->{'lstack'}}, $self->line_number());
  if (!$self->{'if_skip'}) {
    my($vname) = undef;
    if ($val =~ /([^,]+),(.*)/) {
      $vname = $1;
      $val   = $2;
      $vname =~ s/^\s+//;
      $vname =~ s/\s+$//;
      $val   =~ s/^\s+//;
      $val   =~ s/\s+$//;

      ## Due to the way flag_overrides works, we can't allow
      ## the user to name the foreach variable when dealing
      ## with custom types.
      if ($val =~ /^custom_type\->/ || $val eq 'custom_types') {
        $status = 0;
        $errorString = 'ERROR: The foreach variable can not be ' .
                       'named when dealing with custom types';
      }
    }

    push(@{$self->{'sstack'}}, $name);
    ++$self->{'foreach'}->{'count'};

    my($index) = $self->{'foreach'}->{'count'};
    $self->{'foreach'}->{'name'}->[$index]  = $vname;
    $self->{'foreach'}->{'names'}->[$index] = $val;
    $self->{'foreach'}->{'text'}->[$index]  = '';
    $self->{'foreach'}->{'scope'}->[$index] = {};
  }
  else {
    push(@{$self->{'sstack'}}, "*$name");
  }

  return $status, $errorString;
}


sub handle_special {
  my($self) = shift;
  my($name) = shift;
  my($val)  = shift;

  ## If $name (fornotlast, forfirst, etc.) is set to 1
  ## Then we append the $val onto the current string that's
  ## being built.
  if (!$self->{'if_skip'}) {
    if ($self->get_value($name)) {
      $self->append_current($val);
    }
  }
}


sub handle_noextension {
  my($self) = shift;
  my($name) = shift;

  if (!$self->{'if_skip'}) {
    my($val) = $self->get_value_with_default($name);
    $val =~ s/\.[^\.]+$//;
    $self->append_current($val);
  }
}


sub handle_dirname {
  my($self) = shift;
  my($name) = shift;

  if (!$self->{'if_skip'}) {
    my($val) = $self->dirname($self->get_value_with_default($name));
    $self->append_current($val);
  }
}


sub handle_basename {
  my($self) = shift;
  my($name) = shift;

  if (!$self->{'if_skip'}) {
    my($val) = $self->basename($self->get_value_with_default($name));
    $self->append_current($val);
  }
}


sub handle_basenoextension {
  my($self) = shift;
  my($name) = shift;

  if (!$self->{'if_skip'}) {
    my($val) = $self->basename($self->get_value_with_default($name));
    $val =~ s/\.[^\.]+$//;
    $self->append_current($val);
  }
}


sub handle_flag_overrides {
  my($self) = shift;
  my($name) = shift;
  my($type) = '';

  ($name, $type) = split(/,\s*/, $name);

  if (!$self->{'if_skip'}) {
    my($value) = $self->get_flag_overrides($name, $type);
    if (defined $value) {
      $self->append_current($value);
    }
  }
}


sub handle_marker {
  my($self) = shift;
  my($name) = shift;

  if (!$self->{'if_skip'}) {
    my($value) = $self->{'prjc'}->get_verbatim($name);
    if (defined $value) {
      $self->append_current($value);
    }
  }
}


## Given a line that starts with an identifier, we split
## then name from the possible value stored inside ()'s and
## we stop looking at the line when we find the %> ending
sub split_name_value {
  my($self)   = shift;
  my($line)   = shift;
  my($length) = length($line);
  my($name)   = undef;
  my($val)    = undef;

  for(my $i = 0; $i < $length; ++$i) {
    my($ch) = substr($line, $i, 1);
    if (!defined $name && $ch eq '(') {
      $name = substr($line, 0, $i);
      $val  = '';
    }
    elsif (!defined $name && $ch eq '%') {
      if (substr($line, $i + 1, 1) eq '>') {
        $name = substr($line, 0, $i);
        last;
      }
    }
    elsif (defined $val && $ch ne ')') {
      $val .= $ch;
    }
    elsif (defined $val && $ch eq ')') {
      if (substr($line, $i + 1, 2) eq '%>') {
        last;
      }
      else {
        $val .= $ch;
      }
    }
  }

  return $name, $val;
}


sub process_name {
  my($self)        = shift;
  my($line)        = shift;
  my($length)      = 0;
  my($status)      = 1;
  my($errorString) = '';

  if ($line eq '') {
  }
  elsif ($line =~ /^(\w+)(\(([^\)]+|\".*\"|flag_overrides\([^\)]+,\s*[^\)]+\))\)|\->\w+([\w\-\>]+)?)?%>/) {
    my($name, $val) = $self->split_name_value($line);

    $length += length($name);
    if (defined $val) {
      $length += length($val) + 2;
    }

    if (defined $keywords{$name}) {
      if ($name eq 'endif' || $name eq 'endfor') {
        ($status, $errorString) = $self->handle_end($name);
      }
      elsif ($name eq 'if') {
        $self->handle_if($val);
      }
      elsif ($name eq 'else') {
        $self->handle_else();
      }
      elsif ($name eq 'foreach') {
        ($status, $errorString) = $self->handle_foreach($val);
      }
      elsif ($name eq 'fornotlast'  || $name eq 'forlast' ||
             $name eq 'fornotfirst' || $name eq 'forfirst') {
        $self->handle_special($name, $self->process_special($val));
      }
      elsif ($name eq 'comment') {
        ## Ignore the contents of the comment
      }
      elsif ($name eq 'flag_overrides') {
        $self->handle_flag_overrides($val);
      }
      elsif ($name eq 'marker') {
        $self->handle_marker($val);
      }
      elsif ($name eq 'noextension') {
        $self->handle_noextension($val);
      }
      elsif ($name eq 'dirname') {
        $self->handle_dirname($val);
      }
      elsif ($name eq 'basename') {
        $self->handle_basename($val);
      }
      elsif ($name eq 'basenoextension') {
        $self->handle_basenoextension($val);
      }
    }
    else {
      if (!$self->{'if_skip'}) {
        if (defined $val && !defined $self->{'defaults'}->{$name}) {
          $self->{'defaults'}->{$name} = $self->process_special($val);
        }

        $val = $self->get_value_with_default($name);
        $self->append_current($val);
      }
    }
  }
  else {
    my($error)  = $line;
    my($length) = length($line);
    for(my $i = 0; $i < $length; ++$i) {
      my($part) = substr($line, $i, 2);
      if ($part eq '%>') {
        $error = substr($line, 0, $i + 2);
        last;
      }
    }
    $status = 0;
    $errorString = "ERROR: Unable to parse line starting at $error";
  }

  return $status, $errorString, $length;
}


sub collect_data {
  my($self) = shift;
  my($prjc) = $self->{'prjc'};

  ## Save crlf so we don't have to keep going back to the prjc
  $self->{'crlf'} = $prjc->crlf();

  ## Collect the components into {'values'} somehow
  foreach my $key (keys %{$prjc->{'valid_components'}}) {
    my(@list) = $prjc->get_component_list($key);
    if (defined $list[0]) {
      $self->{'values'}->{$key} = "@list";
    }
  }

  ## A tiny hack (mainly for VC6 projects)
  ## for the workspace generator.  It needs to know the
  ## target names to match up with the project name.
  $prjc->update_project_info($self, 0, ['project_name']);

  ## This is for all projects
  $prjc->update_project_info($self, 1, ['depends']);

  ## VC7 Projects need to know the GUID.
  ## We need to save this value in our known values
  ## since each guid generated will be different.  We need
  ## this to correspond to the same guid used in the workspace.
  my($guid) = $prjc->update_project_info($self, 1, ['guid']);
  $self->{'values'}->{'guid'} = $guid;
}


sub is_only_keyword {
  my($self) = shift;
  my($line) = shift;

  ## Does the line contain only a keyword?
  if ($line =~ /^<%(.*)%>$/) {
    my($part) = $1;
    if ($part !~ /%>/) {
      $part =~ s/\(.*//;
      return (defined $keywords{$part} ? 1 : 0);
    }
  }
  return 0;
}


sub parse_line {
  my($self)        = shift;
  my($ih)          = shift;
  my($line)        = shift;
  my($status)      = 1;
  my($errorString) = '';
  my($length)      = length($line);
  my($name)        = 0;
  my($crlf)        = $self->{'crlf'};
  my($clen)        = length($crlf);
  my($startempty)  = ($line eq '' ? 1 : 0);
  my($append_name) = 0;

  ## If processing a foreach or the line only
  ## contains a keyword, then we do
  ## not need to add a newline to the end.
  if ($self->{'foreach'}->{'processing'} == 0 &&
      !$self->is_only_keyword($line)) {
    $line   .= $crlf;
    $length += $clen;
  }

  if ($self->{'foreach'}->{'count'} < 0) {
    $self->{'built'} = '';
  }

  for(my $i = 0; $i < $length; ++$i) {
    my($part) = substr($line, $i, 2);
    if ($part eq '<%') {
      ++$i;
      $name = 1;
    }
    elsif ($part eq '%>') {
      ++$i;
      $name = 0;
      if ($append_name) {
        $append_name = 0;
        if (!$self->{'if_skip'}) {
          $self->append_current($part);
        }
      }
    }
    elsif ($name) {
      my($substr)  = substr($line, $i);
      my($efcheck) = ($substr =~ /^endfor\%\>/);
      my($focheck) = ($substr =~ /^foreach\(/);

      if ($focheck && $self->{'foreach'}->{'count'} >= 0) {
        ++$self->{'foreach'}->{'nested'};
      }

      if ($self->{'foreach'}->{'count'} < 0 ||
          $self->{'foreach'}->{'processing'} > $self->{'foreach'}->{'nested'} ||
          (($efcheck || $focheck) &&
           $self->{'foreach'}->{'nested'} == $self->{'foreach'}->{'processing'})) {
        my($nlen) = 0;
        ($status,
         $errorString,
         $nlen) = $self->process_name($substr);

        if ($status && $nlen == 0) {
          $errorString = "ERROR: Could not parse this line at column $i";
          $status = 0;
        }
        if (!$status) {
          last;
        }

        $i += ($nlen - 1);
      }
      else  {
        $name = 0;
        if (!$self->{'if_skip'}) {
          $self->append_current('<%' . substr($line, $i, 1));
          $append_name = 1;
        }
      }

      if ($efcheck && $self->{'foreach'}->{'nested'} > 0) {
        --$self->{'foreach'}->{'nested'};
      }
    }
    else {
      if (!$self->{'if_skip'}) {
        $self->append_current(substr($line, $i, 1));
      }
    }
  }

  if ($self->{'foreach'}->{'count'} < 0) {
    ## If the line started out empty and we're not
    ## skipping from the start or the built up line is not empty
    if ($startempty ||
        ($self->{'built'} ne $crlf && $self->{'built'} ne '')) {
      push(@{$self->{'lines'}}, $self->{'built'});
    }
  }

  return $status, $errorString;
}


sub parse_file {
  my($self)  = shift;
  my($input) = shift;

  $self->collect_data();
  my($status, $errorString) = $self->read_file($input);

  if ($status) {
    my($sstack) = $self->{'sstack'};
    if (defined $$sstack[0]) {
      my($lstack) = $self->{'lstack'};
      $status = 0;
      $errorString = "ERROR: missing an $$sstack[0] starting at $$lstack[0]";
    }
  }

  if (!$status) {
    my($linenumber) = $self->line_number();
    $errorString = "$input: line $linenumber:\n$errorString\n";
  }

  return $status, $errorString;
}


sub get_lines {
  my($self) = shift;
  return $self->{'lines'};
}


1;
