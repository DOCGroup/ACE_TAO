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
use WinVersionTranslator;

use vars qw(@ISA);
@ISA = qw(Parser);

# ************************************************************
# Data Section
# ************************************************************

# Valid keywords for use in template files.  Each has a handle_
# method available, but some have other methods too.
# Bit  Meaning
# 0 means there is a get_ method available (used by if and nested functions)
# 1 means there is a perform_ method available (used by foreach and nested)
# 2 means there is a doif_ method available (used by if)
# 3 means that parameters to perform_ should not be evaluated
#
# Perl Function		Parameter Type		Return Type
# get_			string			string or array
# perform_		array reference		array
# doif_			array reference		boolean
#
my %keywords = ('if'              => 0,
                'else'            => 0,
                'endif'           => 0,
                'noextension'     => 3,
                'dirname'         => 7,
                'basename'        => 0,
                'basenoextension' => 0,
                'foreach'         => 0,
                'forfirst'        => 0,
                'fornotfirst'     => 0,
                'fornotlast'      => 0,
                'forlast'         => 0,
                'endfor'          => 0,
                'eval'            => 0,
                'comment'         => 0,
                'marker'          => 0,
                'uc'              => 3,
                'lc'              => 3,
                'ucw'             => 0,
                'normalize'       => 3,
                'flag_overrides'  => 1,
                'reverse'         => 3,
                'sort'            => 3,
                'uniq'            => 3,
                'multiple'        => 5,
                'starts_with'     => 5,
                'ends_with'       => 5,
                'contains'        => 5,
                'remove_from'     => 0xf,
                'compares'        => 5,
                'duplicate_index' => 5,
                'transdir'        => 5,
                'has_extension'   => 5,
                'keyname_used'    => 0,
                'scope'           => 0,
                'full_path'       => 3,
                'extensions'      => 0xa,
               );

my %target_type_vars = ('type_is_static'   => 1,
                        'need_staticflags' => 1,
                        'type_is_dynamic'  => 1,
                        'type_is_binary'   => 1,
                       );

my %arrow_op_ref = ('custom_type'     => 'custom types',
                    'grouped_.*_file' => 'grouped files',
                    'feature'         => 'features',
                   );

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class, $prjc) = @_;
  my $self = $class->SUPER::new();

  $self->{'prjc'}                 = $prjc;
  $self->{'ti'}                   = $prjc->get_template_input();
  $self->{'cslashes'}             = $prjc->convert_slashes();
  $self->{'crlf'}                 = $prjc->crlf();
  $self->{'cmds'}                 = $prjc->get_command_subs();
  $self->{'vnames'}               = $prjc->get_valid_names();
  $self->{'values'}               = {};
  $self->{'defaults'}             = {};
  $self->{'lines'}                = [];
  $self->{'built'}                = '';
  $self->{'sstack'}               = [];
  $self->{'lstack'}               = [];
  $self->{'if_skip'}              = 0;
  $self->{'eval'}                 = 0;
  $self->{'eval_str'}             = '';
  $self->{'dupfiles'}             = {};
  $self->{'override_target_type'} = undef;
  $self->{'keyname_used'}         = {};
  $self->{'scopes'}               = {};

  $self->{'foreach'}  = {};
  $self->{'foreach'}->{'count'}      = -1;
  $self->{'foreach'}->{'nested'}     = 0;
  $self->{'foreach'}->{'name'}       = [];
  $self->{'foreach'}->{'vars'}       = [];
  $self->{'foreach'}->{'text'}       = [];
  $self->{'foreach'}->{'scope'}      = [];
  $self->{'foreach'}->{'scope_name'} = [];
  $self->{'foreach'}->{'temp_scope'} = [];
  $self->{'foreach'}->{'processing'} = 0;

  return $self;
}


sub tp_basename {
  my($self, $file) = @_;

  if ($self->{'cslashes'}) {
    $file =~ s/.*[\/\\]//;
  }
  else {
    $file =~ s/.*\///;
  }
  return $file;
}


sub validated_dirname {
  my($self, $file) = @_;
  my $index = rindex($file, ($self->{'cslashes'} ? '\\' : '/'));

  if ($index >= 0) {
    return $self->{'prjc'}->validated_directory(substr($file, 0, $index));
  }
  else {
    return '.';
  }
}


sub tp_dirname {
  my($self, $file) = @_;
  my $index = rindex($file, ($self->{'cslashes'} ? '\\' : '/'));

  if ($index >= 0) {
    return substr($file, 0, $index);
  }
  else {
    return '.';
  }
}


sub strip_line {
  #my $self = shift;
  #my $line = shift;

  ## Override strip_line() from Parser.
  ## We need to preserve leading space and
  ## there is no comment string in templates.
  ++$_[0]->{'line_number'};
  $_[1] =~ s/\s+$//;

  return $_[1];
}


## Append the current value to the line that is being
## built.  This line may be a foreach line or a general
## line without a foreach.
sub append_current {
  my $value = $_[1];
  my $scope = $_[0]->{'scopes'};
  while(defined $$scope{'scope'}) {
    $scope = $$scope{'scope'};
    if (defined $$scope{'escape'}) {
      if ($$scope{'escape'}->[1] < 0 && $_[0]->{'foreach'}->{'count'} >= 0) {
        ## This scope was created outside of a foreach.  If we are
        ## processing a foreach, we need to skip this at this point as it
        ## will be handled once the foreach has been completed and is
        ## appended to the main project body.
        last;
      }
      else {
        my $key = $$scope{'escape'}->[0];
        if ($key eq '\\') {
          $value =~ s/\\/\\\\/g;
        }
        else {
          $value =~ s/($key)/\\$1/g;
        }
      }
    }
    else {
      foreach my $key (keys %$scope) {
        $_[0]->warning("Unrecognized scope function: $key.");
      }
    }
  }


  if ($_[0]->{'foreach'}->{'count'} >= 0) {
    $_[0]->{'foreach'}->{'text'}->[$_[0]->{'foreach'}->{'count'}] .= $value;
  }
  elsif ($_[0]->{'eval'}) {
    $_[0]->{'eval_str'} .= $value;
  }
  else {
    $_[0]->{'built'} .= $value;
  }
}


sub split_parameters {
  my($self, $str) = @_;
  my @params;

  while($str =~ /^(\w+\([^\)]+\))\s*,\s*(.*)/) {
    push(@params, $1);
    $str = $2;
  }
  while($str =~ /^([^,]+)\s*,\s*(.*)/) {
    push(@params, $1);
    $str = $2;
  }

  ## Return the parameters (which includes whatever is left in the
  ## string).  Just return it instead of pushing it onto @params.
  return @params, $str;
}


