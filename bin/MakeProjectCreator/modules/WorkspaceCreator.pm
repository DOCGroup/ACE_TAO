package WorkspaceCreator;

# ************************************************************
# Description   : Base class for all workspace creators
# Author        : Chad Elliott
# Create Date   : 5/13/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use Cwd;
use FileHandle;
use File::Path;
use File::Basename;

use Creator;

use vars qw(@ISA);
@ISA = qw(Creator);

# ************************************************************
# Data Section
# ************************************************************

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)     = shift;
  my($global)    = shift;
  my($inc)       = shift;
  my($template)  = shift;
  my($ti)        = shift;
  my($dynamic)   = shift;
  my($static)    = shift;
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, 'workspace');
  my($typecheck) = $self->{'type_check'};

  $self->{'workspace_name'} = undef;
  $self->{$typecheck}       = 0;
  $self->{'projects'}       = [];
  $self->{'project_info'}   = {};
  $self->{'reading_parent'} = [];
  $self->{'project_files'}  = [];
  $self->{'dynamic'}        = $dynamic;
  $self->{'static'}         = $static;

  return $self;
}


sub parse_line {
  my($self)   = shift;
  my($ih)     = shift;
  my($line)   = shift;
  my($status,
     $errorString,
     @values) = $self->parse_known($line);

  ## Was the line recognized?
  if ($status && defined $values[0]) {
    if ($values[0] eq $self->{'grammar_type'}) {
      my($name)      = $values[1];
      my($typecheck) = $self->{'type_check'};
      if (defined $name && $name eq "}") {
        my($rp) = $self->{'reading_parent'};
        if (!defined $$rp[0]) {
          ## Fill in all the default values
          $self->generate_defaults();

          ## End of project; Have subclass write out the file
          $self->write_workspace();

          $self->{'workspace_name'} = undef;
          $self->{'projects'}       = [];
          $self->{'project_info'}   = {};
          $self->{'project_files'}  = [];
        }
        $self->{$typecheck} = 0;
      }
      else {
        ## Project Beginning
        ## Deal with the inheritance hiearchy first
        my($parents) = $values[2];
        if (defined $parents) {
          foreach my $parent (@$parents) {
            ## Read in the parent onto ourself
            my($file) = $self->search_include_path("$parent.mwc");

            if (defined $file) {
              my($rp) = $self->{'reading_parent'};
              push(@$rp, 1);
              $self->parse_file("$parent.mwc");
              pop(@$rp);
              if (!$status) {
                $errorString = "ERROR: Invalid parent: $parent";
              }
            }
            else {
              $status = 0;
              $errorString = "ERROR: Unable to locate parent: $parent";
            }
          }
        }

        ## Set up some initial values
        if (defined $name) {
          $name =~ s/^\(\s*//;
          $name =~ s/\s*\)$//;
          $self->{'workspace_name'} = $name;
        }
        $self->{$typecheck} = 1;
      }
    }
    else {
      $errorString = "ERROR: Unrecognized line: $line";
      $status = 0;
    }
  }
  elsif ($status == -1) {
    my($project_files) = $self->{'project_files'};
    push(@$project_files, $line);
    $status = 1;
  }

  return $status, $errorString;
}


sub search_for_files {
  my($self)  = shift;
  my($files) = shift;
  my($exts)  = shift;
  my($array) = shift;

  foreach my $file (@$files) {
    if (-d $file) {
      my(@f) = $self->generate_default_file_list($file);
      $self->search_for_files(\@f, $exts, $array);
    }
    else {
      foreach my $ext (@$exts) {
        if ($file =~ /$ext$/) {
          unshift(@$array, $file);
          last;
        }
      }
    }
  }
}


sub generate_default_components {
  my($self)  = shift;
  my($files) = shift;
  my($pjf)   = $self->{'project_files'};
  my(@exts)  = ("\\.mpc");
  if (defined $$pjf[0]) {
    ## If we have files, then process directories
    my(@built) = ();
    foreach my $file (@$pjf) {
      if (-d $file) {
        my(@found) = ();
        my(@gen)   = $self->generate_default_file_list($file);
        $self->search_for_files(\@gen, \@exts, \@found);
        push(@built, @found);
      }
      else {
        push(@built, $file);
      }
    }
    $self->{'project_files'} = \@built;
  }
  else {
    ## Add all of the mpc files in this directory
    ## and in the subdirectories.
    $self->search_for_files($files, \@exts, $pjf);

    ## If no files were found, then we push the empty
    ## string, so the Project Creator will generate
    ## the default project file.
    if (!defined $$pjf[0]) {
      push(@$pjf, "");
    }
  }
}


