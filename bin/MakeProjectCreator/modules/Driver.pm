package Driver;

# ************************************************************
# Description   : Functionality to call a workspace or project creator
# Author        : Chad Elliott
# Create Date   : 5/28/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)    = shift;
  my($path)     = shift;
  my($name)     = shift;
  my(@creators) = @_;
  my($self)     = bless {'path'     => $path,
                         'name'     => $name,
                         'version'  => 0.8,
                         'types'    => {},
                         'creators' => \@creators,
                        }, $class;
  return $self;
}


sub usageAndExit {
  my($self) = shift;
  my($line) = shift;
  my($base) = $self->{'name'};

  if (defined $line) {
    print STDERR "$line\n";
  }
  print STDERR "$base v$self->{'version'}\n" .
               "Usage: $base [-global <file>] [-include <directory>]\n" .
               (" " x (length($base) + 8)) .
               "[-ti <dll | lib | dll_exe | lib_exe>:<file>]\n" .
               (" " x (length($base) + 8)) . "[-template <file>] " .
               "[-dynamic_only] [-static_only]\n" .
               (" " x (length($base) + 8)) . "[-relative NAME=VAR]\n" .
               (" " x (length($base) + 8)) . "[-type <";

  my(@keys) = sort keys %{$self->{'types'}};
  for(my $i = 0; $i <= $#keys; $i++) {
    print STDERR "$keys[$i]";
    if ($i != $#keys) {
      print STDERR " | ";
    }
  }
  print STDERR ">]\n" .
               (" " x (length($base) + 8)) . "[files]\n\n";

  print STDERR
"       -global       Specifies the global input file.  Values stored\n" .
"                     within this file are applied to all projects.\n" .
"       -include      Specifies a directory to search when looking for base\n" .
"                     projects, template input files and templates.  This\n" .
"                     option can be used multiple times to add directories.\n" .
"       -ti           Specifies the template input file (with no extension)\n" .
"                     for the specific type as shown above\n" .
"                     (ex. -ti dll_exe:vc8exe)\n" .
"       -template     Specifies the template name (with no extension).\n" .
"       -dynamic_only Specifies that only dynamic projects will be generated.\n" .
"       -static_only  Specifies that only static projects will be generated.\n" .
"       -relative     Any \$() variable in an mpc that is matched to NAME\n" .
"                     is replaced by VAR only if VAR can be made into a\n" .
"                     relative path based on the current working directory.\n" .
"       -type         Specifies the type of project file to generate.  This\n" .
"                     option can be used multiple times to generate multiple\n" .
"                     types.\n";

  exit(0);
}


sub completion_command {
  my($self) = shift;
  my($str)  = "complete $self->{'name'} " .
              "'c/-/(global include type template relative " .
              "ti dynamic_only static_only)/' " .
              "'c/dll:/f/' 'c/dll_exe:/f/' 'c/lib_exe:/f/' 'c/lib:/f/' " .
              "'n/-ti/(dll lib dll_exe lib_exe)/:' 'n/-type/(";

  my(@keys) = sort keys %{$self->{'types'}};
  for(my $i = 0; $i <= $#keys; $i++) {
    $str .= $keys[$i];
    if ($i != $#keys) {
      $str .= " ";
    }
  }
  $str .= ")/'";
  return $str;
}


