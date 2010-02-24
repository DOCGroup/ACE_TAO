package GHSWorkspaceCreator;

# ************************************************************
# Description   : A GHS Workspace creator for version 4.x
# Author        : Chad Elliott
# Create Date   : 7/3/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use GHSProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my %directives = ('I'          => 1,
                  'L'          => 1,
                  'D'          => 1,
                  'l'          => 1,
                  'G'          => 1,
                  'non_shared' => 1,
                  'bsp'        => 1,
                  'os_dir'     => 1,
                 );
my $tgt;
my $integrity  = '[INTEGRITY Application]';
my @integ_bsps;

# ************************************************************
# Subroutine Section
# ************************************************************

sub compare_output {
  #my $self = shift;
  return 1;
}


sub workspace_file_name {
  return $_[0]->get_modified_workspace_name('default', '.gpj');
}


sub pre_workspace {
  my($self, $fh) = @_;
  my $crlf = $self->crlf();
  my $prjs = $self->get_projects();

  ## Take the primaryTarget from the first project in the list
  if (defined $$prjs[0]) {
    my $fh      = new FileHandle();
    my $outdir  = $self->get_outdir();
    if (open($fh, "$outdir/$$prjs[0]")) {
      while(<$fh>) {
        if (/^#primaryTarget=(.+)$/) {
          $tgt = $1;
          last;
        }
      }
      close($fh);
    }
  }

  ## Print out the preliminary information
  print $fh "#!gbuild$crlf",
            "primaryTarget=$tgt$crlf",
            "[Project]$crlf",
            "\t--one_instantiation_per_object$crlf",
            "\t:sourceDir=.$crlf",
            "\t--std$crlf",
            "\t-language=cxx$crlf",
            "\t--long_long$crlf",
            "\t--new_style_casts$crlf";
}


sub create_integrity_project {
  my($self, $int_proj, $project, $type, $target) = @_;
  my $outdir   = $self->get_outdir();
  my $crlf     = $self->crlf();
  my $fh       = new FileHandle();
  my $int_file = $int_proj;
  $int_file =~ s/\.gpj$/.int/;

  if (open($fh, ">$outdir/$int_proj")) {
    ## First print out the project file
    print $fh "#!gbuild$crlf",
              "\t$integrity$crlf",
              "$project\t\t$type$crlf",
              "$int_file$crlf";
    foreach my $bsp (@integ_bsps) {
      print $fh "$bsp$crlf";
    }
    close($fh);

    ## Next create the integration file
    if (open($fh, ">$outdir/$int_file")) {
      print $fh "Kernel$crlf",
                "\tFilename\t\t\tDynamicDownload$crlf",
                "EndKernel$crlf$crlf",
                "AddressSpace$crlf",
                "\tFilename\t\t\t$target$crlf",
                "\tLanguage\t\t\tC++$crlf",
                "\tLibrary\t\t\t\tlibINTEGRITY.so$crlf",
                "\tLibrary\t\t\t\tlibc.so$crlf",
                "\tLibrary\t\t\t\tlibscxx_e.so$crlf",
                "\tTask Initial$crlf",
                "\t\tStackLength\t\t0x8000$crlf",
                "\tEndTask$crlf",
                "EndAddressSpace$crlf";
      close($fh);
    }
  }
}


sub mix_settings {
  my($self, $project) = @_;
  my $rh     = new FileHandle();
  my $mix    = $project;
  my $outdir = $self->get_outdir();

  ## Things that seem like they should be set in the project
  ## actually have to be set in the controlling project file.
  if (open($rh, "$outdir/$project")) {
    my $crlf = $self->crlf();
    my $integrity_project = (index($tgt, 'integrity') >= 0);
    my($int_proj, $int_type, $target);

    while(<$rh>) {
      if (/^\s*(\[(Program|Library|Subproject)\])\s*$/) {
        my $type = $1;
        if ($integrity_project && $type eq '[Program]') {
          $int_proj = $project;
          $int_proj =~ s/(\.gpj)$/_int$1/;
          $int_type = $type;
          $mix =~ s/(\.gpj)$/_int$1/;
          $type = $integrity;
        }
        $mix .= "\t\t$type$crlf" .
                "\t-object_dir=" . $self->mpc_dirname($project) .
                '/.obj' . $crlf;
      }
      elsif (/^\s*(\[Shared Object\])\s*$/) {
        $mix .= "\t\t$1$crlf" .
                "\t-pic$crlf" .
                "\t-object_dir=" . $self->mpc_dirname($project) .
                '/.shobj' . $crlf;
      }
      elsif ($integrity_project && /^(.*\.bsp)\s/) {
        push(@integ_bsps, $1);
      }
      else {
        if (/^\s*\-((\w)\w*)/) {
          ## Save the required options into the mixed project string
          if (defined $directives{$2} || defined $directives{$1}) {
            $mix .= $_;
          }

          ## If this is an integrity project, we need to find out
          ## what the output file will be for the integrate file.
          if (defined $int_proj && /^\s*\-o\s+(.*)\s$/) {
            $target = $1;
          }
        }
      }
    }
    if (defined $int_proj) {
      $self->create_integrity_project($int_proj, $project,
                                      $int_type, $target);
    }
    close($rh);
  }

  return $mix;
}


sub write_comps {
  my($self, $fh) = @_;

  ## Print out each projet
  foreach my $project ($self->sort_dependencies($self->get_projects(), 0)) {
    print $fh $self->mix_settings($project);
  }
}



1;