sub set_current_values {
  my($self, $name) = @_;
  my $set = 0;

  ## If any value within a foreach matches the name
  ## of a hash table within the template input we will
  ## set the values of that hash table in the current scope
  if (defined $self->{'ti'}) {
    my $counter = $self->{'foreach'}->{'count'};
    if ($counter >= 0) {
      ## Variable names are case-insensitive in MPC, however this can
      ## cause problems when dealing with template variable values that
      ## happen to match HASH names only by case-insensitivity.  So, we
      ## now make HASH names match with case-sensitivity.
      my $value = $self->{'ti'}->get_value($name);
      if (defined $value && UNIVERSAL::isa($value, 'HASH') &&
          $self->{'ti'}->get_realname($name) eq $name) {
        $self->{'foreach'}->{'scope_name'}->[$counter] = $name;
        my %copy;
        foreach my $key (keys %$value) {
          $copy{$key} = $self->{'prjc'}->adjust_value(
                    [$name . '::' . $key, $name], $$value{$key}, $self);
        }
        $self->{'foreach'}->{'temp_scope'}->[$counter] = \%copy;
        $set = 1;
      }
      else {
        ## Since we're not creating a temporary scope for this level, we
        ## need to empty out the scope that may have been held here from
        ## a previous foreach.
        $self->{'foreach'}->{'temp_scope'}->[$counter] = {};
      }
    }
  }
  return $set;
}


