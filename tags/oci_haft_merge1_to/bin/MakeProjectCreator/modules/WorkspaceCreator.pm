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
use File::Compare;
use File::Basename;

use Creator;
use Options;

use vars qw(@ISA);
@ISA = qw(Creator Options);

# ************************************************************
# Data Section
# ************************************************************

my($wsext)  = 'mwc';
my($wsbase) = 'mwb';

## Valid names for assignments within a workspace
my(%validNames) = ('cmdline'  => 1,
                   'implicit' => 1,
                  );

## Singleton hash maps of project information
my(%allprinfo)   = ();
my(%allprojects) = ();

## Global previous workspace names
my(%previous_workspace_name) = ();

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
  my($baseprojs) = shift;
  my($gfeature)  = shift;
  my($feature)   = shift;
  my($self)      = Creator::new($class, $global, $inc,
                                $template, $ti, $dynamic, $static,
                                $relative, $addtemp, $addproj,
                                $progress, $toplevel, $baseprojs,
                                $feature, 'workspace');
  my($typecheck) = $self->{'type_check'};

  $self->{'workspace_name'}      = undef;
  $self->{$typecheck}            = 0;
  $self->{'projects'}            = [];
  $self->{'project_info'}        = {};
  $self->{'reading_parent'}      = [];
  $self->{'project_files'}       = [];
  $self->{'scoped_assign'}       = {};
  $self->{'cacheok'}             = 1;
  $self->{'exclude'}             = {};
  $self->{'wctype'}              = $self->extractType("$self");
  $self->{'modified_count'}      = 0;
  $self->{'global_feature_file'} = $gfeature;

  ## Add a hash reference for our workspace type
  if (!defined $previous_workspace_name{$self->{'wctype'}}) {
    $previous_workspace_name{$self->{'wctype'}} = {};
  }

  return $self;
}


sub modify_assignment_value {
  my($self)  = shift;
  my($value) = shift;

  ## Workspace assignments do not need modification.
  return $value;
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

          ## End of workspace; Have subclass write out the file
          ## Generate the project files
          my($gstat, $generator) = $self->generate_project_files();
          if ($gstat) {
            ($status, $errorString) = $self->write_workspace($generator, 1);
            $self->{'assign'} = {};
          }
          else {
            $errorString = 'ERROR: Unable to ' .
                           'generate all of the project files';
            $status = 0;
          }

          $self->{'modified_count'} = 0;
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
            my($file) = $self->search_include_path("$parent.$wsbase");
            if (!defined $file) {
              $file = $self->search_include_path("$parent.$wsext");
            }

            if (defined $file) {
              my($rp) = $self->{'reading_parent'};
              push(@$rp, 1);
              $status = $self->parse_file($file);
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
          if ($name =~ /[\/\\]/) {
            $status = 0;
            $errorString = 'ERROR: Workspaces can not have a slash ' .
                           'or a back slash in the name';
          }
          else {
            $name =~ s/^\(\s*//;
            $name =~ s/\s*\)$//;

            ## Replace any *'s with the default name
            my($def) = $self->get_default_workspace_name();
            $name = $self->fill_type_name($name, $def);

            $self->{'workspace_name'} = $name;
          }
        }
        $self->{$typecheck} = 1;
      }
    }
    elsif ($values[0] eq 'assignment') {
      if (defined $validNames{$values[1]}) {
        $self->process_assignment($values[1], $values[2]);
      }
      else {
        $errorString = "ERROR: Invalid assignment name: $values[1]";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'assign_add') {
      if (defined $validNames{$values[1]}) {
        $self->process_assignment_add($values[1], $values[2]);
      }
      else {
        $errorString = "ERROR: Invalid addition name: $values[1]";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'assign_sub') {
      if (defined $validNames{$values[1]}) {
        $self->process_assignment_sub($values[1], $values[2]);
      }
      else {
        $errorString = "ERROR: Invalid subtraction name: $values[1]";
        $status = 0;
      }
    }
    elsif ($values[0] eq 'component') {
      if ($values[1] eq 'exclude') {
        ($status, $errorString) = $self->parse_exclude($ih,
                                                       $values[2]);
      }
      else {
        ($status, $errorString) = $self->parse_scope($ih,
                                                     $values[1],
                                                     $values[2],
                                                     \%validNames);
      }
    }
    else {
      $errorString = "ERROR: Unrecognized line: $line";
      $status = 0;
    }
  }
  elsif ($status == -1) {
    push(@{$self->{'project_files'}}, $line);
    $status = 1;
  }

  return $status, $errorString;
}