sub run {
  my($self)       = shift;
  my(@args)       = @_;
  my($global)     = undef;
  my(@include)    = ();
  my(@input)      = ();
  my(@generators) = ();
  my($status)     = 0;
  my($default)    = undef;
  my($template)   = undef;
  my(%ti)         = ();
  my($dynamic)    = 1;
  my($static)     = 1;
  my($signif)     = 3;
  my(%relative)   = ();

  ## Dynamically load in each perl module and set up
  ## the type tags and project creators
  my($creators) = $self->{'creators'};
  foreach my $creator (@$creators) {
    my($tag) = lc(substr($creator, 0, $signif));
    $self->{'types'}->{$tag} = $creator;
    if (!defined $default) {
      $default = $creator;
    }
    require "$creator.pm";
  }

  for(my $i = 0; $i <= $#args; $i++) {
    my($arg) = $args[$i];
    if ($arg eq '-complete') {
      print $self->completion_command() . "\n";
      return $status;
    }
    elsif ($arg eq '-type') {
      $i++;
      if (!defined $args[$i]) {
        $self->usageAndExit("-type requires an argument");
      }

      my($type) = lc(substr($args[$i], 0, $signif));
      if (defined $self->{'types'}->{$type}) {
        my($call) = $self->{'types'}->{$type};
        push(@generators, $call);
      }
      else {
        $self->usageAndExit("Invalid type: $args[$i]");
      }
    }
    elsif ($arg eq '-global') {
      $i++;
      $global = $args[$i];
      if (!defined $global) {
        $self->usageAndExit("-global requires a file name argument");
      }
    }
    elsif ($arg eq '-include') {
      $i++;
      my($include) = $args[$i];
      if (!defined $include) {
        $self->usageAndExit("-include requires a directory argument");
      }
      push(@include, $include);
    }
    elsif ($arg eq '-template') {
      $i++;
      $template = $args[$i];
      if (!defined $template) {
        $self->usageAndExit("-template requires a file name argument");
      }
    }
    elsif ($arg eq '-relative') {
      $i++;
      my($rel) = $args[$i];
      if (!defined $rel) {
        $self->usageAndExit("-relative requires a variable assignment argument");
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
          $self->usageAndExit("Invalid option to -relative");
        }
      }
    }
    elsif ($arg eq '-ti') {
      $i++;
      my($tmpi) = $args[$i];
      if (!defined $tmpi) {
        $self->usageAndExit("-ti requires a template input argument");
      }
      else {
        if ($tmpi =~ /(dll|lib|dll_exe|lib_exe):(.*)/) {
          my($key)  = $1;
          my($name) = $2;
          $ti{$key} = $name;
        }
        else {
          $self->usageAndExit("Invalid -ti argument: $tmpi");
        }
      }
    }
    elsif ($arg eq '-dynamic_only') {
      $static  = 0;
      $dynamic = 1;
    }
    elsif ($arg eq '-static_only') {
      $static  = 1;
      $dynamic = 0;
    }
    elsif ($arg =~ /^-/) {
      $self->usageAndExit();
    }
    else {
      push(@input, $arg);
    }
  }

  ## Set up default values
  if (!defined $input[0]) {
    push(@input, "");
  }
  if (!defined $generators[0]) {
    push(@generators, $default);
  }
  if (!defined $global) {
    $global = $self->{'path'} . "/config/global.mpb";
  }
  if (!defined $include[0]) {
    push(@include, $self->{'path'} . "/config");
    push(@include, $self->{'path'} . "/templates");
  }
  if (!defined $relative{'ACE_ROOT'} && defined $ENV{ACE_ROOT}) {
    $relative{'ACE_ROOT'} = $ENV{ACE_ROOT};
  }
  if (!defined $relative{'TAO_ROOT'}) {
    if (defined $ENV{TAO_ROOT}) {
      $relative{'TAO_ROOT'} = $ENV{TAO_ROOT};
    }
    else {
      $relative{'TAO_ROOT'} = "$relative{ACE_ROOT}/TAO";
    }
  }

  ## Generate the files
  foreach my $file (@input) {
    foreach my $name (@generators) {
      my($generator) = $name->new($global, \@include, $template,
                                  \%ti, $dynamic, $static, \%relative);
      print "Generating output using " .
            ($file eq "" ? "default input" : $file) . "\n";
      print "Start Time: " . scalar(localtime(time())) . "\n";
      if (!$generator->generate($file)) {
        print STDERR "Unable to process: $file\n";
        $status++;
      }
      print "  End Time: " . scalar(localtime(time())) . "\n";
    }
  }

  return $status;
}


1;