sub get_value {
  my($self, $name) = @_;
  my $value;
  my $counter = $self->{'foreach'}->{'count'};
  my $fromprj;
  my $scope;
  my $sname;
  my $adjust = 1;

  ## $name should always be all lower-case
  $name = lc($name);

  ## First, check the temporary scope (set inside a foreach)
  if ($counter >= 0) {
    ## Find the outer most scope for our variable name
    for(my $index = $counter; $index >= 0; --$index) {
      if (defined $self->{'foreach'}->{'scope_name'}->[$index]) {
        $scope = $self->{'foreach'}->{'scope_name'}->[$index];
        $sname = $scope . '::' . $name;
        last;
      }
    }
    while(!defined $value && $counter >= 0) {
      $value = $self->{'foreach'}->{'temp_scope'}->[$counter]->{$name};
      --$counter;
    }
    $counter = $self->{'foreach'}->{'count'};

    if ($self->{'override_target_type'} &&
        defined $value && defined $target_type_vars{$name}) {
      $value = $self->{'values'}->{$name};
    }
  }

  if (!defined $value) {
    if ($name =~ /^flag_overrides\((.*)\)$/) {
      $value = $self->get_flag_overrides($1);
    }

    if (!defined $value) {
      ## Next, check for a template value
      if (defined $self->{'ti'}) {
        $value = $self->{'ti'}->get_value($name);
      }

      if (!defined $value) {
        ## Calling adjust_value here allows us to pick up template
        ## overrides before getting values elsewhere.
        my $uvalue = $self->{'prjc'}->adjust_value([$sname, $name],
                                                   [], $self);
        if (defined $$uvalue[0]) {
          $value = $uvalue;
          $adjust = 0;
          $fromprj = 1;
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
            $fromprj = 1;
            $value = $self->{'prjc'}->get_assignment($name);

            ## Then get it from our known values
            if (!defined $value) {
              $value = $self->{'values'}->{$name};
              if (!defined $value) {
                ## Call back onto the project creator to allow
                ## it to fill in the value before defaulting to undef.
                $value = $self->{'prjc'}->fill_value($name);
                if (!defined $value && $name =~ /^(.*)\->(\w+)/) {
                  my $pre  = $1;
                  my $post = $2;
                  my $base = $self->get_value($pre);

                  if (defined $base) {
                    $value = $self->{'prjc'}->get_special_value(
                               $pre, $post, $base,
                               ($self->{'prjc'}->requires_parameters($post) ?
                                   $self->prepare_parameters($pre) : undef));
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  ## Adjust the value even if we haven't obtained one from an outside
  ## source.
  if ($adjust && defined $value) {
    $value = $self->{'prjc'}->adjust_value([$sname, $name], $value, $self);
  }

  ## If the value did not come from the project creator, we
  ## check the variable name.  If it is a project keyword we then
  ## check to see if we need to add the project value to the template
  ## variable value.  If so, we make a copy of the value array and
  ## push the project value onto that (to avoid modifying the original).
  if (!$fromprj && defined $self->{'vnames'}->{$name} &&
      $self->{'prjc'}->add_to_template_input_value($name)) {
    my $pjval = $self->{'prjc'}->get_assignment($name);
    if (defined $pjval) {
      my @copy = @$value;
      if (!UNIVERSAL::isa($pjval, 'ARRAY')) {
        $pjval = $self->create_array($pjval);
      }
      push(@copy, @$pjval);
      $value = \@copy;
    }
  }

  return $self->{'prjc'}->relative($value, undef, $scope);
}


sub get_value_with_default {
  my $self  = shift;
  my $name = lc(shift);
  my $value = $self->get_value($name);

  if (!defined $value) {
    $value = $self->{'defaults'}->{$name};
    if (defined $value) {
      my $counter = $self->{'foreach'}->{'count'};
      my $sname;

      if ($counter >= 0) {
        ## Find the outer most scope for our variable name
        for(my $index = $counter; $index >= 0; --$index) {
          if (defined $self->{'foreach'}->{'scope_name'}->[$index]) {
            $sname = $self->{'foreach'}->{'scope_name'}->[$index] .
                     '::' . $name;
            last;
          }
        }
      }
      $value = $self->{'prjc'}->relative(
                 $self->{'prjc'}->adjust_value(
                   [$sname, $name], $value, $self));

      ## If the user set the variable to empty, we will go ahead and use
      ## the default value (since we know we have one at this point).
      $value = $self->{'defaults'}->{$name} if (!defined $value);
    }
    else {
      #$self->warning("$name defaulting to empty string.");
      $value = '';
    }
  }

  return (UNIVERSAL::isa($value, 'ARRAY') ? "@$value" : $value);
}


sub process_foreach {
  my $self   = shift;
  my $index = $self->{'foreach'}->{'count'};
  my $text = $self->{'foreach'}->{'text'}->[$index];
  my @values;
  my $name = $self->{'foreach'}->{'name'}->[$index];
  my @cmds;
  my $val = $self->{'foreach'}->{'vars'}->[$index];
  my $check_for_mixed;

  if ($val =~ /^((\w+),\s*)?flag_overrides\((.*)\)$/) {
    ## If the user did not provide a name we have to pick one otherwise
    ## there would be no way to access the foreach values.
    $name = (defined $2 ? $2 : '__unnamed__');

    ## Now check to see if there were overrides for this value.  If there
    ## were, convert them into an array (if necessary) and continue
    ## processing.
    $val = $self->get_flag_overrides($3);
    if (defined $val) {
      $val = $self->create_array($val) if (!UNIVERSAL::isa($val, 'ARRAY'));
      @values = @$val;
    }
  }
  else {
    ## Pull out modifying commands first
    while ($val =~ /(\w+)\((.+)\)/) {
      my $cmd = $1;
      $val = $2;
      if (($keywords{$cmd} & 0x02) != 0) {
        push(@cmds, 'perform_' . $cmd);
        if (($keywords{$cmd} & 0x08) != 0) {
          my @params = $self->split_parameters($val);
          $val = \@params;
          last;
        }
      }
      else {
        $self->warning("Unable to use $cmd in foreach (no perform_ method).");
      }
    }

    ## Get the values for all of the variable names
    ## contained within the foreach
    if (UNIVERSAL::isa($val, 'ARRAY')) {
      @values = @$val;
    }
    else {
      my $names = $self->create_array($val);
      foreach my $n (@$names) {
        my $vals = $self->get_value($n);
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
        ## We only want to check for the mixing of scalar and hash
        ## variables if the variable name is not a keyword (or the
        ## special 'features' template variable).
        if (!$check_for_mixed &&
            !$self->{'prjc'}->is_keyword($n) && $n ne 'features') {
          $check_for_mixed = 1;
        }
      }
    }
  }

  ## Perform the commands on the built up @values
  foreach my $cmd (reverse @cmds) {
    @values = $self->$cmd(\@values);
  }

  ## Reset the text (it will be regenerated by calling parse_line
  $self->{'foreach'}->{'text'}->[$index] = '';

  if (defined $values[0]) {
    my $scope = $self->{'foreach'}->{'scope'}->[$index];
    my $base  = $self->{'foreach'}->{'base'}->[$index];

    $$scope{'forlast'}     = '';
    $$scope{'fornotlast'}  = 1;
    $$scope{'forfirst'}    = 1;
    $$scope{'fornotfirst'} = '';

    ## If the foreach values are mixed (HASH and SCALAR), then
    ## remove the SCALAR values.
    if ($check_for_mixed) {
      my %mixed;
      my $mixed = 0;
      foreach my $mval (@values) {
        $mixed{$mval} = $self->set_current_values($mval);
        $mixed |= $mixed{$mval};
      }
      if ($mixed) {
        my @nvalues;
        foreach my $key (sort keys %mixed) {
          push(@nvalues, $key) if ($mixed{$key});
        }

        ## Set the new values only if they are different
        ## from the original (except for order).
        my @sorted = sort(@values);
        @values = @nvalues if (@sorted != @nvalues);
      }
    }

    for(my $i = 0; $i <= $#values; ++$i) {
      my $value = $values[$i];

      ## Set the corresponding values in the temporary scope
      $self->set_current_values($value);

      ## Set the special values that only exist
      ## within a foreach
      if ($i != 0) {
        $$scope{'forfirst'}    = '';
        $$scope{'fornotfirst'} = 1;
      }
      if ($i == $#values) {
        $$scope{'forlast'}    = 1;
        $$scope{'fornotlast'} = '';
      }
      $$scope{'forcount'} = $i + $base;

      ## We don't use adjust_value here because these names
      ## are generated from a foreach and should not be adjusted.
      $$scope{$name} = $value;

      ## A tiny hack for VC7
      if ($name eq 'configuration' &&
          $self->get_value_with_default('platform') ne '') {
        $self->{'prjc'}->update_project_info($self, 1,
                                             ['configuration', 'platform'],
                                             '|');
      }

      ## Now parse the line of text, each time
      ## with different values
      ++$self->{'foreach'}->{'processing'};
      my($status, $error) = $self->parse_line(undef, $text);
      --$self->{'foreach'}->{'processing'};
      return $error if (defined $error);
    }
  }

  return undef;
}


sub generic_handle {
  my($self, $func, $str) = @_;

  if (defined $str) {
    my $val = $self->$func([$str]);

    if (defined $val) {
      $self->append_current($val);
    }
    else {
      $self->append_current(0);
    }
  }
}


sub handle_endif {
  my($self, $name) = @_;
  my $end = pop(@{$self->{'sstack'}});
  pop(@{$self->{'lstack'}});

  if (!defined $end) {
    return "Unmatched $name";
  }
  else {
    my $in = index($end, $name);
    if ($in == 0) {
      $self->{'if_skip'} = 0;
    }
    elsif ($in == -1) {
      return "Unmatched $name";
    }
  }

  return undef;
}


sub handle_endfor {
  my($self, $name) = @_;
  my $end = pop(@{$self->{'sstack'}});
  pop(@{$self->{'lstack'}});

  if (!defined $end) {
    return "Unmatched $name";
  }
  else {
    my $in = index($end, $name);
    if ($in == 0) {
      my $index = $self->{'foreach'}->{'count'};
      my $error = $self->process_foreach();
      if (!defined $error) {
        --$self->{'foreach'}->{'count'};
        $self->append_current($self->{'foreach'}->{'text'}->[$index]);
      }
      return $error;
    }
    elsif ($in == -1) {
      return "Unmatched $name";
    }
  }

  return undef;
}


sub get_flag_overrides {
  my($self, $name) = @_;
  my $type;

  ## Split the name and type parameters
  ($name, $type) = split(/,\s*/, $name);

  my $file = $self->get_value($name);
  if (defined $file) {
    ## Save the name prefix (if there is one) for
    ## command parameter conversion at the end
    my $pre;
    if ($name =~ /^(\w+)->/) {
      $pre = $1;

      ## Replace the custom_type key with the actual custom type
      if ($pre eq 'custom_type') {
        my $ct = $self->get_value($pre);
        $name = $ct if (defined $ct);
      }
      elsif ($pre =~ /^grouped_(.*_file)$/) {
        $name = $1;
      }
    }

    my $fo  = $self->{'prjc'}->{'flag_overrides'};
    my $key = (defined $$fo{$name . 's'} ? $name . 's' :
                       (defined $$fo{$name} ? $name : undef));

    if (defined $key) {
      ## Convert the file name into a unix style file name
      my $ustyle = $file;
      $ustyle =~ s/\\/\//g if ($self->{'cslashes'});

      ## Save the directory portion for checking in the foreach
      my $dir = $self->mpc_dirname($ustyle);

      my $of = (defined $$fo{$key}->{$ustyle} ? $ustyle :
                   (defined $$fo{$key}->{$dir} ? $dir : undef));
      if (defined $of) {
        my $prjc = $self->{'prjc'};
        foreach my $aname (@{$prjc->{'matching_assignments'}->{$key}}) {
          if ($aname eq $type && defined $$fo{$key}->{$of}->{$aname}) {
            my $value = $$fo{$key}->{$of}->{$aname};

            ## If the name that we're overriding has a value and
            ## requires parameters, then we will convert all of the
            ## pseudo variables and provide parameters.
            if (defined $pre && $prjc->requires_parameters($type)) {
              $value = $prjc->convert_command_parameters(
                                      $key, $value,
                                      $self->prepare_parameters($pre));
            }

            return $prjc->relative($value);
          }
        }
      }
    }
  }

  return undef;
}


sub get_multiple {
  my($self, $name) = @_;
  return $self->doif_multiple(
                  $self->create_array($self->get_value_with_default($name)));
}


sub doif_multiple {
  my($self, $value) = @_;
  return defined $value ? (scalar(@$value) > 1) : undef;
}


sub handle_multiple {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  if (defined $val) {
    my $array = $self->create_array($val);
    $self->append_current(scalar(@$array));
  }
  else {
    $self->append_current(0);
  }
}


sub get_starts_with {
  my($self, $str) = @_;
  return $self->doif_starts_with([$str]);
}


sub doif_starts_with {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      return ($self->get_value_with_default($name) =~ /^$pattern/);
    }
  }
  return undef;
}


sub handle_starts_with {
  my($self, $str) = @_;
  $self->generic_handle('doif_starts_with', $str);
}


sub get_ends_with {
  my($self, $str) = @_;
  return $self->doif_ends_with([$str]);
}


sub doif_ends_with {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      return ($self->get_value_with_default($name) =~ /$pattern$/);
    }
  }
  return undef;
}


sub handle_ends_with {
  my($self, $str) = @_;
  $self->generic_handle('doif_ends_with', $str);
}


sub handle_keyname_used {
  my($self, $str) = @_;

  if (defined $str) {
    my($name, $key) = $self->split_parameters($str);
    my $file = $self->get_value_with_default($name);
    if (defined $self->{'keyname_used'}->{$file}->{$key}) {
      $self->append_current($self->{'keyname_used'}->{$file}->{$key}++);
    }
    else {
      $self->{'keyname_used'}->{$file}->{$key} = 1;
    }
  }
}


sub handle_scope {
  my($self, $str) = @_;

  if (defined $str) {
    my($state, $func, $param) = $self->split_parameters($str);
    if (defined $state) {
      my $pscope;
      my $scope = $self->{'scopes'};

      while(defined $$scope{'scope'}) {
        $pscope = $scope;
        $scope = $$scope{'scope'};
      }
      if ($state eq 'enter') {
        if (defined $func) {
          $param = '' if (!defined $param);
          $$scope{'scope'}->{$func} = [$self->process_special($param),
                                       $_[0]->{'foreach'}->{'count'}];
        }
        else {
          $self->warning("The enter scope function requires a parameter.");
        }
      }
      elsif ($state eq 'leave') {
        if (defined $pscope) {
          delete $$pscope{'scope'};
        }
        else {
          $self->warning("leave scope function encountered without an enter.");
        }
      }
      else {
        $self->warning("Unrecognized scope function parameter: $state.");
      }
    }
    else {
      $self->warning("The scope function requires 1 to 3 parameters.");
    }
  }
}

sub get_has_extension {
  my($self, $str) = @_;
  return $self->doif_has_extension([$str]);
}


sub doif_has_extension {
  my($self, $val) = @_;

  if (defined $val) {
    return ($self->tp_basename(
                $self->get_value_with_default("@$val")) =~ /\.[^\.]*$/);
  }
  return undef;
}


sub handle_has_extension {
  my($self, $str) = @_;
  $self->generic_handle('doif_has_extension', $str);
}


sub get_contains {
  my($self, $str) = @_;
  return $self->doif_contains([$str]);
}


sub doif_contains {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      return ($self->get_value_with_default($name) =~ /$pattern/);
    }
  }
  return undef;
}


sub handle_contains {
  my($self, $str) = @_;
  $self->generic_handle('doif_contains', $str);
}


sub get_remove_from {
  my($self, $str) = @_;
  return $self->doif_remove_from($str);
}


sub doif_remove_from {
  my($self, $str) = @_;
  my @params = $self->split_parameters($str);
  my @removed = $self->perform_remove_from(\@params);
  return (defined $removed[0] ? 1 : undef);
}


sub perform_remove_from {
  my($self, $val) = @_;
  my($source, $pattern, $target, $tremove) = @$val;

  ## $source should be a component name (e.g., source_files,
  ## header_files, etc.)  $target is a variable name
  ## $pattern and $tremove are optional; $pattern is a partial regular
  ## expression to match the end of the files found from $source.  The
  ## beginning of the regular expression is made from $target by removing
  ## $tremove from the end of it.
  if (defined $source && defined $target &&
      defined $self->{'values'}->{$source}) {
    my $tval = $self->get_value_with_default($target);
    if (defined $tval) {
      $tval =~ s/$tremove$// if (defined $tremove);
      $tval = $self->escape_regex_special($tval);
      my @removed;
      my $max = scalar(@{$self->{'values'}->{$source}});
      for(my $i = 0; $i < $max;) {
        if ($self->{'values'}->{$source}->[$i] =~ /^$tval$pattern$/) {
          push(@removed, splice(@{$self->{'values'}->{$source}}, $i, 1));
          $max--;
        }
        else {
          $i++;
        }
      }
      return @removed;
    }
  }

  return ();
}


sub handle_remove_from {
  my($self, $str) = @_;

  if (defined $str) {
    my @params = $self->split_parameters($str);
    my $val = $self->perform_remove_from(\@params);
    $self->append_current("@$val") if (defined $val);
  }
}


sub get_compares {
  my($self, $str) = @_;
  return $self->doif_compares([$str]);
}


sub doif_compares {
  my($self, $val) = @_;

  if (defined $val) {
    my($name, $pattern) = $self->split_parameters("@$val");
    if (defined $name && defined $pattern) {
      return ($self->get_value_with_default($name) eq $pattern);
    }
  }
  return undef;
}


sub handle_compares {
  my($self, $str) = @_;
  $self->generic_handle('doif_compares', $str);
}


sub get_reverse {
  my($self, $name) = @_;
  my $value = $self->get_value_with_default($name);

  if (defined $value) {
    my @array = $self->perform_reverse($self->create_array($value));
    return \@array;
  }

  return undef;
}


sub perform_reverse {
  my($self, $value) = @_;
  return reverse(@$value);
}


sub handle_reverse {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  if (defined $val) {
    my @array = $self->perform_reverse($self->create_array($val));
    $self->append_current("@array");
  }
}


sub get_sort {
  my($self, $name) = @_;
  my $value = $self->get_value_with_default($name);

  if (defined $value) {
    my @array = $self->perform_sort($self->create_array($value));
    return \@array;
  }

  return undef;
}


sub perform_sort {
  my($self, $value) = @_;
  return sort(@$value);
}


sub handle_sort {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  if (defined $val) {
    my @array = $self->perform_sort($self->create_array($val));
    $self->append_current("@array");
  }
}


sub get_uniq {
  my($self, $name) = @_;
  my $value = $self->get_value_with_default($name);

  if (defined $value) {
    my @array = $self->perform_uniq($self->create_array($value));
    return \@array;
  }

  return undef;
}


sub perform_uniq {
  my($self, $value) = @_;
  my %value;
  @value{@$value} = ();
  return sort(keys %value);
}


sub handle_uniq {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  if (defined $val) {
    my @array = $self->perform_uniq($self->create_array($val));
    $self->append_current("@array");
  }
}


sub process_compound_if {
  my($self, $str) = @_;

  if (index($str, '||') >= 0) {
    my $ret = 0;
    foreach my $v (split(/\s*\|\|\s*/, $str)) {
      $ret |= $self->process_compound_if($v);
      return 1 if ($ret != 0);
    }
    return 0;
  }
  elsif (index($str, '&&') >= 0) {
    my $ret = 1;
    foreach my $v (split(/\s*\&\&\s*/, $str)) {
      $ret &&= $self->process_compound_if($v);
      return 0 if ($ret == 0);
    }
    return 1;
  }
  else {
    ## See if we need to reverse the return value
    my $not = 0;
    if ($str =~ /^!+(.*)/) {
      $not = 1;
      $str = $1;
    }

    ## Get the value based on the string
    my @cmds;
    my $val;
    while ($str =~ /(\w+)\((.+)\)(.*)/) {
      if ($3 eq '') {
        push(@cmds, $1);
        $str = $2;
      }
      else {
        ## If there is something trailing the closing parenthesis then
        ## the whole thing is considered a parameter to the first
        ## function.
        last;
      }
    }

    if (defined $cmds[0]) {
      ## Start out calling get_xxx on the string
      my $type = 0x01;
      my $prefix = 'get_';

      $val = $str;
      foreach my $cmd (reverse @cmds) {
        if (defined $keywords{$cmd} && ($keywords{$cmd} & $type) != 0) {
          my $func = "$prefix$cmd";
          $val = $self->$func($val);

          ## Now that we have a value, we need to switch over
          ## to calling doif_xxx
          $type = 0x04;
          $prefix = 'doif_';
        }
        else {
          $self->warning("Unable to use $cmd in if (no $prefix method).");
        }
      }
    }
    else {
      $val = $self->get_value($str);
    }

    ## See if any portion of the value is defined and not empty
    my $ret = 0;
    if (defined $val) {
      if (UNIVERSAL::isa($val, 'ARRAY')) {
        foreach my $v (@$val) {
          if ($v ne '') {
            $ret = 1;
            last;
          }
        }
      }
      elsif ($val ne '') {
        $ret = 1;
      }
    }
    return ($not ? !$ret : $ret);
  }
}


sub handle_if {
  my($self, $val) = @_;
  my $name = 'endif';

  push(@{$self->{'lstack'}},
       "<%if($val)%> (" . $self->get_line_number() . '?)');
  if ($self->{'if_skip'}) {
    push(@{$self->{'sstack'}}, "*$name");
  }
  else {
    ## Determine if we are skipping the portion of this if statement
    ## $val will always be defined since we won't get into this method
    ## without properly parsing the if statement.
    $self->{'if_skip'} = !$self->process_compound_if($val);
    push(@{$self->{'sstack'}}, $name);
  }
}


sub handle_else {
  my $self  = shift;
  my @scopy = @{$self->{'sstack'}};
  my $index = index($scopy[$#scopy], 'endif');
  if ($index >= 0) {
    if ($index == 0) {
      $self->{'if_skip'} ^= 1;
    }
    $self->{'sstack'}->[$#scopy] .= ':';
  }

  return 'Unmatched else' if (($self->{'sstack'}->[$#scopy] =~ tr/:/:/) > 1);
  return undef;
}


sub handle_foreach {
  my $self        = shift;
  my $val = lc(shift);
  my $name = 'endfor';
  my $errorString;

  push(@{$self->{'lstack'}}, $self->get_line_number());
  if (!$self->{'if_skip'}) {
    my $base = 1;
    my $vname;
    if ($val =~ /flag_overrides\([^\)]+\)/) {
    }
    elsif ($val =~ /([^,]*),(.*)/) {
      $vname = $1;
      $val   = $2;
      $vname =~ s/^\s+//;
      $vname =~ s/\s+$//;
      $val   =~ s/^\s+//;
      $val   =~ s/\s+$//;

      if ($vname eq '') {
        $errorString = 'The foreach variable name is not valid';
      }

      if ($val =~ /([^,]*),(.*)/) {
        $base = $1;
        $val  = $2;
        $base =~ s/^\s+//;
        $base =~ s/\s+$//;
        $val  =~ s/^\s+//;
        $val  =~ s/\s+$//;

        if ($base !~ /^\d+$/) {
          $errorString = 'The forcount specified is not a valid number';
        }
      }
      elsif ($vname =~ /^\d+$/) {
        $base  = $vname;
        $vname = undef;
      }

      ## Due to the way flag_overrides works, we can't allow
      ## the user to name the foreach variable when dealing
      ## with variables that can be used with the -> operator
      if (defined $vname) {
        foreach my $ref (keys %arrow_op_ref) {
          my $name_re  = $ref . 's';
          if ($val =~ /^$ref\->/ || $val =~ /^$name_re$/) {
            $errorString = 'The foreach variable can not be ' .
                           'named when dealing with ' .
                           $arrow_op_ref{$ref};
          }
        }
      }
    }

    push(@{$self->{'sstack'}}, $name);
    my $index = ++$self->{'foreach'}->{'count'};

    $self->{'foreach'}->{'base'}->[$index]  = $base;
    $self->{'foreach'}->{'name'}->[$index]  = $vname;
    $self->{'foreach'}->{'vars'}->[$index]  = $val;
    $self->{'foreach'}->{'text'}->[$index]  = '';
    $self->{'foreach'}->{'scope'}->[$index] = {};
    $self->{'foreach'}->{'scope_name'}->[$index] = undef;
  }
  else {
    push(@{$self->{'sstack'}}, "*$name");
  }

  return $errorString;
}


sub handle_special {
  my($self, $name, $val) = @_;

  ## If $name (fornotlast, forfirst, etc.) is set to 1
  ## Then we append the $val onto the current string that's
  ## being built.
  $self->append_current($val) if ($self->get_value($name));
}


sub get_uc {
  my($self, $name) = @_;
  return uc($self->get_value_with_default($name));
}


sub handle_uc {
  my($self, $name) = @_;
  $self->append_current($self->get_uc($name));
}


sub perform_uc {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, uc($val));
  }
  return @val;
}


sub get_lc {
  my($self, $name) = @_;
  return lc($self->get_value_with_default($name));
}


sub handle_lc {
  my($self, $name) = @_;
  $self->append_current($self->get_lc($name));
}


sub perform_lc {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, lc($val));
  }
  return @val;
}


sub handle_ucw {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  substr($val, 0, 1) = uc(substr($val, 0, 1));
  while($val =~ /[_\s]([a-z])/) {
    my $uc = uc($1);
    $val =~ s/[_\s][a-z]/ $uc/;
  }
  $self->append_current($val);
}


sub actual_normalize {
  $_[1] =~ tr/ \t\/\\\-$()./_/;
  return $_[1];
}

sub perform_normalize {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, $self->actual_normalize($val));
  }
  return @val;
}


sub get_normalize {
  my($self, $name) = @_;
  return $self->actual_normalize($self->get_value_with_default($name));
}


sub handle_normalize {
  my($self, $name) = @_;
  $self->append_current($self->get_normalize($name));
}


sub actual_noextension {
  $_[1] =~ s/\.[^\.]*$//;
  return $_[1];
}


sub perform_noextension {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, $self->actual_noextension($val));
  }
  return @val;
}


