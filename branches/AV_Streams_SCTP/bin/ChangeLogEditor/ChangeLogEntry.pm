package ChangeLogEntry;

# ************************************************************
# Description   : Create a ChangeLog entry based on modified files.
# Author        : Chad Elliott
# Create Date   : 6/18/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use FileLocator;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class) = shift;
  my($name)  = shift;
  my($email) = shift;
  my($self)  = bless {'name'  => $name,
                      'email' => $email,
                     }, $class;
  return $self;
}


sub sortFileList {
  my($self) = shift;
  return sort {
           if ($a =~ /\.h$/) {
             my($base) = $a;
             $base =~ s/\.h//;
             if ($b =~ /^$base\./) {
               return -1;
             }
             else {
               return $a cmp $b;
             }
           }
           elsif ($a =~ /\.i(nl)?$/) {
             my($base) = $a;
             $base =~ s/\.i(nl)?$//;
             if ($b =~ /^$base\.cpp/) {
               return -1;
             }
             else {
               return $a cmp $b;
             }
           }
           elsif ($a =~ /\.cpp?$/) {
             my($base) = $a;
             $base =~ s/\.cpp?$//;
             if ($b =~ /^$base\./) {
               return 1;
             }
             else {
               return $a cmp $b;
             }
           }
           return $a cmp $b;
         } @_;
}


sub create {
  my($self)  = shift;
  my(@dirs)  = @_;
  my($fl)    = new FileLocator();
  my($modif,
     $remov,
     $confl,
     $unknown) = $fl->locate(@dirs);
  my($entry) = scalar(localtime());

  if (defined $$confl[0]) {
    $entry = "ERROR: The following files have conflicts:\n";
    foreach my $file (@$confl) {
      $entry .= "$file\n";
    }
  }
  else {
    my($prefix) = '        * ';

    ## Remove the timezone
    $entry =~ s/(:\d\d\s+)(.*)(\d\d\d\d)$/$1$3/;

    ## Add the name and email address
    $entry .= "  $self->{'name'}  <$self->{'email'}>\n\n";

    my($previous) = undef;
    foreach my $file ($self->sortFileList(@$modif)) {
      my($directory) = dirname($file);
      if (defined $previous && $previous ne $directory) {
        $entry .= "\n";
      }
      $entry .= "$prefix$file:\n";
      $previous = $directory;
    }
    $previous = '';
    my($removed) = 0;
    foreach my $file ($self->sortFileList(@$remov)) {
      my($directory) = dirname($file);
      if (defined $previous && $previous ne $directory) {
        $entry .= "\n";
      }
      $entry   .= "$prefix$file:\n";
      $previous = $directory;
      $removed++;
    }
    if ($removed) {
      $entry .= "\n          Removed " .
                ($removed > 1 ? 'these files' : 'this file') . ".\n";
    }
    $entry .= "\n";
  }

  if (!defined $$modif[0] && !defined $$remov[0]) {
    $entry = undef;
  }

  return $entry, $unknown;
}


1;
