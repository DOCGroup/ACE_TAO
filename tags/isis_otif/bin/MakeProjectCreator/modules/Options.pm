package Options;

# ************************************************************
# Description   : Process mpc command line options
# Author        : Chad Elliott
# Create Date   : 3/20/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub optionError {
  #my($self) = shift;
  #my($str)  = shift;
}


sub completion_command {
  my($self)  = shift;
  my($name)  = shift;
  my($types) = shift;
  my($str)   = "complete $name " .
               "'c/-/(global include type template relative " .
               "ti static noreldefs notoplevel " .
               "value_template value_project)/' " .
               "'c/dll:/f/' 'c/dll_exe:/f/' 'c/lib_exe:/f/' 'c/lib:/f/' " .
               "'n/-ti/(dll lib dll_exe lib_exe)/:' 'n/-type/(";

  my(@keys) = sort keys %$types;
  for(my $i = 0; $i <= $#keys; $i++) {
    $str .= $keys[$i];
    if ($i != $#keys) {
      $str .= " ";
    }
  }
  $str .= ")/'";
  return $str;
}


sub options {
  my($self)       = shift;
  my($name)       = shift;
  my($types)      = shift;
  my($defaults)   = shift;
  my(@args)       = @_;
  my(@include)    = ();
  my(@input)      = ();
  my(@generators) = ();
  my(@baseprojs)  = ();
  my(%ti)         = ();
  my(%relative)   = ();
  my(%addtemp)    = ();
  my(%addproj)    = ();
  my($global)     = undef;
  my($template)   = undef;
  my($dynamic)    = ($defaults ? 1 : undef);
  my($reldefs)    = ($defaults ? 1 : undef);
  my($toplevel)   = ($defaults ? 1 : undef);
  my($static)     = ($defaults ? 0 : undef);
  my($recurse)    = ($defaults ? 0 : undef);

  ## Process the command line arguments
  for(my $i = 0; $i <= $#args; $i++) {
    my($arg) = $args[$i];
    if ($arg eq '-complete') {
      print $self->completion_command($name, $types) . "\n";
      return undef;
    }
    elsif ($arg eq '-base') {
      $i++;
      if (!defined $args[$i]) {
        $self->optionError('-base requires an argument');
      }
      else {
        push(@baseprojs, $args[$i]);
      }
    }
    elsif ($arg eq '-type') {
      $i++;
      if (!defined $args[$i]) {
        $self->optionError('-type requires an argument');
      }
      else {
        my($type) = lc($args[$i]);
        if (defined $types->{$type}) {
          my($call)  = $types->{$type};
          my($found) = 0;
          foreach my $generator (@generators) {
            if ($generator eq $call) {
              $found = 1;
              last;
            }
          }
          if (!$found) {
            push(@generators, $call);
          }
        }
        else {
          $self->optionError("Invalid type: $args[$i]");
        }
      }
    }
    elsif ($arg eq '-global') {
      $i++;
      $global = $args[$i];
      if (!defined $global) {
        $self->optionError('-global requires a file name argument');
      }
    }
    elsif ($arg eq '-include') {
      $i++;
      my($include) = $args[$i];
      if (!defined $include) {
        $self->optionError('-include requires a directory argument');
      }
      else {
        push(@include, $include);
      }
    }
    elsif ($arg eq '-noreldefs') {
      $reldefs = 0;
    }
    elsif ($arg eq '-notoplevel') {
      $toplevel = 0;
    }
    elsif ($arg eq '-recurse') {
      $recurse = 1;
    }
    elsif ($arg eq '-template') {
      $i++;
      $template = $args[$i];
      if (!defined $template) {
        $self->optionError('-template requires a file name argument');
      }
    }
    elsif ($arg eq '-relative') {
      $i++;
      my($rel) = $args[$i];
      if (!defined $rel) {
        $self->optionError('-relative requires a variable assignment argument');
      }
      else {
        if ($rel =~ /(\w+)\s*=\s*(.*)/) {
          my($name) = $1;
          my($val)  = $2;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;
          $relative{$name} = $val;
        }
        else {
          $self->optionError('Invalid option to -relative');
        }
      }
    }
    elsif ($arg eq '-ti') {
      $i++;
      my($tmpi) = $args[$i];
      if (!defined $tmpi) {
        $self->optionError('-ti requires a template input argument');
      }
      else {
        if ($tmpi =~ /(dll|lib|dll_exe|lib_exe):(.*)/) {
          my($key)  = $1;
          my($name) = $2;
          $ti{$key} = $name;
        }
        else {
          $self->optionError("Invalid -ti argument: $tmpi");
        }
      }
    }
    elsif ($arg eq '-value_template') {
      $i++;
      my($value) = $args[$i];
      if (!defined $value) {
        $self->optionError('-value_template requires a variable assignment argument');
      }
      else {
        if ($value =~ /(\w+)\s*([\-+]?=)\s*(.*)/) {
          my($name) = $1;
          my($op)   = $2;
          my($val)  = $3;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;
          if ($op eq '+=') {
            $op = 1;
          }
          elsif ($op eq '-=') {
            $op = -1;
          }
          else {
            $op = 0;
          }
          $addtemp{$name} = [$op, $val];
        }
        else {
          $self->optionError('Invalid option to -value_template');
        }
      }
    }
    elsif ($arg eq '-value_project') {
      $i++;
      my($value) = $args[$i];
      if (!defined $value) {
        $self->optionError('-value_project requires a variable assignment argument');
      }
      else {
        if ($value =~ /(\w+)\s*([\-+]?=)\s*(.*)/) {
          my($name) = $1;
          my($op)   = $2;
          my($val)  = $3;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;
          if ($op eq '+=') {
            $op = 1;
          }
          elsif ($op eq '-=') {
            $op = -1;
          }
          else {
            $op = 0;
          }
          $addproj{$name} = [$op, $val];
        }
        else {
          $self->optionError('Invalid option to -value_project');
        }
      }
    }
    elsif ($arg eq '-static') {
      $static  = 1;
    }
    elsif ($arg eq '-static_only') {
      $static  = 1;
      $dynamic = 0;
    }
    elsif ($arg =~ /^-/) {
      $self->optionError("Unknown option: $arg");
    }
    else {
      push(@input, $arg);
    }
  }

  my(%options) = ('global'     => $global,
                  'include'    => \@include,
                  'input'      => \@input,
                  'generators' => \@generators,
                  'baseprojs'  => \@baseprojs,
                  'template'   => $template,
                  'ti'         => \%ti,
                  'dynamic'    => $dynamic,
                  'static'     => $static,
                  'relative'   => \%relative,
                  'reldefs'    => $reldefs,
                  'toplevel'   => $toplevel,
                  'recurse'    => $recurse,
                  'addtemp'    => \%addtemp,
                  'addproj'    => \%addproj,
                 );

  return \%options;
}


sub is_set {
  my($self)    = shift;
  my($key)     = shift;
  my($options) = shift;

  if (UNIVERSAL::isa($options->{$key}, 'ARRAY')) {
    if (defined $options->{$key}->[0]) {
      return 'ARRAY';
    }
  }
  elsif (UNIVERSAL::isa($options->{$key}, 'HASH')) {
    my(@keys) = keys %{$options->{$key}};
    if (defined $keys[0]) {
      return 'HASH';
    }
  }
  elsif (defined $options->{$key}) {
    return 'SCALAR';
  }

  return undef;
}

1;