sub get_noextension {
  my($self, $name) = @_;
  return $self->actual_noextension($self->get_value_with_default($name));
}

sub handle_noextension {
  my($self, $name) = @_;
  $self->append_current($self->get_noextension($name));
}


sub perform_full_path {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, $self->actual_full_path($val));
  }
  return @val;
}


sub get_full_path {
  my($self, $name) = @_;
  return $self->actual_full_path($self->get_value_with_default($name));
}


sub actual_full_path {
  my($self, $value) = @_;

  ## Expand all defined env vars
  $value =~ s/\$\((\w+)\)/$ENV{$1} || '$(' . $1 . ')'/ge;

  ## If we expanded all env vars, get absolute path
  if ($value =~ /\$\(\w+\)/) {
    $self->{'error_in_handle'} = "<%full_path%> couldn't expand " .
                                 "environment variables in $value";
    return $value;
  }

  ## Always convert the slashes since they may be in the OS native
  ## format and we need them in UNIX format.
  $value =~ s/\\/\//g;
  my $dir = $self->mpc_dirname($value);
  if (-e $dir) {
    $dir = Cwd::abs_path($dir);
  }
  elsif ($self->{'prjc'}->path_is_relative($dir)) {
    ## If the directory is is not already an absolute path, then we will
    ## assume that the directory is relative to the current directory
    ## (which will be the location of the MPC file).
    $dir = $self->getcwd() . '/' . $dir;
  }

  ## Create the full path value and convert the slashes if necessary.
  $value = $dir . '/' . $self->mpc_basename($value);
  $value =~ s/\//\\/g if ($self->{'cslashes'});
  return $value;
}


