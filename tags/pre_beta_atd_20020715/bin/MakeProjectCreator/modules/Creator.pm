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
use Cwd;
use FileHandle;
use File::Basename;

use Parser;

use vars qw(@ISA);
@ISA = qw(Parser);

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)    = shift;
  my($global)   = shift;
  my($inc)      = shift;
  my($template) = shift;
  my($ti)       = shift;
  my($relative) = shift;
  my($type)     = shift;
  my($self)     = Parser::new($class);

  $self->{'relative'}     = $relative;
  $self->{'template'}     = $template;
  $self->{'ti'}           = $ti;
  $self->{'global_cfg'}   = $global;
  $self->{'grammar_type'} = $type;
  $self->{'type_check'}   = $type . '_defined';
  $self->{'global_read'}  = 0;
  $self->{'include_path'} = $inc;

  return $self;
}


sub generate_default_input {
  my($self) = shift;
  $self->parse_line(undef, "$self->{'grammar_type'} {");
  $self->parse_line(undef, "}");
  return 1;
}


sub parse_file {
  my($self)       = shift;
  my($input)      = shift;
  my($typecheck)  = $self->{'type_check'};
  my($oline)      = $self->line_number();

  ## Read the input file and get the last line number
  my($status, $errorString) = $self->read_file($input);
  my($linenumber) = $self->line_number();

  if (!$status) {
    print STDERR "$input: line $linenumber: $errorString\n";
  }
  elsif ($status && $self->{$typecheck}) {
    ## If we are at the end of the file and the type we are looking at
    ## is still defined, then we have an error
    print STDERR "$input: line $linenumber: ERROR: Did not " .
                 "find the end of the $self->{'grammar_type'}\n";
    $status = 0;
  }
  $self->line_number($oline);

  return $status;
}


sub generate {
  my($self)   = shift;
  my($input)  = shift;
  my($status) = 1;

  ## Allow subclasses to reset values before
  ## each call to generate().
  $self->reset_values();

  ## Read the global configuration file
  if (!$self->{'global_read'}) {
    $status = $self->read_global_configuration();
    $self->{'global_read'} = 1;
  }

  if ($status) {
    ## An empty input file name says that we
    ## should generate a default input file and use that
    if ($input eq "") {
      $status = $self->generate_default_input();
    }
    else {
      $status = $self->parse_file($input);
    }
  }

  return $status;
}


sub parse_known {
  my($self)        = shift;
  my($line)        = shift;
  my($status)      = 1;
  my($errorString) = "";
  my($type)        = $self->{'grammar_type'};
  my(@values)      = ();
  my($typecheck)   = $self->{'type_check'};

  ##
  ## Each regexp that looks for the '{' looks for it at the
  ## end of the line.  It is purposely this way to decrease
  ## the amount of extra lines in each file.  This
  ## allows for the most compact file as human readably
  ## possible.
  ##
  if ($line eq "") {
  }
  elsif ($line =~ /^$type\s*(\([^\)]+\))?\s*(:.*)?\s*{$/) {
    my($name)    = $1;
    my($parents) = $2;
    if ($self->{$typecheck}) {
      $errorString = "ERROR: Did not find the end of the $type";
      $status = 0;
    }
    else {
      if (defined $parents) {
        my(@parents) = ();
        foreach my $parent (split(/[:,]/, $parents)) {
          $parent =~ s/^\s+//;
          $parent =~ s/\s+$//;
          if ($parent ne "") {
            push(@parents, $parent);
          }
        }
        $parents = \@parents;
      }
      push(@values, $type, $name, $parents);
    }
  }
  elsif ($line =~ /^}$/) {
    if ($self->{$typecheck}) {
      push(@values, $type, $line);
    }
    else {
      $errorString = "ERROR: Did not find the beginning of the $type";
      $status = 0;
    }
  }
  elsif (!$self->{$typecheck}) {
    $errorString = "ERROR: No $type was defined";
    $status = 0;
  }
  elsif ($line =~ /^(\w+)\s*=\s*(.*)?/) {
    my($name)  = lc($1);
    my($value) = $2;
    push(@values, "assignment", $name, $value);
  }
  elsif ($line =~ /^(\w+)\s*\+=\s*(.*)?/) {
    my($name)  = lc($1);
    my($value) = $2;
    push(@values, "assign_add", $name, $value);
  }
  elsif ($line =~ /^(\w+)\s*\-=\s*(.*)?/) {
    my($name)  = lc($1);
    my($value) = $2;
    push(@values, "assign_sub", $name, $value);
  }
  elsif ($line =~ /^(\w+)\s*(\([^\)]+\))?\s*{$/) {
    my($comp) = lc($1);
    my($name) = $2;

    if (defined $name) {
      $name =~ s/^\(\s*//;
      $name =~ s/\s*\)$//;
    }
    else {
      $name = "default";
    }
    push(@values, "component", $comp, $name);
  }
  else {
    $errorString = "ERROR: Unrecognized line: $line";
    $status = -1;
  }

  return $status, $errorString, @values;
}


sub base_directory {
  my($self) = shift;
  return basename(getcwd());
}


sub generate_default_file_list {
  my($self)  = shift;
  my($dir)   = shift;
  my($dh)    = new FileHandle();
  my(@files) = ();

  if (!defined $dir) {
    $dir = ".";
  }

  if (opendir($dh, $dir)) {
    @files = grep(!/^\.\.?$/, readdir($dh));
    if ($self->sort_files()) {
      @files = sort { $self->file_sorter($a, $b) } @files;
    }

    ## Prefix each file name with the directory
    ## only if it's not .
    if ($dir ne ".") {
      for(my $i = 0; $i <= $#files; $i++) {
        $files[$i] = "$dir/$files[$i]";
      }
    }

    closedir($dh);
  }
  return @files;
}


sub get_global_cfg {
  my($self) = shift;
  return $self->{'global_cfg'};
}


sub get_include_path {
  my($self) = shift;
  return $self->{'include_path'};
}


sub search_include_path {
  my($self)     = shift;
  my($file)     = shift;
  my($found)    = undef;
  my($includes) = $self->get_include_path();
  foreach my $include (@$includes) {
    if (-r "$include/$file") {
      $found = "$include/$file";
      last;
    }
  }
  return $found;
}


sub get_template_override {
  my($self) = shift;
  return $self->{'template'};
}


sub get_ti_override {
  my($self) = shift;
  return $self->{'ti'};
}


sub get_relative {
  my($self) = shift;
  return $self->{'relative'};
}


sub windows_crlf {
  my($self) = shift; 
  if ($^O eq 'MSWin32') {
    return "\n";
  }
  else {
    return "\r\n";
  }
}


sub transform_file_name {
  my($self) = shift;
  my($name) = shift;

  $name =~ s/\s/_/g;
  return $name;
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub crlf {
  my($self) = shift;
  return "\n";
}


sub reset_values {
  my($self) = shift;
}


sub sort_files {
  my($self) = shift;
  return 1;
}


sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return $left cmp $right;
}


sub read_global_configuration {
  my($self)  = shift;
  my($input) = shift;
  return 1;
}


1;