sub parse_exclude {
  my($self)        = shift;
  my($fh)          = shift;
  my($typestr)     = shift;
  my($status)      = 0;
  my($errorString) = 'ERROR: Unable to process exclude';

  if ($typestr eq $self->get_default_component_name()) {
    $typestr = $self->{'wctype'};
  }

  my(@types)   = split(/\s*,\s*/, $typestr);
  my(@exclude) = ();

  while($_ = $fh->getline()) {
    my($line) = $self->strip_line($_);

    if ($line eq '') {
    }
    elsif ($line =~ /^}/) {
      $status = 1;
      $errorString = '';
      last;
    }
    else {
      push(@exclude, $line);
    }
  }

  foreach my $type (@types) {
    if (!defined $self->{'exclude'}->{$type}) {
      $self->{'exclude'}->{$type} = [];
    }
    push(@{$self->{'exclude'}->{$type}}, @exclude);
  }

  return $status, $errorString;
}


sub excluded {
  my($self) = shift;
  my($file) = shift;

  foreach my $excluded (@{$self->{'exclude'}->{$self->{'wctype'}}}) {
    if ($excluded eq $file || $file =~ /$excluded\//) {
      return 1;
    }
  }

  return 0;
}


sub handle_scoped_unknown {
  my($self)  = shift;
  my($fh)    = shift;
  my($type)  = shift;
  my($flags) = shift;
  my($line)  = shift;

  if (-d $line) {
    my(@files) = ();
    $self->search_for_files([ $line ], \@files, $$flags{'implicit'});
    foreach my $file (@files) {
      $self->{'scoped_assign'}->{$file} = $flags;
      push(@{$self->{'project_files'}}, $file);
    }
  }
  else {
    $self->{'scoped_assign'}->{$line} = $flags;
    push(@{$self->{'project_files'}}, $line);
  }

  return 1, '';
}


sub search_for_files {
  my($self)  = shift;
  my($files) = shift;
  my($array) = shift;
  my($impl)  = shift;

  foreach my $file (@$files) {
    if (-d $file) {
      my(@f) = $self->generate_default_file_list($file);
      $self->search_for_files(\@f, $array, $impl);
      if ($impl) {
        unshift(@$array, $file);
      }
    }
    else {
      if ($file =~ /\.mpc$/) {
        unshift(@$array, $file);
      }
    }
  }
}


sub remove_duplicate_projects {
  my($self)  = shift;
  my($list)  = shift;
  my($count) = scalar(@$list);

  for(my $i = 0; $i < $count; ++$i) {
    my($file) = $$list[$i];
    foreach my $inner (@$list) {
      if ($file ne $inner && $file eq dirname($inner) && ! -d $inner) {
        splice(@$list, $i, 1);
        --$count;
        --$i;
        last;
      }
    }
  }
}


sub generate_default_components {
  my($self)  = shift;
  my($files) = shift;
  my($impl)  = shift;
  my($pjf)   = $self->{'project_files'};

  if (defined $$pjf[0]) {
    ## If we have files, then process directories
    my(@built) = ();
    foreach my $file (@$pjf) {
      if (!$self->excluded($file)) {
        if (-d $file) {
          my(@found) = ();
          my(@gen)   = $self->generate_default_file_list($file);
          $self->search_for_files(\@gen, \@found, $impl);
          push(@built, @found);
          if ($impl || $self->{'scoped_assign'}->{$file}->{'implicit'}) {
            push(@built, $file);
          }
        }
        else {
          push(@built, $file);
        }
      }
    }

    ## If the workspace is set to implicit
    if ($impl) {
      ## Remove duplicates from this list
      $self->remove_duplicate_projects(\@built);
    }

    ## Set the project files
    $self->{'project_files'} = \@built;
  }
  else {
    ## Add all of the wanted files in this directory
    ## and in the subdirectories.
    $self->search_for_files($files, $pjf, $impl);

    ## If the workspace is set to implicit
    if ($impl) {
      ## Remove duplicates from this list
      $self->remove_duplicate_projects($pjf);
    }

    ## If no files were found, then we push the empty
    ## string, so the Project Creator will generate
    ## the default project file.
    if (!defined $$pjf[0]) {
      push(@$pjf, '');
    }
  }
}


sub get_default_workspace_name {
  my($self) = shift;
  my($name) = $self->get_current_input();

  if ($name eq '') {
    $name = $self->base_directory();
  }
  else {
    ## Since files on UNIX can have back slashes, we transform them
    ## into underscores.
    $name =~ s/\\/_/g;

    ## Take off the extension
    $name =~ s/\.[^\.]+$//;
  }

  return $name;
}