sub handle_full_path {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  $self->append_current($self->actual_full_path($val));
}


sub perform_extensions {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, $self->{'prjc'}->get_component_extensions($val));
  }
  return @val;
}


sub handle_extensions {
  my($self, $name) = @_;
  my @val = $self->perform_extensions([$name]);
  $self->append_current("@val");
}


sub evaluate_nested_functions {
  my($self, $name, $val) = @_;

  ## Get the value based on the string
  my @cmds = ($name);
  while ($val =~ /(\w+)\((.+)\)/) {
    push(@cmds, $1);
    $val = $2;
  }

  ## Start out calling get_xxx on the string
  my $type = 0x01;
  my $prefix = 'get_';

  foreach my $cmd (reverse @cmds) {
    if (defined $keywords{$cmd} && ($keywords{$cmd} & $type) != 0) {
      my $func = "$prefix$cmd";
      if ($type == 0x01) {
        $val = $self->$func($val);
        $val = [ $val ] if (!UNIVERSAL::isa($val, 'ARRAY'));
      }
      else {
        my @array = $self->$func($val);
        $val = \@array;
      }

      ## Now that we have a value, we need to switch over
      ## to calling perform_xxx
      $type = 0x02;
      $prefix = 'perform_';
    }
    else {
      $self->warning("Unable to use $cmd in nested " .
                     "functions (no $prefix method).");
    }
  }
  if (defined $val && UNIVERSAL::isa($val, 'ARRAY')) {
    $self->append_current("@$val");
  }
}


