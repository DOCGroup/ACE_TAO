package HTMLProjectCreator;

# ************************************************************
# Description   : An HTML project creator to display all settings
# Author        : Justin Michel & Chad Elliott
# Create Date   : 8/25/2003
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use ProjectCreator;

use vars qw(@ISA);
@ISA = qw(ProjectCreator);

# ************************************************************
# Data Section
# ************************************************************

my($style_indent) = .5;

# ************************************************************
# Subroutine Section
# ************************************************************

sub file_sorter {
  my($self)  = shift;
  my($left)  = shift;
  my($right) = shift;
  return lc($left) cmp lc($right);
}


sub convert_slashes {
  #my($self) = shift;
  return 0;
}


sub label_nodes {
  my($self)  = shift;
  my($hash)  = shift;
  my($nodes) = shift;
  my($level) = shift;

  foreach my $key (sort keys %$hash) {
    push(@$nodes, [$level, $key]);
    $self->label_nodes($$hash{$key}, $nodes, $level + 1);
  }
}


sub count_levels {
  my($self)    = shift;
  my($hash)    = shift;
  my($current) = shift;
  my($count)   = shift;

  foreach my $key (keys %$hash) {
    $self->count_levels($$hash{$key}, $current + 1, $count);
  }
  if ($current > $$count) {
    $$count = $current;
  }
}


sub fill_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = undef;

  if ($name eq 'inheritance_nodes') {
    ## Get the nodes with numeric labels for the level
    my(@nodes) = ();
    $self->label_nodes($self->get_inheritance_tree(), \@nodes, 0);

    ## Push each node onto the value array
    $value = [];
    for(my $i = 0; $i <= $#nodes; ++$i) {
      my($file) = $nodes[$i]->[1];
      my($dir)  = dirname($file);
      my($base) = basename($file);

      ## Relative paths do not work at all in a web browser
      if ($dir eq '.') {
        $file = $base;
      }

      my($path) = ($base eq $file ? $self->getcwd() . '/' : '');
      my($name) = undef;

      if ($i == 0) {
        ## If this is the first node, then replace the base filename
        ## with the actual project name.
        $name = $self->project_name();
      }
      else {
        ## This is a base project, so we use the basename and
        ## remove the file extension.
        $name = $base;
        $name =~ s/\.[^\.]+$//;
      }

      ## Create the div and a tags.
      push(@$value, '<a href="file://' . $path . $file .
                    '" onClick="return popup(this, \'Project File\')" ' .
                    'target=_blank>' .
                    '<div class="tree' . $nodes[$i]->[0] . '">' .
                     $name . '</div></a>');
    }
  }
  elsif ($name eq 'tree_styles') {
    ## Count the number of levels deep the inheritance goes
    my($count) = 0;
    $self->count_levels($self->get_inheritance_tree(), 0, \$count);

    my($margin) = 0;
    my($start)  = 100;
    my($max)    = 255;
    my($inc)    = ($count ne 0 ? int(($max - $start) / $count) : $max);

    ## Push each tree style onto the value array
    $value = [];
    for(my $i = 0; $i < $count; ++$i) {
      push(@$value, ".tree$i { background-color: #" .
                    sprintf("%02x%02x%02x", 0, $start, $start) . ';' .
                    ($margin != 0 ? " margin-left: $margin" . 'in;' : '') .
                    ' }');
      $start += $inc;
      $margin += $style_indent;
    }
  }

  return $value;
}


sub project_file_name {
  my($self) = shift;
  my($name) = shift;

  if (!defined $name) {
    $name = $self->project_name();
  }

  return "$name.html";
}


sub get_template {
  #my($self) = shift;
  return 'html';
}


1;
