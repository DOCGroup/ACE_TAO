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

my(@keywords) = ('if', 'else', 'endif',
                 'noextension', 'dirname', 'basename', 'basenoextension',
                 'foreach', 'forfirst', 'fornotfirst',
                 'fornotlast', 'forlast', 'endfor',
                 'comment'
                );

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($prjc)  = shift;
  my($self)  = Parser::new($class);

  $self->{'prjc'}     = $prjc;
  $self->{'values'}   = {};
  $self->{'defaults'} = {};
  $self->{'lines'}    = [];
  $self->{'built'}    = "";
  $self->{'sstack'}   = [];
  $self->{'lstack'}   = [];
  $self->{'if_skip'}  = 0;

  $self->{'foreach'}  = {};
  $self->{'foreach'}->{'count'}      = -1;
  $self->{'foreach'}->{'nested'}     = 0;
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
  for(my $i = length($file) - 1; $i >= 0; $i--) {
    my($ch) = substr($file, $i, 1);
    if ($ch eq '/' || $ch eq '\\') {
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
  for(my $i = length($file) - 1; $i != 0; $i--) {
    my($ch) = substr($file, $i, 1);
    if ($ch eq '/' || $ch eq '\\') {
      $self->{'values'}->{'dirname_found'} = 1;
      return substr($file, 0, $i);
    }
  }
  delete $self->{'values'}->{'dirname_found'};
  return ".";
}


sub strip_line {
  my($self) = shift;
  my($line) = shift;

  ## Override strip_line() from Parser.
  ## We need to preserve leading space and
  ## there is no comment string in templates.
  $self->{'line_number'}++;
  $line =~ s/\s+$//;

  return $line;
}


sub is_keyword {
  my($self) = shift;
  my($name) = shift;

  foreach my $key (@keywords) {
    if ($name eq $key) {
      return 1;
    }
  }
  return 0;
}


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


sub set_current_values {
  my($self) = shift;
  my($name) = shift;

  ## If any value within a foreach matches the name
  ## of a hash table within the template input we will
  ## set the values of that hash tablein the current scope
  my($ti) = $self->{'prjc'}->get_template_input();
  if (defined $ti) {
    my($counter) = $self->{'foreach'}->{'count'};
    my($value) = $ti->get_value($name);
    if (defined $value && $counter >= 0 &&
        UNIVERSAL::isa($value, 'HASH')) {
      $self->{'foreach'}->{'temp_scope'}->[$counter] = $value;
    }
  }
}


sub get_value {
  my($self)    = shift;
  my($name)    = shift;
  my($value)   = undef;
  my($counter) = $self->{'foreach'}->{'count'};

  ## First, check the temporary scope (set inside a foreach)
  if ($counter >= 0) {
    while(!defined $value && $counter >= 0) {
      my($scope) = $self->{'foreach'}->{'temp_scope'}->[$counter];
      $value = $$scope{$name};
      $counter--;
    }
    $counter = $self->{'foreach'}->{'count'};
  }

  if (!defined $value) {
    ## Next, check for a template value
    my($ti) = $self->{'prjc'}->get_template_input();
    if (defined $ti) {
      $value = $ti->get_value($name);
    }

    if (!defined $value) {
      ## Next, check the inner to outer foreach
      ## scopes for overriding values
      while(!defined $value && $counter >= 0) {
        my($scope) = $self->{'foreach'}->{'scope'}->[$counter];
        $value = $$scope{$name};
        $counter--;
      }

      ## Then get the value from the project creator
      if (!defined $value) {
        $value = $self->{'prjc'}->get_assignment($name);

        ## Then get it from our known values
        if (!defined $value) {
          $value = $self->{'values'}->{$name};
        }
      }
    }
  }

  return $value;
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
      ## Call back onto the project creator to allow
      ## it to fill in the value before defaulting to an empty string.
      $value = $self->{'prjc'}->fill_value($name);
      if (!defined $value) {
#        print "DEBUG: WARNING: $name defaulting to empty string\n";
        $value = "";
      }
    }
    else {
#      print "DEBUG: WARNING: $name using default value of $value\n";
    }
  }

  return $value;
}


