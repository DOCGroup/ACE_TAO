package AutomakeWorkspaceHelper;

# ************************************************************
# Description   : An Automake Workspace Helper
# Author        : J.T. Conklin
# Create Date   : 9/01/2004
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use FileHandle;

use WorkspaceHelper;

use vars qw(@ISA);
@ISA = qw(WorkspaceHelper);

# ************************************************************
# Subroutine Section
# ************************************************************

sub write_settings {
  my($self)   = shift;
  my($wsc)    = shift;
  my($fh)     = shift;
  my(@locals) = @_;
  my($status) = 1;
  my($error)  = undef;
  my($crlf)   = $wsc->crlf();
  my($pfh)    = new FileHandle();

  my($seen_ace_root)     = 0;
  my($seen_tao_root)     = 0;
  my($seen_ace_builddir) = 0;
  my($seen_tao_builddir) = 0;
  my($seen_tao_idl)      = 0;

  foreach my $local (reverse @locals) {
    if (open($pfh,$local)) {
      while(<$pfh>) {
        if (/ACE_ROOT/) {
           $seen_ace_root = 1;
        }
        if (/TAO_ROOT/) {
           $seen_tao_root = 1;
        }
        if (/ACE_BUILDDIR/) {
           $seen_ace_builddir = 1;
        }
        if (/TAO_BUILDDIR/) {
           $seen_tao_builddir = 1;
        }
        if (/TAO_IDL/) {
           $seen_tao_idl = 1;
        }
      }

      close($pfh);
    }
    else {
      $status = 0;
      $error  = "Unable to open $local for reading.";
    }
  }

  if ($seen_ace_root || $seen_ace_builddir ||
      $seen_tao_root || $seen_tao_builddir) {

    if ($seen_ace_root) {
      if ($seen_tao_root || $seen_tao_builddir) {
        print $fh "ACE_ROOT = \$(top_srcdir)/..", $crlf;
      } else {
        print $fh "ACE_ROOT = \$(top_srcdir)", $crlf;
      }
    }
    if ($seen_ace_builddir) {
      if ($seen_tao_root || $seen_tao_builddir) {
        print $fh "ACE_BUILDDIR = \$(top_builddir)/..", $crlf;
      } else {
        print $fh "ACE_BUILDDIR = \$(top_builddir)", $crlf;
      }
    }
    if ($seen_tao_root) {
      print $fh "TAO_ROOT = \$(top_srcdir)", $crlf;
    }
    if ($seen_tao_builddir) {
      print $fh "TAO_BUILDDIR = \$(top_builddir)", $crlf;
    }

    print $fh $crlf;
  }

  if ($seen_tao_idl) {
    print $fh "TAO_IDL = ACE_ROOT=\$(ACE_ROOT) TAO_ROOT=\$(TAO_ROOT) \$(TAO_BUILDDIR)/TAO_IDL/tao_idl", $crlf;
    print $fh "TAO_IDLFLAGS = -Ge 1 -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I\$(TAO_ROOT) -I\$(srcdir) -g \$(ACE_BUILDDIR)/apps/gperf/src/gperf", $crlf;
    print $fh $crlf;
  }

  return $status, $error;
}


1;