sub generate_defaults {
  my($self) = shift;
  my($base) = $self->base_directory();

  ## Generate default workspace name
  if (!defined $self->{'workspace_name'}) {
    $self->{'workspace_name'} = $base;
  }

  my(@files) = $self->generate_default_file_list();

  ## Generate default components
  $self->generate_default_components(\@files);
}


sub get_workspace_name {
  my($self) = shift;
  return $self->{'workspace_name'};
}


sub write_workspace {
  my($self) = shift;

  ## Generate the project files
  my($status, $generator) = $self->generate_project_files();
  if ($status) {
    my($fh)   = new FileHandle();
    my($name) = $self->workspace_file_name();
    my($dir)  = dirname($name);

    if ($dir ne ".") {
      mkpath($dir, 0, 0777);
    }
    if (open($fh, ">$name")) {
      $self->pre_workspace($fh);
      $self->write_comps($fh, $generator);
      $self->post_workspace($fh);
      close($fh);
    }
    else {
      print STDERR "ERROR: Unable to open $name for output\n";
      $status = 0;
    }
  }
  else {
    print STDERR "ERROR: Unable to generate all of the project files\n";
    $status = 0;
  }

  return $status;
}


sub generate_project_files {
  my($self)       = shift;
  my($status)     = 0;
  my(@projects)   = ();
  my(%pi)         = ();
  my($generator)  = $self->project_creator();
  my($cwd)        = getcwd();

  foreach my $file (@{$self->{'project_files'}}) {
    my($dir)  = dirname($file);
    my($gen)  = [];
    my($sgen) = [];

    ## We must change to the subdirectory for
    ## which this project file is intended
    if (chdir($dir)) {
      $status = $generator->generate(basename($file));

      ## If any one project file fails, then stop
      ## processing altogether.
      if (!$status) {
        return $status;
      }

      ## Get the individual project information and
      ## generated file name(s)
      $gen = $generator->get_files_written();
      chdir($cwd);
    }
    my($gpi) = $generator->get_project_info();
    my($c) = 0;
    foreach my $pj (@$gen) {
      my($full) = ($dir ne "." ? "$dir/" : "") . $pj;
      push(@projects, $full);
      $pi{$full} = $$gpi[$c];
      $c++;
    }
  }

  $self->{'projects'}     = \@projects;
  $self->{'project_info'} = \%pi;

  return $status, $generator;
}


sub get_projects {
  my($self) = shift;
  return $self->{'projects'};
}


sub get_project_info {
  my($self) = shift;
  return $self->{'project_info'};
}


sub get_dynamic {
  my($self) = shift;
  return $self->{'dynamic'};
}


sub get_static {
  my($self) = shift;
  return $self->{'static'};
}


sub sort_dependencies {
  my($self)     = shift;
  my($projects) = shift;
  my($pjs)      = shift;
  my(@list)     = @$projects;
  my(%prepend)  = ();

  foreach my $project (@list) {
    my($dname) = dirname($project);
    if ($dname ne ".") {
      $prepend{basename($project)} = dirname($project);
    }
  }

  ## Put the projects in the order specified
  ## by the project dpendencies.
  for(my $i = 0; $i <= $#list; $i++) {
    my($project) = $list[$i];
    my($pi) = $$pjs{$project};
    my($name, $deps) = @$pi;

    if ($deps ne "") {
      my($darr)  = $self->create_array($deps);
      my($moved) = 0;
      foreach my $dep (@$darr) {
        my($full) = (defined $prepend{$dep} ? "$prepend{$dep}/" : "") . $dep;
        if ($project ne $full) {
          ## See if the dependency is listed after this project
          for(my $j = $i; $j <= $#list; $j++) {
            if ($list[$j] eq $full) {
              ## If so, move it in front of the current project
              splice(@list, $i, 0, $full);
              splice(@list, $j + 1, 1);
              $moved = 1;
              $j--;
            }
          }
        }
      }
      if ($moved) {
        $i--;
      }
    }
  }
  return @list;
}


# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return "";
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
}


sub write_comps {
  my($self) = shift;
  my($fh)   = shift;
  my($gens) = shift;
}


sub post_workspace {
  my($self) = shift;
  my($fh)   = shift;
}


sub project_creator {
  my($self) = shift;
  return undef;
}


1;
