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

use Options;
use StringProcessor;

use vars qw(@ISA);
@ISA = qw(StringProcessor Options);

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
  $self->{'version'}  = 1.6;
  $self->{'types'}    = {};
  $self->{'creators'} = \@creators;
  $self->{'default'}  = $creators[0];

  return $self;
}


sub optionError {
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
               $spaces . "[-template <file>] [-relative NAME=VAR] [-base <project>]\n" .
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
"       -base           Add <project> as a base project to each generated\n" .
"                       project file.\n" .
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


sub run {
  my($self)       = shift;
  my(@args)       = @_;
  my($status)     = 0;

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

  my($options) = $self->options($self->{'name'},
                                $self->{'types'},
                                1,
                                @args);
  if (!defined $options) {
    return $status;
  }

  ## Set up a hash that we can use to keep track of what
  ## has been 'required'
  my(%loaded) = ();

  ## Set up the default generator, if no type is selected
  if (!defined $options->{'generators'}->[0]) {
    push(@{$options->{'generators'}}, $self->{'default'});
  }

  if ($options->{'recurse'}) {
    if (defined $options->{'input'}->[0]) {
      ## This is an error.
      ## -recurse was used and input files were specified.
      $self->optionError('No files should be ' .
                         'specified when using -recurse');
    }
    else {
      ## We have to load at least one generator here in order
      ## to call the generate_recursive_input_list virtual function.
      my($name) = $options->{'generators'}->[0];
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }

      ## Generate the recursive input list
      my($generator) = $name->new();
      my(@input) = $generator->generate_recursive_input_list('.');
      $options->{'input'} = \@input;

      ## If no files were found above, then we issue a warning
      ## that we are going to use the default input
      if (!defined $options->{'input'}->[0]) {
        print "WARNING: No files were found using the -recurse option.\n" .
              "         Using the default input.\n";
      }
    }
  }

  ## Set up default values
  if (!defined $options->{'input'}->[0]) {
    push(@{$options->{'input'}}, '');
  }
  if (!defined $options->{'global'}) {
    $options->{'global'} = $self->{'path'} . '/config/global.mpb';
  }
  ## Always add the default include paths
  unshift(@{$options->{'include'}}, $self->{'path'} . '/templates');
  unshift(@{$options->{'include'}}, $self->{'path'} . '/config');

  if ($options->{'reldefs'}) {
    if (defined $ENV{ACE_ROOT} &&
        !defined $options->{'relative'}->{'ACE_ROOT'}) {
      $options->{'relative'}->{'ACE_ROOT'} = $ENV{ACE_ROOT};
    }
    if (!defined $options->{'relative'}->{'TAO_ROOT'}) {
      if (defined $ENV{TAO_ROOT}) {
        $options->{'relative'}->{'TAO_ROOT'} = $ENV{TAO_ROOT};
      }
      else {
        $options->{'relative'}->{'TAO_ROOT'} =
                  $options->{'relative'}->{'ACE_ROOT'} . '/TAO';
      }
    }
    if (!defined $options->{'relative'}->{'CIAO_ROOT'}) {
      if (defined $ENV{CIAO_ROOT}) {
        $options->{'relative'}->{'CIAO_ROOT'} = $ENV{CIAO_ROOT};
      }
      else {
        $options->{'relative'}->{'CIAO_ROOT'} =
                  $options->{'relative'}->{ACE_ROOT} . '/TAO/CIAO';
      }
    }
  }

  ## Set up un-buffered output for the progress callback
  $| = 1;

  ## Save the original directory outside of the loop
  ## to avoid calling it multiple times.
  my($orig_dir) = Cwd::getcwd();

  ## Generate the files
  foreach my $cfile (@{$options->{'input'}}) {
    ## To correctly reference any pathnames in the input file, chdir to
    ## its directory if there's any directory component to the specified path.
    my($base) = basename($cfile);
    foreach my $name (@{$options->{'generators'}}) {
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }
      my($file) = $cfile;
      my($generator) = $name->new($options->{'global'},
                                  $options->{'include'},
                                  $options->{'template'},
                                  $options->{'ti'},
                                  $options->{'dynamic'},
                                  $options->{'static'},
                                  $options->{'relative'},
                                  $options->{'addtemp'},
                                  $options->{'addproj'},
                                  (-t 1 ? \&progress : undef),
                                  $options->{'toplevel'},
                                  $options->{'baseprojs'});
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
