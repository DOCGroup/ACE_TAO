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
use Parser;
use Version;

use vars qw(@ISA);
@ISA = qw(Parser Options);

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
  $self->{'types'}    = {};
  $self->{'creators'} = \@creators;
  $self->{'default'}  = $creators[0];
  $self->{'reldefs'}  = {};
  $self->{'relorder'} = [];

  return $self;
}


sub convert_slashes {
  #my($self) = shift;
  return 0;
}


sub parse_line {
  my($self)        = shift;
  my($ih)          = shift;
  my($line)        = shift;
  my($status)      = 1;
  my($errorString) = undef;

  if ($line eq '') {
  }
  elsif ($line =~ /^([\w\*]+)(\s*,\s*(.*))?$/) {
    my($name)  = $1;
    my($value) = $3;
    if (defined $value) {
      $value =~ s/^\s+//;
      $value =~ s/\s+$//;
    }
    if ($name =~ /\*/) {
      $name =~ s/\*/.*/g;
      foreach my $key (sort keys %ENV) {
        if ($key =~ /^$name$/ && !exists $self->{'reldefs'}->{$key}) {
          ## Put this value at the front since it doesn't need
          ## to be built up from anything else.  It is a stand-alone
          ## relative definition.
          $self->{'reldefs'}->{$key} = undef;
          unshift(@{$self->{'relorder'}}, $key);
        }
      }
    }
    else {
      $self->{'reldefs'}->{$name} = $value;
      if (defined $value) {
        ## This relative definition may need to be built up from an
        ## existing value, so it needs to be put at the end.
        push(@{$self->{'relorder'}}, $name);
      }
      else {
        ## Put this value at the front since it doesn't need
        ## to be built up from anything else.  It is a stand-alone
        ## relative definition.
        unshift(@{$self->{'relorder'}}, $name);
      }
    }
  }
  else {
    $status = 0;
    $errorString = "Unrecognized line: $line";
  }

  return $status, $errorString;
}


