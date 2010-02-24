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

use DirectoryManager;
use StringProcessor;
use ProjectCreator;

# ************************************************************
# Subroutine Section
# ************************************************************

sub printUsage {
  my $self    = shift;
  my $msg     = shift;
  my $base    = shift;
  my $version = shift;
  my @types   = @_;

  print STDERR "ERROR: $msg\n" if (defined $msg);

  my $spaces = (' ' x (length($base) + 8));
  print STDERR "$base v$version\n" .
               "Usage: $base [-global <file>] [-include <directory>] [-recurse]\n" .
               $spaces . "[-ti <dll | lib | dll_exe | lib_exe>:<file>] [-hierarchy]\n" .
               $spaces . "[-template <file>] [-relative NAME=VAL] [-base <project>]\n" .
               $spaces . "[-noreldefs] [-notoplevel] [-static] [-genins] [-use_env]\n" .
               $spaces . "[-value_template <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-value_project <NAME+=VAL | NAME=VAL | NAME-=VAL>]\n" .
               $spaces . "[-make_coexistence] [-feature_file <file name>] [-gendot]\n" .
               $spaces . "[-expand_vars] [-features <feature definitions>]\n" .
               $spaces . "[-exclude <directories>] [-name_modifier <pattern>]\n" .
               $spaces . "[-apply_project] [-version] [-into <directory>]\n" .
               $spaces . "[-gfeature_file <file name>] [-nocomments]\n" .
               $spaces . "[-relative_file <file name>] [-for_eclipse]\n" .
               $spaces . "[-language <";

  my $olen = length($spaces) + 12;
  my $len  = $olen;
  my $mlen = 77;
  my @keys = sort(Creator::validLanguages());
  for(my $i = 0; $i <= $#keys; $i++) {
    my $klen = length($keys[$i]);
    $len += $klen;
    if ($len > $mlen) {
      print STDERR "\n$spaces        ";
      $len = $olen + $klen;
    }
    print STDERR $keys[$i];
    if ($i != $#keys) {
      print STDERR ' | ';
      $len += 3;
    }
  }
  print STDERR ">]\n",
               $spaces, "[-type <";

  $olen = length($spaces) + 8;
  $len  = $olen;

  ## Sort the project types, but keep those that are the same with different
  ## version numbers in the right order (i.e., vc8, vc9, vc10).  The vc71
  ## type is a special case and needs to stay betwen vc7 and vc8.
  @keys = sort { if ($a ne 'vc71' && $b ne 'vc71' && $a =~ /^([^\d]+)(\d+)$/) {
                   my($a1, $a2) = ($1, $2);
                   if ($b =~ /^([^\d]+)(\d+)$/ && $a1 eq $1) {
                     return $a2 <=> $2;
                   }
                 }
                 return $a cmp $b;
               } @types;
  for(my $i = 0; $i <= $#keys; $i++) {
    my $klen = length($keys[$i]);
    $len += $klen;
    if ($len > $mlen) {
      print STDERR "\n$spaces        ";
      $len = $olen + $klen;
    }
    print STDERR $keys[$i];
    if ($i != $#keys) {
      print STDERR ' | ';
      $len += 3;
    }
  }
  print STDERR ">]\n" .
               $spaces . "[files]\n\n";

  print STDERR
"       -base           Add <project> as a base project to each generated\n" .
"                       project file.  Do not provide a file extension, the\n" .
"                       .mpb extension will be tried first; if that fails the\n" .
"                       .mpc extension will be tried.\n" .
"       -exclude        Use this option to exclude directories or files when\n" .
"                       searching for input files.\n" .
"       -expand_vars    Perform direct expansion, instead of performing relative\n" .
"                       replacement with either -use_env or -relative options.\n" .
"       -feature_file   Specifies the feature file to read before processing.\n" .
"                       The default feature file is default.features under the\n" .
"                       config directory.\n" .
"       -features       Specifies the feature list to set before processing.\n" .
"       -for_eclipse    Generate files for use with eclipse.  This is only\n" .
"                       useful for make based project types.\n" .
"       -gendot         Generate .dot files for use with Graphviz.\n" .
"       -genins         Generate .ins files for use with prj_install.pl.\n" .
"       -gfeature_file  Specifies the global feature file.  The\n" .
"                       default value is global.features under the\n" .
"                       config directory.\n" .
"       -global         Specifies the global input file.  Values stored\n" .
"                       within this file are applied to all projects.\n" .
"       -hierarchy      Generate a workspace in a hierarchical fashion.\n" .
"       -include        Specifies a directory to search when looking for base\n" .
"                       projects, template input files and templates.  This\n" .
"                       option can be used multiple times to add directories.\n" .
"       -into           Place all output files in a mirrored directory\n" .
"                       structure starting at <directory>.  This should be a\n" .
"                       full path.\n" .
"       -language       Specify the language preference; possible values are\n",
"                       [", join(', ', sort(Creator::validLanguages())), "].  The default is\n".
"                       " . Creator::defaultLanguage() . ".\n",
"       -make_coexistence If multiple 'make' based project types are\n" .
"                       generated, they will be named such that they can coexist.\n" .
"       -name_modifier  Modify output names.  The pattern passed to this\n" .
"                       parameter will have the '*' portion replaced with the\n" .
"                       actual output name.  Ex. *_Static\n" .
"       -apply_project  When used in conjunction with -name_modifier, it applies\n" .
"                       the name modifier to the project name also.\n" .
"       -nocomments     Do not place comments in the generated files.\n" .
"       -noreldefs      Do not try to generate default relative definitions.\n" .
"       -notoplevel     Do not generate the top level target file.  Files\n" .
"                       are still process, but no top level file is created.\n" .
"       -recurse        Recurse from the current directory and generate from\n" .
"                       all found input files.\n" .
"       -relative       Any \$() variable in an mpc file that is matched to NAME\n" .
"                       is replaced by VAL only if VAL can be made into a\n" .
"                       relative path based on the current working directory.\n" .
"                       This option can be used multiple times to add multiple\n" .
"                       variables.\n" .
"       -relative_file  Specifies the relative file to read before processing.\n" .
"                       The default relative file is default.rel under the\n" .
"                       config directory.\n" .
"       -static         Specifies that only static projects will be generated.\n" .
"                       By default, only dynamic projects are generated.\n" .
"       -template       Specifies the template name (with no extension).\n" .
"       -ti             Specifies the template input file (with no extension)\n" .
"                       for the specific type (ex. -ti dll_exe:vc8exe).\n" .
"       -type           Specifies the type of project file to generate.  This\n" .
"                       option can be used multiple times to generate multiple\n" .
"                       types.  There is no longer a default.\n" .
"       -use_env        Use environment variables for all uses of \$() instead\n" .
"                       of the relative replacement values.\n" .
"       -value_project  This option allows modification of a project variable\n" .
"                       assignment .  Use += to add VAL to the NAME's value.\n" .
"                       Use -= to subtract and = to override the value.\n" .
"                       This can be used to introduce new name value pairs to\n" .
"                       a project.  However, it must be a valid project\n" .
"                       assignment.\n" .
"       -value_template This option allows modification of a template input\n" .
"                       name value pair.  Use += to add VAL to the NAME's\n" .
"                       value.  Use -= to subtract and = to override the value.\n" .
"       -version        Print the MPC version and exit.\n";
}


