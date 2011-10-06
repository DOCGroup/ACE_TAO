package ChangeLogEdit;

# ************************************************************
# Description   : Edit the existing ChangeLog.
# Author        : Chad Elliott
# Create Date   : 9/10/2002
# $Id$
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;
use File::Copy;

use ChangeLogEntry;

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


sub edit {
  my($self)    = shift;
  my($ofile)   = shift;
  my(@dirs)    = @_;
  my($tfile)   = "$ofile.$<.$$";
  my($status)  = 0;
  my($error)   = '';
  my($rh)      = new FileHandle();
  my($unknown) = undef;

  if (open($rh, $ofile)) {
    my($creator) = new ChangeLogEntry($self->{'name'},
                                      $self->{'email'});
    my($entry) = '';
    ($entry, $unknown, $error) = $creator->create(@dirs);
    if (defined $entry) {
      if ($entry =~ /^ERROR:/) {
        $error = $entry;
      }
      else {
        my($oh) = new FileHandle();
        if (open($oh, ">$tfile")) {
          $status = print $oh $entry;
          if ($status) {
            while(<$rh>) {
              my($line) = $_;
              $line =~ s/\s+$//;
              if ($line =~ /\t/) {
                $line = $self->convertTabs($line);
              }
              $status = print $oh "$line\n";
              if ($status == 0) {
                $error = "Unable to copy $ofile";
                last;
              }
            }
          }
          else {
            $error = 'Unable to print the first entry';
          }
          close($oh);
        }
        else {
          $error = "Unable to open $tfile for writing";
        }
        close($rh);

        if ($status) {
          $status = 0;
          if (unlink($ofile)) {
            if (rename($tfile, $ofile)) {
              $status = 1;
            }
            else {
              $error = "Unable to rename $tfile to $ofile";
            }
          }
          else {
            $error = "Unable to remove $ofile";
          }
        }
      }
    }
    else {
      $error = (defined $error ? "There is a revision control system " .
                                 "problem:\n$error" :
                                 'There are no modified/removed files.');
    }
  }
  else {
    $error = "Unable to open $ofile for reading";
  }

  return $status, $error, $unknown;
}


sub convertTabs {
  my($self) = shift;
  my($line) = shift;
  while($line =~ /\t/) {
    my($spaces) = 8 - (index($line, "\t") % 8);
    $line =~ s/\t/sprintf("%${spaces}s", ' ')/e;
  }
  return $line;
}


1;