sub perform_dirname {
  my($self, $value) = @_;
  my @val;
  foreach my $val (@$value) {
    push(@val, $self->validated_dirname($val));
  }
  return @val;
}


sub get_dirname {
  my($self, $name) = @_;
  return $self->doif_dirname($self->get_value_with_default($name));
}


sub doif_dirname {
  my($self, $value) = @_;

  if (defined $value) {
    $value = $self->validated_dirname($value);
    return ($value ne '.');
  }
  return undef;
}


sub handle_dirname {
  my($self, $name) = @_;

  $self->append_current(
            $self->validated_dirname($self->get_value_with_default($name)));
}


sub handle_basename {
  my($self, $name) = @_;

  $self->append_current(
            $self->tp_basename($self->get_value_with_default($name)));
}


sub handle_basenoextension {
  my($self, $name) = @_;
  my $val = $self->tp_basename($self->get_value_with_default($name));

  $val =~ s/\.[^\.]*$//;
  $self->append_current($val);
}


sub handle_flag_overrides {
  my($self, $name) = @_;
  my $value = $self->get_flag_overrides($name);
  $self->append_current(UNIVERSAL::isa($value, 'ARRAY') ?
                          "@$value" : $value) if (defined $value);
}


sub handle_marker {
  my($self, $name) = @_;
  my $val = $self->{'prjc'}->get_verbatim($name);
  $self->append_current($val) if (defined $val);
}