sub process_foreach {
  my($self)   = shift;
  my($index)  = $self->{'foreach'}->{'count'};
  my($name)   = $self->{'foreach'}->{'names'}->[$index];
  my($text)   = $self->{'foreach'}->{'text'}->[$index];
  my($status) = 1;
  my($errorString) = "";
  my(@values) = ();
  my($names)  = $self->create_array($name);

  $name = undef;
  foreach my $n (@$names) {
    my($vals) = $self->get_value($n);
    if (defined $vals && $vals ne "") {
      if (!UNIVERSAL::isa($vals, 'ARRAY')) {
        $vals = $self->create_array($vals);
      }
      push(@values, @$vals);
    }
    if (!defined $name) {
      $name = $n;
    }
  }

  ## Reset the text (it will be regenerated by calling parse_line
  $self->{'foreach'}->{'text'}->[$index] = "";

  if (defined $values[0]) {
    my($inner) = $name;
    my($scope) = $self->{'foreach'}->{'scope'}->[$index];

    $inner =~ s/s$//;
    $$scope{'forlast'}     = 0;
    $$scope{'fornotlast'}  = 1;
    $$scope{'forfirst'}    = 1;
    $$scope{'fornotfirst'} = 0;

    for(my $i = 0; $i <= $#values; $i++) {
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
      $$scope{$inner} = $value;

      ## A tiny hack for VC7
      if ($inner eq 'configuration') {
        $self->{'prjc'}->update_project_info($self, 1,
                                             ['configuration', 'platform'],
                                             '|');
      }

      ## Now parse the line of text, each time
      ## with different values
      $self->{'foreach'}->{'processing'}++;
      ($status, $errorString) = $self->parse_line(undef, $text);
      $self->{'foreach'}->{'processing'}--;
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
  my($errorString) = "";
  my($sstack)      = $self->{'sstack'};
  my($lstack)      = $self->{'lstack'};
  my($end)         = pop(@$sstack);
  pop(@$lstack);

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
      $self->{'foreach'}->{'count'}--;
      $self->append_current($self->{'foreach'}->{'text'}->[$index]);
    }
  }

  return $status, $errorString;
}


sub handle_if {
  my($self)   = shift;
  my($val)    = shift;
  my($sstack) = $self->{'sstack'};
  my($lstack) = $self->{'lstack'};
  my($name)   = 'endif';

  push(@$lstack, $self->line_number() . " $val");
  if (!$self->{'if_skip'}) {
    my($true)  = 1;
    push(@$sstack, $name);
    if ($val =~ /^!(.*)/) {
      $val = $1;
      $val =~ s/^\s+//;
      $true = 0;
    }
    if (!defined $self->get_value($val)) {
      $self->{'if_skip'} = $true;
    }
    else {
      $self->{'if_skip'} = !$true;
    }
  }
  else {
    push(@$sstack, "*$name");
  }
}