sub optionError {
  my($self) = shift;
  my($line) = shift;
  my($base) = $self->{'name'};

  if (defined $line) {
    $self->error($line);
  }
  my($spaces) = (' ' x (length($base) + 8));
  print STDERR "$base v" . Version::get() . "\n" .
               "Usage: $base [-global <file>] [-include <directory>] [-recurse]]\n" .
               $spaces . "[-ti <dll | lib | dll_exe | lib_exe>:<file>] [-hierarchy]\n" .
               $spaces . "[-template <file>] [-relative NAME=VAR] [-base <project>]\n" .
               $spaces . "[-noreldefs] [-notoplevel] [-static]\n" .
               $spaces . "[-value_template <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-value_project <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-feature_file <file name>] [-make_coexistence]\n" .
               $spaces . "[-exclude <directories>] [-name_modifier <pattern>]\n" .
               $spaces . "[-apply_project]\n" .
               $spaces . "[-type <";

  my(@keys) = sort keys %{$self->{'types'}};
  for(my $i = 0; $i <= $#keys; $i++) {
    print STDERR $keys[$i];
    if ($i != $#keys) {
      print STDERR ' | ';
    }
    if ($i != $#keys && (($i + 1) % 6) == 0) {
      print STDERR "\n$spaces        ";
    }
  }
  print STDERR ">]\n" .
               $spaces . "[files]\n\n";

  my($default) = $self->extractType($self->{'default'});
  print STDERR
"       -base           Add <project> as a base project to each generated\n" .
"                       project file.\n" .
"       -exclude        Use this option to exclude directories when searching\n" .
"                       for input files.\n" .
"       -feature_file   Specifies the feature file to read before processing.\n" .
"                       The default feature file is default.features under the\n" .
"                       config directory.\n" .
"       -global         Specifies the global input file.  Values stored\n" .
"                       within this file are applied to all projects.\n" .
"       -hierarchy      Generate a workspace in a hierarchical fashion.\n" .
"       -include        Specifies a directory to search when looking for base\n" .
"                       projects, template input files and templates.  This\n" .
"                       option can be used multiple times to add directories.\n" .
"       -ti             Specifies the template input file (with no extension)\n" .
"                       for the specific type as shown above\n" .
"                       (ex. -ti dll_exe:vc8exe)\n" .
"       -template       Specifies the template name (with no extension).\n" .
"       -static         Specifies that only static projects will be generated.\n" .
"                       By default, only dynamic projects will be generated.\n" .
"       -recurse        Recurse from the current directory and generate from\n" .
"                       all found input files.\n" .
"       -relative       Any \$() variable in an mpc that is matched to NAME\n" .
"                       is replaced by VAR only if VAR can be made into a\n" .
"                       relative path based on the current working directory.\n" .
"       -make_coexistence If multiple 'make' based project types are\n" .
"                       generated, they will be named such that they can coexist.\n" .
"       -name_modifier  Modify output names.  The pattern passed to this\n" .
"                       parameter will have the '*' portion replaced with the\n" .
"                       actual output name.  Ex. *_Static\n" .
"       -apply_project  When used in conjunction with -name_modifier, it applies\n" .
"                       the name modifier to the project name also.\n" .
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

  ## Set up the default creator, if no type is selected
  if (!defined $options->{'creators'}->[0]) {
    push(@{$options->{'creators'}}, $self->{'default'});
  }

  if ($options->{'recurse'}) {
    if (defined $options->{'input'}->[0]) {
      ## This is an error.
      ## -recurse was used and input files were specified.
      $self->optionError('No files should be ' .
                         'specified when using -recurse');
    }
    else {
      ## We have to load at least one creator here in order
      ## to call the generate_recursive_input_list virtual function.
      my($name) = $options->{'creators'}->[0];
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }

      ## Generate the recursive input list
      my($creator) = $name->new();
      my(@input) = $creator->generate_recursive_input_list(
                                              '.', $options->{'exclude'});
      $options->{'input'} = \@input;

      ## If no files were found above, then we issue a warning
      ## that we are going to use the default input
      if (!defined $options->{'input'}->[0]) {
        $self->information('No files were found using the -recurse option. ' .
                           'Using the default input.');
      }
    }
  }

  ## Set the global feature file
  my($global_feature_file) = $self->{'path'} . '/config/global.features';

  ## Set up default values
  if (!defined $options->{'input'}->[0]) {
    push(@{$options->{'input'}}, '');
  }
  if (!defined $options->{'feature_file'}) {
    my($feature_file) = $self->{'path'} . '/config/default.features';
    if (-r $feature_file) {
      $options->{'feature_file'} = $feature_file;
    }
  }
  if (!defined $options->{'global'}) {
    my($global) = $self->{'path'} . '/config/global.mpb';
    if (-r $global) {
      $options->{'global'} = $global;
    }
  }
  ## Save the original directory outside of the loop
  ## to avoid calling it multiple times.
  my($orig_dir) = $self->getcwd();

  ## Always add the default include paths
  unshift(@{$options->{'include'}}, $orig_dir);
  unshift(@{$options->{'include'}}, $self->{'path'} . '/templates');
  unshift(@{$options->{'include'}}, $self->{'path'} . '/config');

  if ($options->{'reldefs'}) {
    ## Only try to read the file if it exists
    my($rel) = $self->{'path'} . '/config/default.rel';
    if (-r $rel) {
      my($srel, $errorString) = $self->read_file($rel);
      if (!$srel) {
        $self->error("$errorString\nin $rel");
        return $status;
      }
    }

    foreach my $key (@{$self->{'relorder'}}) {
      if (defined $ENV{$key} &&
          !defined $options->{'relative'}->{$key}) {
        $options->{'relative'}->{$key} = $ENV{$key};
      }
      if (defined $self->{'reldefs'}->{$key} &&
          !defined $options->{'relative'}->{$key}) {
        my($value) = $self->{'reldefs'}->{$key};
        if ($value =~ /\$(\w+)(.*)?/) {
          my($var)   = $1;
          my($extra) = $2;
          $options->{'relative'}->{$key} =
                     (defined $options->{'relative'}->{$var} ?
                              $options->{'relative'}->{$var} : '') .
                     (defined $extra ? $extra : '');
        }
        else {
          $options->{'relative'}->{$key} = $value;
        }
      }

      ## If a relative path is defined, remove all trailing slashes
      ## and replace any two or more slashes with a single slash.
      if (defined $options->{'relative'}->{$key}) {
        $options->{'relative'}->{$key} =~ s/([\/\\])[\/\\]+/$1/g;
        $options->{'relative'}->{$key} =~ s/[\/\\]$//g;
      }
    }
  }

  ## Set up un-buffered output for the progress callback
  $| = 1;

  ## Generate the files
  foreach my $cfile (@{$options->{'input'}}) {
    ## To correctly reference any pathnames in the input file, chdir to
    ## its directory if there's any directory component to the specified path.
    my($base) = basename($cfile);

    if (-d $cfile) {
      $base = '';
    }

    foreach my $name (@{$options->{'creators'}}) {
      if (!$loaded{$name}) {
        require "$name.pm";
        $loaded{$name} = 1;
      }
      my($file) = $cfile;
      my($creator) = $name->new($options->{'global'},
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
                                  $options->{'baseprojs'},
                                  $global_feature_file,
                                  $options->{'feature_file'},
                                  $options->{'hierarchy'},
                                  $options->{'exclude'},
                                  $options->{'coexistence'},
                                  $options->{'name_modifier'},
                                  $options->{'apply_project'});
      if ($base ne $file) {
        my($dir) = ($base eq '' ? $file : dirname($file));
        if (!$creator->cd($dir)) {
          $self->error("Unable to change to directory: $dir");
          $status++;
          last;
        }
        $file = $base;
      }
      print 'Generating output using ';
      if ($file eq '') {
        print 'default input';
      }
      else {
        my($partial)  = $self->getcwd();
        my($oescaped) = $self->escape_regex_special($orig_dir) . '(/)?';
        $partial =~ s/^$oescaped//;
        print '' . ($partial ne '' ? "$partial/" : '') . $file;
      }
      print "\n" . 'Start Time: ' . scalar(localtime(time())) . "\n";
      if (!$creator->generate($file)) {
        $self->error("Unable to process: " .
                     ($file eq '' ? 'default input' : $file));
        $status++;
        last;
      }
      print '  End Time: ' . scalar(localtime(time())) . "\n";
      $creator->cd($orig_dir);
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
