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
use FileHandle;
use File::Path;
use File::Basename;

use Creator;

use vars qw(@ISA);
@ISA = qw(Creator);

# ************************************************************
# Data Section
# ************************************************************

my($wsext) = 'mwc';

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
  my($relative)  = shift;
  my($addtemp)   = shift;
  my($addproj)   = shift;
  my($progress)  = shift;
  my($toplevel)  = shift;
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, $relative,
                                $addtemp, $addproj,
                                $progress, $toplevel, 'workspace');
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
      if (defined $name && $name eq '}') {
        my($rp) = $self->{'reading_parent'};
        if (!defined $$rp[0]) {
          ## Fill in all the default values
          $self->generate_defaults();

          ## End of project; Have subclass write out the file
          ## Generate the project files
          my($gstat, $generator) = $self->generate_project_files();
          if ($gstat) {
            $self->write_workspace($generator, 1);
          }
          else {
            $errorString = 'ERROR: Unable to ' .
                           'generate all of the project files';
            $status = 0;
          }

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
            my($file) = $self->search_include_path("$parent.$wsext");

            if (defined $file) {
              my($rp) = $self->{'reading_parent'};
              push(@$rp, 1);
              $self->parse_file("$parent.$wsext");
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
  my(@exts)  = ('\\.mpc');
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
      push(@$pjf, '');
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
  my($self)      = shift;
  my($generator) = shift;
  my($addfile)   = shift;
  my($status)    = 1;

  if ($self->get_toplevel()) {
    my($fh)   = new FileHandle();
    my($name) = $self->transform_file_name($self->workspace_file_name());
    my($dir)  = dirname($name);

    if ($dir ne '.') {
      mkpath($dir, 0, 0777);
    }
    if (open($fh, ">$name")) {
      $self->pre_workspace($fh);
      $self->write_comps($fh, $generator);
      $self->post_workspace($fh);
      close($fh);

      if ($addfile) {
        $self->add_file_written($name);
      }
    }
    else {
      print STDERR "ERROR: Unable to open $name for output\n";
      $status = 0;
    }
  }

  return $status;
}


sub save_project_info {
  my($self)     = shift;
  my($gen)      = shift;
  my($gpi)      = shift;
  my($dir)      = shift;
  my($projects) = shift;
  my($pi)       = shift;
  my($c)        = 0;
  foreach my $pj (@$gen) {
    my($full) = ($dir ne '.' ? "$dir/" : '') . $pj;
    push(@$projects, $full);
    $$pi{$full} = $$gpi[$c];
    $c++;
  }
}


sub generate_project_files {
  my($self)       = shift;
  my($status)     = 0;
  my(@projects)   = ();
  my(%pi)         = ();
  my($generator)  = $self->project_creator();
  my($cwd)        = $self->getcwd();

  foreach my $file (@{$self->{'project_files'}}) {
    my($dir)  = dirname($file);

    ## We must change to the subdirectory for
    ## which this project file is intended
    if ($self->cd($dir)) {
      $status = $generator->generate(basename($file));

      ## If any one project file fails, then stop
      ## processing altogether.
      if (!$status) {
        return $status, $generator;
      }

      ## Get the individual project information and
      ## generated file name(s)
      my($gen) = $generator->get_files_written();
      my($gpi) = $generator->get_project_info();

      ## If we need to generate a workspace file per project
      ## then we generate a temporary project info and projects
      ## array and call write_project().
      if ($dir ne '.' && $self->workspace_per_project()) {
        my(%perpi)       = ();
        my(@perprojects) = ();
        $self->save_project_info($gen, $gpi, '.', \@perprojects, \%perpi);

        ## Set our per project information
        $self->{'projects'}     = \@perprojects;
        $self->{'project_info'} = \%perpi;

        ## Write our per project workspace
        $self->write_workspace($generator);

        ## Reset our project information to empty
        $self->{'projects'}     = [];
        $self->{'project_info'} = {};
      }
      $self->cd($cwd);
      $self->save_project_info($gen, $gpi, $dir, \@projects, \%pi);
    }
    else {
      ## Unable to change to the directory
      return 0, $generator;
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
    if ($dname ne '.') {
      $prepend{basename($project)} = dirname($project);
    }
  }

  ## Put the projects in the order specified
  ## by the project dpendencies.
  for(my $i = 0; $i <= $#list; $i++) {
    my($project) = $list[$i];
    my($pi) = $$pjs{$project};
    my($name, $deps) = @$pi;

    if ($deps ne '') {
      my($darr)  = $self->create_array($deps);
      my($moved) = 0;
      foreach my $dep (@$darr) {
        my($base) = basename($dep);
        my($full) = (defined $prepend{$base} ?
                       "$prepend{$base}/" : '') . $base;
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


sub project_creator {
  my($self) = shift;
  my($str)  = "$self";

  ## NOTE: If the subclassed WorkspaceCreator name prefix does not
  ##       match the name prefix of the ProjectCreator, this code
  ##       will not work and the subclassed WorkspaceCreator will
  ##       need to override this method.

  $str =~ s/Workspace/Project/;
  $str =~ s/=HASH.*//;

  ## For the toplevel parameter, we always pass 1 since the workspace
  ## creator always wants the ProjectCreator to generate projects.
  return $str->new($self->get_global_cfg(),
                   $self->get_include_path(),
                   $self->get_template_override(),
                   $self->get_ti_override(),
                   $self->get_dynamic(),
                   $self->get_static(),
                   $self->get_relative(),
                   $self->get_addtemp(),
                   $self->get_addproj(),
                   $self->get_progress_callback(),
                   1);
}


sub sort_files {
  #my($self) = shift;
  return 0;
}


sub get_modified_workspace_name {
  my($self) = shift;
  my($name) = shift;
  my($ext)  = shift;

  if (!defined $self->{'previous_workspace_name'}) {
    $self->{'previous_workspace_name'} = $self->get_workspace_name();
  }
  elsif ($self->{'previous_workspace_name'} ne $self->get_workspace_name()) {
    $self->{'previous_workspace_name'} = $self->get_workspace_name();        
    $self->{'current_workspace_name'} =
                          "$name.$self->{'previous_workspace_name'}$ext";
  }

  return (defined $self->{'current_workspace_name'} ?
                  $self->{'current_workspace_name'} : "$name$ext");
}


sub generate_recursive_input_list {
  my($self) = shift;
  my($dir)  = shift;
  return $self->extension_recursive_input_list($dir, $wsext);
}

# ************************************************************
# Virtual Methods To Be Overridden
# ************************************************************

sub workspace_file_name {
  #my($self) = shift;
  return '';
}


sub workspace_per_project {
  #my($self) = shift;
  return 0;
}


sub pre_workspace {
  #my($self) = shift;
  #my($fh)   = shift;
}


sub write_comps {
  #my($self) = shift;
  #my($fh)   = shift;
  #my($gens) = shift;
}


sub post_workspace {
  #my($self) = shift;
  #my($fh)   = shift;
}


1;