sub handle_else {
  my($self)   = shift;
  my($val)    = shift;
  my($sstack) = $self->{'sstack'};
  my(@scopy)  = @$sstack;
  my($name)   = "endif";

  ## This method does not take into account that
  ## multiple else clauses could be supplied to a single if.
  ## Someday, this may be fixed.
  if (defined $scopy[$#scopy] && $scopy[$#scopy] eq $name) {
    $self->{'if_skip'} ^= 1;
  }
}


sub handle_foreach {
  my($self)   = shift;
  my($val)    = shift;
  my($sstack) = $self->{'sstack'};
  my($lstack) = $self->{'lstack'};
  my($name)   = 'endfor';

  push(@$lstack, $self->line_number());
  if (!$self->{'if_skip'}) {
    push(@$sstack, $name);
    $self->{'foreach'}->{'count'}++;

    my($index) = $self->{'foreach'}->{'count'};
    $self->{'foreach'}->{'names'}->[$index] = $val;
    $self->{'foreach'}->{'text'}->[$index]  = "";
    $self->{'foreach'}->{'scope'}->[$index] = {};
  }
  else {
    push(@$sstack, "*$name");
  }
}


sub handle_special {
  my($self) = shift;
  my($name) = shift;
  my($val)  = shift;

  if ($self->get_value($name)) {
    $self->append_current($val);
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


sub split_name_value {
  my($self)   = shift;
  my($line)   = shift;
  my($length) = length($line);
  my($name)   = undef;
  my($val)    = undef;

  for(my $i = 0; $i < $length; $i++) {
    my($ch) = substr($line, $i, 1);
    if (!defined $name && $ch eq '(') {
      $name = substr($line, 0, $i);
      $val  = "";
    }
    elsif (!defined $name && $ch eq '%') {
      if (substr($line, $i + 1, 1) eq ">") {
        $name = substr($line, 0, $i);
        last;
      }
    }
    elsif (defined $val && $ch ne ')') {
      $val .= $ch;
    }
    elsif (defined $val && $ch eq ')') {
      if (substr($line, $i + 1, 2) eq "%>") {
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
  my($errorString) = "";

  if ($line eq "") {
  }
  elsif ($line =~ /^(\w+)(\(([^\)]+|\".*\")\))?%>/) {
    my($name, $val) = $self->split_name_value($line);

    $length += length($name);
    if (defined $val) {
      $length += length($val) + 2;
    }

    if ($self->is_keyword($name)) {
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
        $self->handle_foreach($val);
      }
      elsif ($name eq 'fornotlast'  || $name eq 'forlast' ||
             $name eq 'fornotfirst' || $name eq 'forfirst') {
        $self->handle_special($name, $self->process_special($val));
      }
      elsif ($name eq 'comment') {
        ## Ignore the contents of the comment
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
    for(my $i = 0; $i < $length; $i++) {
      my($part) = substr($line, $i, 2);
      if ($part eq "%>") {
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
  $prjc->update_project_info($self, 1, ['guid']);
}


sub is_only_keyword {
  my($self)   = shift;
  my($line)   = shift;
  my($status) = 0;

  if ($line =~ /^<%(.*)%>$/) {
    my($part) = $1;
    if ($part !~ /%>/) {
      foreach my $keyword (@keywords) {
        if ($part =~ /^$keyword/) {
          $status = 1;
          last;
        }
      }
    }
  }
  return $status;
}


sub parse_line {
  my($self)        = shift;
  my($ih)          = shift;
  my($line)        = shift;
  my($status)      = 1;
  my($errorString) = "";
  my($length)      = length($line);
  my($name)        = 0;
  my($skipempty)   = $self->{'if_skip'};
  my($crlf)        = $self->{'prjc'}->crlf();
  my($clen)        = length($crlf);
  my($startempty)  = ($line eq "" ? 1 : 0);
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
    $self->{'built'} = "";
  }

  for(my $i = 0; $i < $length; $i++) {
    my($part) = substr($line, $i, 2);
    if ($part eq "<%") {
      $i++;
      $name = 1;
    }
    elsif ($part eq "%>") {
      $i++;
      $name = 0;
      if ($append_name) {
        $append_name = 0;
        if (!$self->{'if_skip'}) {
          $self->append_current($part);
        }
      }
    }
    elsif ($name) {
      my($substr) = substr($line, $i);
      my($efcheck) = ($substr =~ /^endfor\%\>/);
      my($focheck) = ($substr =~ /^foreach\(/);

      if ($focheck && $self->{'foreach'}->{'count'} >= 0) {
        $self->{'foreach'}->{'nested'}++;
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
          $self->append_current("<%" . substr($line, $i, 1));
          $append_name = 1;
        }
      }

      if ($efcheck && $self->{'foreach'}->{'nested'} > 0) {
        $self->{'foreach'}->{'nested'}--;
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
        ($self->{'built'} ne $crlf && $self->{'built'} ne "")) {
      my($lines) = $self->{'lines'};
      push(@$lines, $self->{'built'});
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
    $errorString = "$input: line $linenumber: $errorString\n";
  }

  return $status, $errorString;
}


sub get_lines {
  my($self) = shift;
  return $self->{'lines'};
}


1;