sub generate_defaults {
  my($self) = shift;

  ## Generate default workspace name
  if (!defined $self->{'workspace_name'}) {
    $self->{'workspace_name'} = $self->get_default_workspace_name();
  }

  my(@files) = $self->generate_default_file_list();

  ## Generate default components
  $self->generate_default_components(\@files,
                                     $self->get_assignment('implicit'));
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
  my($error)     = '';

  if ($self->get_toplevel()) {
    if (!$addfile) {
      $self->{'per_project_workspace_name'} = 1;
    }
    my($name) = $self->transform_file_name($self->workspace_file_name());
    if (defined $self->{'projects'}->[0]) {
      my($fh)   = new FileHandle();
      my($dir)  = dirname($name);

      ## Verify and possibly modify the dependencies
      if ($addfile) {
        $self->verify_build_ordering();
      }

      if ($dir ne '.') {
        mkpath($dir, 0, 0777);
      }

      ## First write the output to a temporary file
      my($tmp) = "MWC$>.$$";
      my($different) = 1;
      if (open($fh, ">$tmp")) {
        $self->pre_workspace($fh);
        $self->write_comps($fh, $generator);
        $self->post_workspace($fh);
        close($fh);

        if (-r $name &&
            -s $tmp == -s $name && compare($tmp, $name) == 0) {
          $different = 0;
        }
      }
      else {
        $error = "ERROR: Unable to open $tmp for output.";
        $status = 0;
      }

      if ($status) {
        if ($different) {
          unlink($name);
          if (rename($tmp, $name)) {
            if ($addfile) {
              $self->add_file_written($name);
            }
          }
          else {
            $error = 'ERROR: Unable to open ' . $self->getcwd() .
                     "/$name for output";
            $status = 0;
          }
        }
        else {
          ## We will pretend that we wrote the file
          unlink($tmp);
          if ($addfile) {
            $self->add_file_written($name);
          }
        }
      }
    }
    else {
      print "WARNING: No projects were created.\n" .
            "         Workspace $name has not been created.\n";
    }
    if (!$addfile) {
      $self->{'per_project_workspace_name'} = undef;
    }
  }

  return $status, $error;
}


sub save_project_info {
  my($self)     = shift;
  my($gen)      = shift;
  my($gpi)      = shift;
  my($dir)      = shift;
  my($projects) = shift;
  my($pi)       = shift;
  my($c)        = 0;

  ## For each file written
  foreach my $pj (@$gen) {
    ## Save the full path to the project file in the array
    my($full) = ($dir ne '.' ? "$dir/" : '') . $pj;
    push(@$projects, $full);

    ## Get the corresponding generated project info and save it
    ## in the hash map keyed on the full project file name
    $$pi{$full} = $$gpi[$c];
    $c++;
  }
}