sub optionError {
  #my $self = shift;
  #my $str  = shift;
}


sub completion_command {
  my($self, $name, $types) = @_;
  my $str = "complete $name " .
            "'c/-/(gendot genins global include type template relative " .
            "ti static noreldefs notoplevel feature_file use_env " .
            "value_template value_project make_coexistence language " .
            "hierarchy exclude name_modifier apply_project version " .
            "expand_vars gfeature_file nocomments for_eclipse relative_file)/' " .
            "'c/dll:/f/' 'c/dll_exe:/f/' 'c/lib_exe:/f/' 'c/lib:/f/' " .
            "'n/-ti/(dll lib dll_exe lib_exe)/:' ";

  $str .= "'n/-language/(";
  my @keys = sort(Creator::validLanguages());
  for(my $i = 0; $i <= $#keys; $i++) {
    $str .= $keys[$i];
    $str .= " " if ($i != $#keys);
  }
  $str .= ")/' 'n/-type/(";

  @keys = sort keys %$types;
  for(my $i = 0; $i <= $#keys; $i++) {
    $str .= $keys[$i];
    $str .= " " if ($i != $#keys);
  }
  $str .= ")/'";
  return $str;
}


sub options {
  my $self     = shift;
  my $name     = shift;
  my $types    = shift;
  my $defaults = shift;
  my @args     = @_;
  my @include;
  my @input;
  my @creators;
  my @baseprojs;
  my %ti;
  my %relative;
  my %addtemp;
  my %addproj;
  my @exclude;
  my $global;
  my $template;
  my $feature_f;
  my $gfeature_f;
  my $relative_f;
  my @features;
  my $nmodifier;
  my $into;
  my $hierarchy = 0;
  my $language = ($defaults ? Creator::defaultLanguage() : undef);
  my $dynamic = ($defaults ? 1 : undef);
  my $comments = ($defaults ? 1 : undef);
  my $reldefs = ($defaults ? 1 : undef);
  my $toplevel = ($defaults ? 1 : undef);
  my $use_env = ($defaults ? 0 : undef);
  my $expandvars = ($defaults ? 0 : undef);
  my $static = ($defaults ? 0 : undef);
  my $recurse = ($defaults ? 0 : undef);
  my $makeco = ($defaults ? 0 : undef);
  my $applypj = ($defaults ? 0 : undef);
  my $genins = ($defaults ? 0 : undef);
  my $gendot = ($defaults ? 0 : undef);
  my $foreclipse = ($defaults ? 0 : undef);

  ## Process the command line arguments
  for(my $i = 0; $i <= $#args; $i++) {
    my $arg = $args[$i];
    $arg =~ s/^--/-/;

    if ($arg eq '-apply_project') {
      $applypj = 1;
    }
    elsif ($arg eq '-complete') {
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
        my $type = lc($args[$i]);
        if (defined $types->{$type}) {
          my $call  = $types->{$type};
          my $found = 0;
          foreach my $creator (@creators) {
            if ($creator eq $call) {
              $found = 1;
              last;
            }
          }
          push(@creators, $call) if (!$found);
        }
        else {
          $self->optionError("Invalid type: $args[$i]");
        }
      }
    }
    elsif ($arg eq '-exclude') {
      $i++;
      if (defined $args[$i]) {
        foreach my $exclude (split(',', $args[$i])) {
          push(@exclude, DirectoryManager::mpc_glob(undef, $exclude));
        }
      }
      else {
        $self->optionError('-exclude requires a ' .
                           'comma separated list argument');
      }
    }
    elsif ($arg eq '-expand_vars') {
      $expandvars = 1;
    }
    elsif ($arg eq '-feature_file') {
      $i++;
      $feature_f = $args[$i];
      if (!defined $feature_f) {
        $self->optionError('-feature_file requires a file name argument');
      }
    }
    elsif ($arg eq '-features') {
      $i++;
      if (defined $args[$i]) {
        @features = split(',', $args[$i]);
      }
      else {
        $self->optionError('-features requires a comma separated list argument');
      }
    }
    elsif ($arg eq '-for_eclipse') {
      $foreclipse = 1;
    }
    elsif ($arg eq '-gfeature_file') {
      $i++;
      $gfeature_f = $args[$i];
      if (!defined $gfeature_f) {
        $self->optionError('-gfeature_file ' .
                           'requires a file name argument');
      }
    }
    elsif ($arg eq '-relative_file') {
      $i++;
      $relative_f = $args[$i];
      if (!defined $relative_f) {
        $self->optionError('-relative_file ' .
                           'requires a file name argument');
      }
    }
    elsif ($arg eq '-gendot') {
      $gendot = 1;
    }
    elsif ($arg eq '-genins') {
      $genins = 1;
    }
    elsif ($arg eq '-global') {
      $i++;
      $global = $args[$i];
      if (!defined $global) {
        $self->optionError('-global requires a file name argument');
      }
    }
    elsif ($arg eq '-help') {
      $self->optionError();
    }
    elsif ($arg eq '-hierarchy') {
      $hierarchy = 1;
    }
    elsif ($arg eq '-include') {
      $i++;
      my $include = $args[$i];
      if (!defined $include) {
        $self->optionError('-include requires a directory argument');
      }
      else {
        ## If the specified include path is relative, expand it based on
        ## the current working directory.
        if ($include !~ /^[\/\\]/ &&
            $include !~ /^[A-Za-z]:[\/\\]?/) {
          $include = DirectoryManager::getcwd() . '/' . $include;
        }

        push(@include, $include);
      }
    }
    elsif ($arg eq '-into') {
      $i++;
      $into = $args[$i];
      if (!defined $into) {
        $self->optionError('-into requires a directory argument');
      }
    }
    elsif ($arg eq '-language') {
      $i++;
      $language = $args[$i];
      if (!defined $language) {
        $self->optionError('-language requires a language argument');
      }
      elsif (!Creator::isValidLanguage($language)) {
        $self->optionError("$language is not a valid language");
      }
    }
    elsif ($arg eq '-make_coexistence') {
      $makeco = 1;
    }
    elsif ($arg eq '-name_modifier') {
      $i++;
      my $nmod = $args[$i];
      if (!defined $nmod) {
        $self->optionError('-name_modifier requires a modifier argument');
      }
      else {
        $nmodifier = $nmod;
      }
    }
    elsif ($arg eq '-nocomments') {
      $comments = 0;
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
      my $rel = $args[$i];
      if (!defined $rel) {
        $self->optionError('-relative requires a variable assignment argument');
      }
      else {
        if ($rel =~ /(\w+)\s*=\s*(.*)/) {
          my $name = $1;
          my $val  = $2;
          $val =~ s/^\s+//;
          $val =~ s/\s+$//;

          ## If the specified path is relative, expand it based on
          ## the current working directory.
          if ($val !~ /^[\/\\]/ &&
              $val !~ /^[A-Za-z]:[\/\\]?/) {
            $val = DirectoryManager::getcwd() . '/' . $val;
          }

          ## Clean up the path as much as possible.  For some reason,
          ## File::Spec->canonpath() on Windows doesn't remove trailing
          ## /. from the path.
          $relative{$name} = File::Spec->canonpath($val);
          $relative{$name} =~ s/\\/\//g;
          $relative{$name} =~ s!/\.$!!;
        }
        else {
          $self->optionError('Invalid argument to -relative');
        }
      }
    }
    elsif ($arg eq '-ti') {
      $i++;
      my $tmpi = $args[$i];
      if (!defined $tmpi) {
        $self->optionError('-ti requires a template input argument');
      }
      else {
        if ($tmpi =~ /((dll|lib|dll_exe|lib_exe):)?(.*)/) {
          my $key  = $2;
          my $name = $3;
          if (defined $key) {
            $ti{$key} = $name;
          }
          else {
            foreach my $type ('dll', 'lib', 'dll_exe', 'lib_exe') {
              $ti{$type} = $name;
            }
          }
        }
        else {
          $self->optionError("Invalid -ti argument: $tmpi");
        }
      }
    }
    elsif ($arg eq '-use_env') {
      $use_env = 1;
    }
    elsif ($arg eq '-value_template') {
      $i++;
      my $value = $args[$i];
      if (!defined $value) {
        $self->optionError('-value_template requires a variable assignment argument');
      }
      else {
        my @values;
        my $pc = new ProjectCreator();
        if ($pc->parse_assignment($value, \@values)) {
          $addtemp{$values[1]} = [] if (!defined $addtemp{$values[1]});
          ## The extra parameter (3rd) indicates that this value was
          ## specified on the command line.  This "extra parameter" is
          ## used in ProjectCreator::update_template_variable().
          push(@{$addtemp{$values[1]}}, [$values[0], $values[2], 1]);

          my $keywords = ProjectCreator::getKeywords();
          if (defined $$keywords{$values[1]}) {
            $self->warning($values[1] . ' is a project keyword; you ' .
                           'should use -value_project instead.');
          }
        }
        else {
          $self->optionError('Invalid argument to -value_template');
        }
      }
    }
    elsif ($arg eq '-value_project') {
      $i++;
      my $value = $args[$i];
      if (!defined $value) {
        $self->optionError('-value_project requires a variable assignment argument');
      }
      else {
        my @values;
        my $pc = new ProjectCreator();
        if ($pc->parse_assignment($value, \@values)) {
          $addproj{$values[1]} = [] if (!defined $addproj{$values[1]});
          push(@{$addproj{$values[1]}}, [$values[0], $values[2]]);
        }
        else {
          $self->optionError('Invalid argument to -value_project');
        }
      }
    }
    elsif ($arg eq '-version') {
      print 'MPC v', Version::get(), "\n";
      return undef;
    }
    elsif ($arg eq '-static') {
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

  return {'global'           => $global,
          'feature_file'     => $feature_f,
          'gfeature_file'    => $gfeature_f,
          'relative_file'    => $relative_f,
          'features'         => \@features,
          'for_eclipse'      => $foreclipse,
          'include'          => \@include,
          'input'            => \@input,
          'comments'         => $comments,
          'creators'         => \@creators,
          'baseprojs'        => \@baseprojs,
          'template'         => $template,
          'ti'               => \%ti,
          'dynamic'          => $dynamic,
          'static'           => $static,
          'relative'         => \%relative,
          'reldefs'          => $reldefs,
          'toplevel'         => $toplevel,
          'recurse'          => $recurse,
          'addtemp'          => \%addtemp,
          'addproj'          => \%addproj,
          'make_coexistence' => $makeco,
          'hierarchy'        => $hierarchy,
          'exclude'          => \@exclude,
          'name_modifier'    => $nmodifier,
          'apply_project'    => $applypj,
          'gendot'           => $gendot,
          'genins'           => $genins,
          'into'             => $into,
          'language'         => $language,
          'use_env'          => $use_env,
          'expand_vars'      => $expandvars,
         };
}


sub is_set {
  my($self, $key, $options) = @_;

  if (defined $options->{$key}) {
    if (UNIVERSAL::isa($options->{$key}, 'ARRAY')) {
      return 'ARRAY' if (defined $options->{$key}->[0]);
    }
    elsif (UNIVERSAL::isa($options->{$key}, 'HASH')) {
      my @keys = keys %{$options->{$key}};
      return 'HASH' if (defined $keys[0]);
    }
    else {
      return 'SCALAR';
    }
  }

  return undef;
}

1;