sub handle_eval {
  my($self, $name) = @_;
  my $val = $self->get_value_with_default($name);

  if (defined $val) {
    if (index($val, "<%eval($name)%>") >= 0) {
      $self->warning("Infinite recursion detected in '$name'.");
    }
    else {
      ## Enter the eval state
      ++$self->{'eval'};

      ## Parse the eval line
      my($status, $error) = $self->parse_line(undef, $val);
      if ($status) {
        $self->{'built'} .= $self->{'eval_str'};
      }
      else {
        $self->warning($error);
      }

      ## Leave the eval state
      --$self->{'eval'};
      $self->{'eval_str'} = '';
    }
  }
}


sub handle_pseudo {
  my($self, $name) = @_;
  $self->append_current($self->{'cmds'}->{$name});
}


sub get_duplicate_index {
  my($self, $name) = @_;
  return $self->doif_duplicate_index($self->get_value_with_default($name));
}


sub doif_duplicate_index {
  my($self, $value) = @_;

  if (defined $value) {
    my $base = lc($self->tp_basename($value));
    my $path = $self->validated_dirname($value);

    if (!defined $self->{'dupfiles'}->{$base}) {
      $self->{'dupfiles'}->{$base} = [$path];
    }
    else {
      my $index = 1;
      foreach my $file (@{$self->{'dupfiles'}->{$base}}) {
        return $index if ($file eq $path);
        ++$index;
      }

      push(@{$self->{'dupfiles'}->{$base}}, $path);
      return 1;
    }
  }

  return undef;
}


sub handle_duplicate_index {
  my($self, $name) = @_;
  my $value = $self->doif_duplicate_index(
                       $self->get_value_with_default($name));
  $self->append_current($value) if (defined $value);
}


sub get_transdir {
  my($self, $name) = @_;
  return $self->doif_transdir($self->get_value_with_default($name));
}


sub doif_transdir {
  my($self, $value) = @_;

  if ($value =~ /([\/\\])/) {
    return $self->{'prjc'}->translate_directory(
                                  $self->tp_dirname($value)) . $1;
  }

  return undef;
}


sub handle_transdir {
  my($self, $name) = @_;
  my $value = $self->doif_transdir($self->get_value_with_default($name));
  $self->append_current($value) if (defined $value);
}


sub prepare_parameters {
  my($self, $prefix) = @_;
  my $input = $self->get_value($prefix . '->input_file');
  my $output;

  if (defined $input) {
    $input =~ s/\//\\/g if ($self->{'cslashes'});
    $output = $self->get_value($prefix . '->input_file->output_files');

    if (defined $output) {
      my $size = scalar(@$output);
      for(my $i = 0; $i < $size; ++$i) {
        my $fo = $self->get_flag_overrides($prefix . '->input_file, gendir');
        if (defined $fo) {
          $$output[$i] = ($fo eq '.' ? '' : $fo . '/') .
                         $self->tp_basename($$output[$i]);
        }
        $$output[$i] =~ s/\//\\/g if ($self->{'cslashes'});
      }
    }
  }

  ## Set the parameters array with the determined input and output files
  return $input, $output;
}


