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
use File::Basename;

# ************************************************************
# Data Section
# ************************************************************

my($index)    = 0;
my(@progress) = ("|", "/", "-", "\\");

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
                         'version'  => 1.2,
                         'types'    => {},
                         'creators' => \@creators,
                         'signif'   => 3,
                         'default'  => $creators[0],
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
  my($spaces) = (" " x (length($base) + 8));
  print STDERR "$base v$self->{'version'}\n" .
               "Usage: $base [-global <file>] [-include <directory>]\n" .
               $spaces . "[-ti <dll | lib | dll_exe | lib_exe>:<file>]\n" .
               $spaces . "[-template <file>] " .
               "[-dynamic_only] [-static_only]\n" .
               $spaces . "[-relative NAME=VAR] [-noreldefs]\n" .
               $spaces . "[-value_template <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-value_project <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-type <";

  my(@keys) = sort keys %{$self->{'types'}};
  for(my $i = 0; $i <= $#keys; $i++) {
    print STDERR "$keys[$i]";
    if ($i != $#keys) {
      print STDERR " | ";
    }
  }
  print STDERR ">]\n" .
               $spaces . "[files]\n\n";

  my($default) = lc(substr($self->{'default'}, 0, $self->{'signif'}));
  print STDERR
"       -global         Specifies the global input file.  Values stored\n" .
"                       within this file are applied to all projects.\n" .
"       -include        Specifies a directory to search when looking for base\n" .
"                       projects, template input files and templates.  This\n" .
"                       option can be used multiple times to add directories.\n" .
"       -ti             Specifies the template input file (with no extension)\n" .
"                       for the specific type as shown above\n" .
"                       (ex. -ti dll_exe:vc8exe)\n" .
"       -template       Specifies the template name (with no extension).\n" .
"       -dynamic_only   Specifies that only dynamic projects will be generated.\n" .
"       -static_only    Specifies that only static projects will be generated.\n" .
"       -relative       Any \$() variable in an mpc that is matched to NAME\n" .
"                       is replaced by VAR only if VAR can be made into a\n" .
"                       relative path based on the current working directory.\n" .
"       -noreldefs      Do not try to generate default relative definitions.\n" .
"       -value_template This option allows modification of a template input\n" .
"                       name value pair.  Use += to add VAL to the NAME's\n" .
"                       value.  Use -= to subtract and = to override the value.\n" .
"                       It is important to note that this will only modify\n" .
"                       existing template input name value pairs and can not\n" .
"                       be used to introduce new name value pairs.\n" .
"       -value_project  This option allows modification of a project variable\n" .
"                       assignment .  Use += to add VAL to the NAME's value.\n" .
"                       Use -= to subtract and = to override the value.\n" .
"                       This can be used to introduce new name value pairs to\n" .
"                       a project.  However, it must be a valid project\n" .
"                       assignment.\n" .
"       -type           Specifies the type of project file to generate.  This\n" .
"                       option can be used multiple times to generate multiple\n" .
"                       types.  If -type is not used, it defaults to '$default'.\n";

  exit(0);
}


sub completion_command {
  my($self) = shift;
  my($str)  = "complete $self->{'name'} " .
              "'c/-/(global include type template relative " .
              "ti dynamic_only static_only noreldefs)/' " .
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
  my($template)   = undef;
  my(%ti)         = ();
  my($dynamic)    = 1;
  my($static)     = 1;
  my($signif)     = $self->{'signif'};
  my(%relative)   = ();
  my($reldefs)    = 1;
  my(%addtemp)    = ();
  my(%addproj)    = ();

  ## Dynamically load in each perl module and set up
  ## the type tags and project creators
  my($creators) = $self->{'creators'};
  foreach my $creator (@$creators) {
    my($tag) = lc(substr($creator, 0, $signif));
    $self->{'types'}->{$tag} = $creator;
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
    elsif ($arg eq '-noreldefs') {
      $reldefs = 0;
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
    elsif ($arg eq '-value_template') {
      $i++;
      my($value) = $args[$i];
      if (!defined $value) {
        $self->usageAndExit("-value_template requires a variable assignment argument");
      }
      else {
        if ($value =~ /(\w+)\s*([\-+]?=)\s*(.*)/) {
          my($name) = $1;
          my($op)   = $2;
          my($val)  = $3;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;
          if ($op eq "+=") {
            $op = 1;
          }
          elsif ($op eq "-=") {
            $op = -1;
          }
          else {
            $op = 0;
          }
          $addtemp{$name} = [$op, $val];
        }
        else {
          $self->usageAndExit("Invalid option to -value_template");
        }
      }
    }
    elsif ($arg eq '-value_project') {
      $i++;
      my($value) = $args[$i];
      if (!defined $value) {
        $self->usageAndExit("-value_project requires a variable assignment argument");
      }
      else {
        if ($value =~ /(\w+)\s*([\-+]?=)\s*(.*)/) {
          my($name) = $1;
          my($op)   = $2;
          my($val)  = $3;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;
          if ($op eq "+=") {
            $op = 1;
          }
          elsif ($op eq "-=") {
            $op = -1;
          }
          else {
            $op = 0;
          }
          $addproj{$name} = [$op, $val];
        }
        else {
          $self->usageAndExit("Invalid option to -value_project");
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
    push(@generators, $self->{'default'});
  }
  if (!defined $global) {
    $global = $self->{'path'} . "/config/global.mpb";
  }
  ## Always add the default include paths
  unshift(@include, $self->{'path'} . "/templates");
  unshift(@include, $self->{'path'} . "/config");

  if ($reldefs) {
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
  }

  ## Set up un-buffered output for the progress callback
  $| = 1;

  ## Set up a hash that we can use to keep track of what
  ## has been 'required'
  my(%loaded) = ();
  ## Generate the files
  foreach my $file (@input) {
    ## To correctly reference any pathnames in the input file, chdir to
    ## its directory if there's any directory component to the specified path.
    my $orig_dir = Cwd::getcwd();
    my $dir = dirname($file);
    my $base = basename($file);
    if ($base ne $file) {
      chdir($dir);
      $file = $base;
    }
    foreach my $name (@generators) {
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }
      my($generator) = $name->new($global, \@include, $template,
                                  \%ti, $dynamic, $static, \%relative,
                                  \%addtemp, \%addproj,
                                  (-t 1 ? \&progress : undef));
      print "Generating output using " .
            ($file eq "" ? "default input" : $file) . "\n";
      print "Start Time: " . scalar(localtime(time())) . "\n";
      if (!$generator->generate($file)) {
        print STDERR "Unable to process: $file\n";
        $status++;
      }
      print "  End Time: " . scalar(localtime(time())) . "\n";
    }
    chdir($orig_dir);
  }

  return $status;
}


sub progress {
  ## This method will be called before each output
  ## file (or set of output files in vc6's case) is written.
  print "$progress[$index]\r";
  $index++;
  if ($index > $#progress) {
    $index = 0;
  }
}


1;
