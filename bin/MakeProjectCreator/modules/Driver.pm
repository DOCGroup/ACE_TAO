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

use StringProcessor;

use vars qw(@ISA);
@ISA = qw(StringProcessor);

# ************************************************************
# Data Section
# ************************************************************

my($index)    = 0;
my(@progress) = ('|', '/', '-', '\\');
my($cmdenv)   = 'MPC_COMMANDLINE';

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)    = shift;
  my($path)     = shift;
  my($name)     = shift;
  my(@creators) = @_;
  my($self)     = $class->SUPER::new();

  $self->{'path'}     = $path;
  $self->{'name'}     = $name;
  $self->{'version'}  = 1.5;
  $self->{'types'}    = {};
  $self->{'creators'} = \@creators;
  $self->{'default'}  = $creators[0];

  return $self;
}


sub usageAndExit {
  my($self) = shift;
  my($line) = shift;
  my($base) = $self->{'name'};

  if (defined $line) {
    print STDERR "$line\n";
  }
  my($spaces) = (' ' x (length($base) + 8));
  print STDERR "$base v$self->{'version'}\n" .
               "Usage: $base [-global <file>] [-include <directory>] [-recurse]\n" .
               $spaces . "[-ti <dll | lib | dll_exe | lib_exe>:<file>]\n" .
               $spaces . "[-template <file>] [-relative NAME=VAR]\n" .
               $spaces . "[-noreldefs] [-notoplevel] [-static] [-static_only]\n" .
               $spaces . "[-value_template <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-value_project <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-type <";

  my(@keys) = sort keys %{$self->{'types'}};
  for(my $i = 0; $i <= $#keys; $i++) {
    print STDERR $keys[$i];
    if ($i != $#keys) {
      print STDERR ' | ';
    }
    if ((($i + 1) % 6) == 0) {
      print STDERR "\n$spaces        ";
    }
  }
  print STDERR ">]\n" .
               $spaces . "[files]\n\n";

  my($default) = $self->extractType($self->{'default'});
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
"       -static         Specifies that static projects will be generated in\n" .
"                       addition to dynamic projects.\n" .
"       -static_only    Specifies that only static projects will be generated.\n" .
"       -recurse        Recurse from the current directory and generate from\n" .
"                       all found input files.\n" .
"       -relative       Any \$() variable in an mpc that is matched to NAME\n" .
"                       is replaced by VAR only if VAR can be made into a\n" .
"                       relative path based on the current working directory.\n" .
"       -noreldefs      Do not try to generate default relative definitions.\n" .
"       -notoplevel     Do not generate the top level target file.  Files\n" .
"                       are still process, but no top level file is created.\n" .
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
              "ti static noreldefs notoplevel " .
              "value_template value_project)/' " .
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
  my($static)     = 0;
  my(%relative)   = ();
  my($reldefs)    = 1;
  my($toplevel)   = 1;
  my($recurse)    = 0;
  my(%addtemp)    = ();
  my(%addproj)    = ();

  ## Dynamically load in each perl module and set up
  ## the type tags and project creators
  my($creators) = $self->{'creators'};
  foreach my $creator (@$creators) {
    my($tag) = $self->extractType($creator);
    $self->{'types'}->{$tag} = $creator;
  }

  ## Before we process the arguments, we will prepend the $cmdenv
  ## environment variable.
  if (defined $ENV{$cmdenv}) {
    my($envargs) = $self->create_array($ENV{$cmdenv});
    unshift(@args, @$envargs);
  }

  ## Process the command line arguments
  for(my $i = 0; $i <= $#args; $i++) {
    my($arg) = $args[$i];
    if ($arg eq '-complete') {
      print $self->completion_command() . "\n";
      return $status;
    }
    elsif ($arg eq '-type') {
      $i++;
      if (!defined $args[$i]) {
        $self->usageAndExit('-type requires an argument');
      }

      my($type) = lc($args[$i]);
      if (defined $self->{'types'}->{$type}) {
        my($call)  = $self->{'types'}->{$type};
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
        $self->usageAndExit("Invalid type: $args[$i]");
      }
    }
    elsif ($arg eq '-global') {
      $i++;
      $global = $args[$i];
      if (!defined $global) {
        $self->usageAndExit('-global requires a file name argument');
      }
    }
    elsif ($arg eq '-include') {
      $i++;
      my($include) = $args[$i];
      if (!defined $include) {
        $self->usageAndExit('-include requires a directory argument');
      }
      push(@include, $include);
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
        $self->usageAndExit('-template requires a file name argument');
      }
    }
    elsif ($arg eq '-relative') {
      $i++;
      my($rel) = $args[$i];
      if (!defined $rel) {
        $self->usageAndExit('-relative requires a variable assignment argument');
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
          $self->usageAndExit('Invalid option to -relative');
        }
      }
    }
    elsif ($arg eq '-ti') {
      $i++;
      my($tmpi) = $args[$i];
      if (!defined $tmpi) {
        $self->usageAndExit('-ti requires a template input argument');
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
        $self->usageAndExit('-value_template requires a variable assignment argument');
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
          $self->usageAndExit('Invalid option to -value_template');
        }
      }
    }
    elsif ($arg eq '-value_project') {
      $i++;
      my($value) = $args[$i];
      if (!defined $value) {
        $self->usageAndExit('-value_project requires a variable assignment argument');
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
          $self->usageAndExit('Invalid option to -value_project');
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
      $self->usageAndExit();
    }
    else {
      push(@input, $arg);
    }
  }

  ## Set up a hash that we can use to keep track of what
  ## has been 'required'
  my(%loaded) = ();

  ## Set up the default generator, if no type is selected
  if (!defined $generators[0]) {
    push(@generators, $self->{'default'});
  }

  if ($recurse) {
    if (defined $input[0]) {
      ## This is an error.
      ## -recurse was used and input files were specified.
      $self->usageAndExit('No files should be ' .
                          'specified when using -recurse');
    }
    else {
      ## We have to load at least one generator here in order
      ## to call the generate_recursive_input_list virtual function.
      my($name) = $generators[0];
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }

      ## Generate the recursive input list
      my($generator) = $name->new();
      @input = $generator->generate_recursive_input_list('.');

      ## If no files were found above, then we issue a warning
      ## that we are going to use the default input
      if (!defined $input[0]) {
        print "WARNING: No files were found using the -recurse option.\n" .
              "         Using the default input.\n";
      }
    }
  }

  ## Set up default values
  if (!defined $input[0]) {
    push(@input, '');
  }
  if (!defined $global) {
    $global = $self->{'path'} . '/config/global.mpb';
  }
  ## Always add the default include paths
  unshift(@include, $self->{'path'} . '/templates');
  unshift(@include, $self->{'path'} . '/config');

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
    if (!defined $relative{'CIAO_ROOT'}) {
      if (defined $ENV{CIAO_ROOT}) {
        $relative{'CIAO_ROOT'} = $ENV{CIAO_ROOT};
      }
      else {
        $relative{'CIAO_ROOT'} = "$relative{ACE_ROOT}/TAO/CIAO";
      }
    }
  }

  ## Set up un-buffered output for the progress callback
  $| = 1;

  ## Save the original directory outside of the loop
  ## to avoid calling it multiple times.
  my($orig_dir) = Cwd::getcwd();

  ## Generate the files
  foreach my $file (@input) {
    ## To correctly reference any pathnames in the input file, chdir to
    ## its directory if there's any directory component to the specified path.
    my($base) = basename($file);
    foreach my $name (@generators) {
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }
      my($generator) = $name->new($global, \@include, $template,
                                  \%ti, $dynamic, $static, \%relative,
                                  \%addtemp, \%addproj,
                                  (-t 1 ? \&progress : undef),
                                  $toplevel);
      if ($base ne $file) {
        my($dir) = dirname($file);
        if (!$generator->cd($dir)) {
          print STDERR "ERROR: Unable to change to directory: $dir\n";
          $status++;
          last;
        }
        $file = $base;
      }
      print 'Generating output using ' .
            ($file eq '' ? 'default input' : $file) . "\n";
      print 'Start Time: ' . scalar(localtime(time())) . "\n";
      if (!$generator->generate($file)) {
        print STDERR "ERROR: Unable to process: $file\n";
        $status++;
        last;
      }
      print '  End Time: ' . scalar(localtime(time())) . "\n";
      $generator->cd($orig_dir);
    }
    if ($status) {
      last;
    }
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