sub process_name {
  my($self, $line) = @_;
  my $length = 0;
  my $errorString;

  ## Split the line into a name and value
  if ($line =~ /([^%\(]+)(\(([^%]+)\))?%>/) {
    my $name = lc($1);
    my $val  = $3;
    $length += length($name);

    if (defined $val) {
      ## Check for the parenthesis
      if (($val =~ tr/(//) != ($val =~ tr/)//)) {
        return 'Missing the closing parenthesis', $length;
      }

      ## Add the length of the value plus 2 for the surrounding ()
      $length += length($val) + 2;
    }

    if (defined $keywords{$name}) {
      if ($name eq 'if') {
        $self->handle_if($val);
      }
      elsif ($name eq 'endif') {
        $errorString = $self->handle_endif($name);
      }
      elsif ($name eq 'else') {
        $errorString = $self->handle_else();
      }
      elsif ($name eq 'endfor') {
        $errorString = $self->handle_endfor($name);
      }
      elsif ($name eq 'foreach') {
        $errorString = $self->handle_foreach($val);
      }
      elsif ($name eq 'fornotlast'  || $name eq 'forlast' ||
             $name eq 'fornotfirst' || $name eq 'forfirst') {
        if (!$self->{'if_skip'}) {
          $self->handle_special($name, $self->process_special($val));
        }
      }
      elsif ($name eq 'comment') {
        ## Ignore the contents of the comment
      }
      else {
        if (!$self->{'if_skip'}) {
          if (index($val, '(') >= 0) {
            $self->evaluate_nested_functions($name, $val);
          }
          else {
            my $func = 'handle_' . $name;
            $self->$func($val);
            if ($self->{'error_in_handle'}) {
              $errorString = $self->{'error_in_handle'};
            }
          }
        }
      }
    }
    elsif (defined $self->{'cmds'}->{$name}) {
      $self->handle_pseudo($name) if (!$self->{'if_skip'});
    }
    else {
      if (!$self->{'if_skip'}) {
        if (defined $val && !defined $self->{'defaults'}->{$name}) {
          $self->{'defaults'}->{$name} = $self->process_special($val);
        }
        $self->append_current($self->get_value_with_default($name));
      }
    }
  }
  else {
    my $error  = $line;
    my $length = length($line);
    for(my $i = 0; $i < $length; ++$i) {
      my $part = substr($line, $i, 2);
      if ($part eq '%>') {
        $error = substr($line, 0, $i + 2);
        last;
      }
    }
    $errorString = "Unable to parse line starting at '$error'";
  }

  return $errorString, $length;
}


sub collect_data {
  my $self  = shift;
  my $prjc = $self->{'prjc'};
  my $cwd = $self->getcwd();

  ## Set the current working directory
  $cwd =~ s/\//\\/g if ($self->{'cslashes'});
  $self->{'values'}->{'cwd'} = $cwd;

  ## Collect the components into {'values'} somehow
  foreach my $key (keys %{$prjc->{'valid_components'}}) {
    my @list = $prjc->get_component_list($key);
    $self->{'values'}->{$key} = \@list if (defined $list[0]);
  }

  ## If there is a staticname and no sharedname then this project
  ## 'type_is_static'.  If we are generating static projects, let
  ## all of the templates know that we 'need_staticflags'.
  ## If there is a sharedname then this project 'type_is_dynamic'.
  my $sharedname = $prjc->get_assignment('sharedname');
  my $staticname = $prjc->get_assignment('staticname');
  if (!defined $sharedname && defined $staticname) {
    $self->{'override_target_type'} = 1;
    $self->{'values'}->{'type_is_static'}   = 1;
    $self->{'values'}->{'need_staticflags'} = 1;
  }
  elsif ($prjc->get_static() == 1) {
    $self->{'values'}->{'need_staticflags'} = 1;
  }
  elsif (defined $sharedname) {
    $self->{'values'}->{'type_is_dynamic'} = 1;
  }

  ## If there is a sharedname or exename then this project
  ## 'type_is_binary'.
  if (defined $sharedname ||
      defined $prjc->get_assignment('exename')) {
    $self->{'values'}->{'type_is_binary'} = 1;
  }

  ## A tiny hack (mainly for VC6 projects)
  ## for the workspace creator.  It needs to know the
  ## target names to match up with the project name.
  $prjc->update_project_info($self, 0, ['project_name']);

  ## This is for all projects
  $prjc->update_project_info($self, 1, ['after']);

  ## VC7 Projects need to know the GUID.
  ## We need to save this value in our known values
  ## since each guid generated will be different.  We need
  ## this to correspond to the same guid used in the workspace.
  my $guid = $prjc->update_project_info($self, 1, ['guid']);
  $self->{'values'}->{'guid'} = $guid;

  ## In order for VC7 to mix languages, we need to keep track
  ## of the language associated with each project.
  $prjc->update_project_info($self, 1, ['language']);

  ## For VC7+ to properly work with wince, which is cross compiled,
  ## a new platform-specific token is added, nocross, which is used
  ## to determine if a project is even to be built for non-native
  ## targets. Additionally, custom-only projects are built but not
  ## deployed, thus these are added to the project_info mix
  $prjc->update_project_info($self, 1, ['custom_only']);
  $prjc->update_project_info($self, 1, ['nocross']);

  ## For VC8 to be able to add references to managed DLL's to the current
  ## managed DLL project (if it is one), we need to keep track of whether
  ## the project is 'managed' or not.
  $prjc->update_project_info($self, 1, ['managed']);

  ## Some Windows based projects can't deal with certain version
  ## values.  So, for those we provide a translated version.
  my $version = $prjc->get_assignment('version');
  if (defined $version) {
    $self->{'values'}->{'win_version'} =
                        WinVersionTranslator::translate($version);
  }
}


sub parse_line {
  my($self, $ih, $line) = @_;
  my $errorString;
  my $startempty = ($line eq '');

  ## If processing a foreach or the line only
  ## contains a keyword, then we do
  ## not need to add a newline to the end.
  if ($self->{'foreach'}->{'processing'} == 0 && !$self->{'eval'} &&
      ($line !~ /^[ ]*<%(\w+)(?:\((?:(?:\w+\s*,\s*)*[!]?\w+\(.+\)|[^\)]+)\))?%>$/ ||
       !defined $keywords{$1})) {
    $line .= $self->{'crlf'};
  }

  if ($self->{'foreach'}->{'count'} < 0 && !$self->{'eval'}) {
    $self->{'built'} = '';
  }

  my $start = index($line, '<%');
  if ($start >= 0) {
    my $append_name;
    if ($start > 0) {
      if (!$self->{'if_skip'}) {
        $self->append_current(substr($line, 0, $start));
      }
      $line = substr($line, $start);
    }

    my $nlen = 0;
    foreach my $item (split('<%', $line)) {
      my $name   = 1;
      my $length = length($item);
      my $endi   = index($item, '%>');
      for(my $i = 0; $i < $length; ++$i) {
        if ($i == $endi) {
          ++$i;
          $endi = index($item, '%>', $i);
          $name = undef;
          if ($append_name) {
            $append_name = undef;
            if (!$self->{'if_skip'}) {
              $self->append_current('%>');
            }
          }
          if ($length != $i + 1) {
            if (!$self->{'if_skip'}) {
              $self->append_current(substr($item, $i + 1));
            }
            last;
          }
        }
        elsif ($name) {
          my $efcheck = (index($item, 'endfor%>') == 0);
          my $focheck = ($efcheck ? 0 : (index($item, 'foreach(') == 0));

          if ($focheck && $self->{'foreach'}->{'count'} >= 0) {
            ++$self->{'foreach'}->{'nested'};
          }

          if ($self->{'foreach'}->{'count'} < 0 ||
              $self->{'foreach'}->{'processing'} > $self->{'foreach'}->{'nested'} ||
              (($efcheck || $focheck) &&
               $self->{'foreach'}->{'nested'} == $self->{'foreach'}->{'processing'})) {
            ($errorString, $nlen) = $self->process_name($item);

            if (defined $errorString) {
              return 0, $errorString;
            }
            elsif ($nlen == 0) {
              return 0, "Could not parse this line at column $i";
            }

            $i += ($nlen - 1);
          }
          else  {
            $name = undef;
            $nlen = ($i < $endi ? $endi : $length) - $i;
            if (!$self->{'if_skip'}) {
              $self->append_current('<%' . substr($item, $i, $nlen));
              $append_name = 1;
            }
            $i += ($nlen - 1);
          }

          if ($efcheck && $self->{'foreach'}->{'nested'} > 0) {
            --$self->{'foreach'}->{'nested'};
          }
        }
        else {
          $nlen = ($i < $endi ? $endi : $length) - $i;
          if (!$self->{'if_skip'}) {
            $self->append_current(substr($item, $i, $nlen));
          }
          $i += ($nlen - 1);
        }
      }
    }
  }
  else {
    $self->append_current($line) if (!$self->{'if_skip'});
  }

  if ($self->{'foreach'}->{'count'} < 0 && !$self->{'eval'} &&
    ## If the line started out empty and we're not
    ## skipping from the start or the built up line is not empty
      ($startempty ||
        ($self->{'built'} ne $self->{'crlf'} && $self->{'built'} ne ''))) {
    push(@{$self->{'lines'}}, $self->{'built'});
  }

  return !defined $errorString, $errorString;
}


sub parse_file {
  my($self, $input) = @_;

  $self->collect_data();
  my($status, $errorString) = $self->cached_file_read($input);

  ## If there was no error, check the stack to make sure that we aren't
  ## missing an <%endif%> or an <%endfor%>.
  if ($status && defined $self->{'sstack'}->[0]) {
    $status = 0;
    $errorString = "Missing an '$self->{'sstack'}->[0]' starting at " .
                   $self->{'lstack'}->[0];
  }

  ## Add in the line number if there is an error
  $errorString = "$input: line " .
                 $self->get_line_number() . ":\n$errorString" if (!$status);

  return $status, $errorString;
}


sub get_lines {
  return $_[0]->{'lines'};
}


# ************************************************************
# Accessors used by support scripts
# ************************************************************

sub getKeywords {
  return \%keywords;
}


sub getArrowOp {
  return \%arrow_op_ref;
}


1;