sub generate_project_files {
  my($self)      = shift;
  my($status)    = (scalar @{$self->{'project_files'}} == 0 ? 1 : 0);
  my(@projects)  = ();
  my(%pi)        = ();
  my($generator) = $self->project_creator();
  my($cwd)       = $self->getcwd();
  my($impl)      = $self->get_assignment('implicit');
  my($postkey)   = $generator->get_dynamic() .
                   $generator->get_static() . "-$self";
  my($previmpl)  = $impl;
  my($prevcache) = $self->{'cacheok'};
  my(%gstate)    = $generator->save_state();

  ## Remove the address portion of the $self string
  $postkey =~ s/=.*//;

  foreach my $ofile (@{$self->{'project_files'}}) {
    if (!$self->excluded($ofile)) {
      my($file)    = $ofile;
      my($dir)     = dirname($file);
      my($restore) = 0;

      if (defined $self->{'scoped_assign'}->{$ofile}) {
        ## Handle the implicit assignment
        my($oi) = $self->{'scoped_assign'}->{$ofile}->{'implicit'};
        if (defined $oi) {
          $previmpl = $impl;
          $impl     = $oi;
        }

        ## Handle the cmdline assignment
        my($cmdline) = $self->{'scoped_assign'}->{$ofile}->{'cmdline'};
        if (defined $cmdline && $cmdline ne '') {
          ## Save the cacheok value
          $prevcache = $self->{'cacheok'};

          ## Get the current parameters and process the command line
          my(%parameters) = $self->current_parameters();
          $self->process_cmdline($cmdline, \%parameters);

          ## Set the parameters on the generator
          $generator->restore_state(\%parameters);
          $restore = 1;
        }
      }

      ## If we are generating implicit projects and the file is a
      ## directory, then we set the dir to the file and empty the file
      if ($impl && -d $file) {
        $dir  = $file;
        $file = '';
      }

      ## Generate the key for this project file
      my($prkey) = $self->getcwd() . '/' .
                   ($file eq '' ? $dir : $file) . "-$postkey";

      ## We must change to the subdirectory for
      ## which this project file is intended
      if ($self->cd($dir)) {
        my($gen) = [];
        my($gpi) = [];
        if ($self->{'cacheok'} && defined $allprojects{$prkey}) {
          $gen = $allprojects{$prkey};
          $gpi = $allprinfo{$prkey};
          $status = 1;
        }
        else {
          $status = $generator->generate(basename($file));

          ## If any one project file fails, then stop
          ## processing altogether.
          if (!$status) {
            ## We don't restore the state before we leave,
            ## but that's ok since we will be exiting soon.
            return $status, $generator;
          }

          ## Get the individual project information and
          ## generated file name(s)
          $gen = $generator->get_files_written();
          $gpi = $generator->get_project_info();

          ## If we need to generate a workspace file per project
          ## then we generate a temporary project info and projects
          ## array and call write_project().
          if ($dir ne '.' && defined $$gen[0] && $self->workspace_per_project()) {
            my(%perpi)       = ();
            my(@perprojects) = ();
            $self->save_project_info($gen, $gpi, '.', \@perprojects, \%perpi);

            ## Set our per project information
            $self->{'projects'}     = \@perprojects;
            $self->{'project_info'} = \%perpi;

            ## Write our per project workspace
            my($error) = '';
            ($status, $error) = $self->write_workspace($generator);
            if (!$status) {
              print STDERR "$error\n";
            }

            ## Reset our project information to empty
            $self->{'projects'}     = [];
            $self->{'project_info'} = {};
          }

          if ($self->{'cacheok'}) {
            $allprojects{$prkey} = $gen;
            $allprinfo{$prkey}   = $gpi;
          }
        }
        $self->cd($cwd);
        $self->save_project_info($gen, $gpi, $dir, \@projects, \%pi);
      }
      else {
        ## Unable to change to the directory.
        ## We don't restore the state before we leave,
        ## but that's ok since we will be exiting soon.
        return 0, $generator;
      }

      ## Return things to the way they were
      if (defined $self->{'scoped_assign'}->{$ofile}) {
        $impl = $previmpl;

        if ($restore) {
          $self->{'cacheok'} = $prevcache;
          $generator->restore_state(\%gstate);
        }
      }
    }
    else {
      ## This one was excluded, so status is ok
      $status = 1;
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
    my($name) = $$pjs{$project}->[0];
    my($deps) = $self->get_validated_ordering($project);

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
            if ($list[$j] eq $full && $i != $j) {
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


sub optionError {
  my($self) = shift;
  my($str)  = shift;
  print 'WARNING: ' . $self->get_current_input() . ": $str\n";
}


sub process_cmdline {
  my($self)       = shift;
  my($cmdline)    = shift;
  my($parameters) = shift;

  ## It's ok to use the cache
  $self->{'cacheok'} = 1;

  if (defined $cmdline && $cmdline ne '') {
    my($args) = $self->create_array($cmdline);

    ## Look for environment variables
    foreach my $arg (@$args) {
      while($arg =~ /\$(\w+)/) {
        my($name) = $1;
        my($val)  = '';
        if ($name eq 'PWD') {
          $val = $self->getcwd();
        }
        elsif (defined $ENV{$name}) {
          $val = $ENV{$name};
        }
        $arg =~ s/\$\w+/$val/;
      }
    }

    my($options) = $self->options('MWC', {}, 0, @$args);
    if (defined $options) {
      foreach my $key (keys %$options) {
        my($type) = $self->is_set($key, $options);

        if (!defined $type) {
          ## This option was not used, so we ignore it
        }
        elsif ($type eq 'ARRAY') {
          push(@{$parameters->{$key}}, @{$options->{$key}});
        }
        elsif ($type eq 'HASH') {
          foreach my $hk (keys %{$options->{$key}}) {
            $parameters->{$key}->{$hk} = $options->{$key}->{$hk};
          }
        }
        elsif ($type eq 'SCALAR') {
          $parameters->{$key} = $options->{$key};
        }
      }

      ## Issue warnings for these options
      if (defined $options->{'recurse'}) {
        $self->optionError('-recurse is ignored');
      }
      if (defined $options->{'reldefs'}) {
        $self->optionError('-noreldefs is ignored');
      }
      if (defined $options->{'input'}->[0]) {
        $self->optionError('Command line files ' .
                           'specified in a workspace are ignored');
      }

      ## Determine if it's ok to use the cache
      my(@cacheInvalidating) = ('global', 'include', 'baseprojs',
                                'template', 'ti', 'relative',
                                'addtemp', 'addproj');
      foreach my $key (@cacheInvalidating) {
        if ($self->is_set($key, $options)) {
          $self->{'cacheok'} = 0;
          last;
        }
      }
    }
  }
}


sub current_parameters {
  my($self) = shift;
  my(%parameters) = $self->save_state();

  ## We always want the project creator to generate a toplevel
  $parameters{'toplevel'} = 1;
  return %parameters;
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

  ## Set up values for each project creator
  ## If we have command line arguments in the workspace, then
  ## we process them before creating the project creator
  my($cmdline)    = $self->get_assignment('cmdline');
  my(%parameters) = $self->current_parameters();
  $self->process_cmdline($cmdline, \%parameters);

  ## Create the new project creator with the updated parameters
  return $str->new($parameters{'global'},
                   $parameters{'include'},
                   $parameters{'template'},
                   $parameters{'ti'},
                   $parameters{'dynamic'},
                   $parameters{'static'},
                   $parameters{'relative'},
                   $parameters{'addtemp'},
                   $parameters{'addproj'},
                   $parameters{'progress'},
                   $parameters{'toplevel'},
                   $parameters{'baseprojs'},
                   $self->{'global_feature_file'},
                   $parameters{'feature_file'});
}


sub sort_files {
  #my($self) = shift;
  return 0;
}


sub get_modified_workspace_name {
  my($self)   = shift;
  my($name)   = shift;
  my($ext)    = shift;
  my($pwd)    = $self->getcwd();
  my($type)   = $self->{'wctype'};
  my($wsname) = $self->get_workspace_name();

  ## If this is a per project workspace, then we should not
  ## modify the workspace name.  It may overwrite another workspace
  ## but that's ok, it will also be a per project workspace.
  if ($self->{'per_project_workspace_name'}) {
    return "$name$ext";
  }

  if (!defined $previous_workspace_name{$type}->{$pwd}) {
    $previous_workspace_name{$type}->{$pwd} = $wsname;
    $self->{'current_workspace_name'} = undef;
  }
  else {
    my($prefix) = ($name eq $wsname ? $name : "$name.$wsname");
    $previous_workspace_name{$type}->{$pwd} = $wsname;
    while($self->file_written("$prefix" .
                              ($self->{'modified_count'} > 0 ?
                                   ".$self->{'modified_count'}" : '') .
                              "$ext")) {
      ++$self->{'modified_count'};
    }
    $self->{'current_workspace_name'} =
               "$prefix" . ($self->{'modified_count'} > 0 ?
                                ".$self->{'modified_count'}" : '') . "$ext";
  }

  return (defined $self->{'current_workspace_name'} ?
                  $self->{'current_workspace_name'} : "$name$ext");
}


sub generate_recursive_input_list {
  my($self) = shift;
  my($dir)  = shift;
  return $self->extension_recursive_input_list($dir, $wsext);
}


sub verify_build_ordering {
  my($self)     = shift;
  my($projects) = $self->get_projects();

  foreach my $project (@$projects) {
    $self->get_validated_ordering($project);
  }
}


sub get_validated_ordering {
  my($self)     = shift;
  my($project)  = shift;
  my($pjs)      = $self->get_project_info();

  my($name, $deps) = @{$$pjs{$project}};
  if (defined $deps && $deps ne '') {
    my($darr)     = $self->create_array($deps);
    my($projects) = $self->get_projects();
    foreach my $dep (@$darr) {
      my($found) = 0;
      ## Avoid cirular dependencies
      if ($dep ne $name && $dep ne basename($project)) {
        foreach my $p (@$projects) {
          if ($dep eq $$pjs{$p}->[0] || $dep eq basename($p)) {
            $found = 1;
            last;
          }
        }
        if (!$found) {
          if (defined $ENV{MPC_VERBOSE_ORDERING}) {
            print "WARNING: '$name' references '$dep' which has " .
                  "not been processed\n";
          }
          $deps =~ s/\s*"$dep"\s*/ /g;
        }
      }
    }
  }

  $deps =~ s/^\s+//;
  $deps =~ s/\s+$//;

  return $deps;
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
