package AutomakeWorkspaceHelper;

# ************************************************************
# Description   : An Automake Workspace Helper
# Author        : Chad Elliott
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
# Data Section
# ************************************************************

my(%vals)  = ('ACE_ROOT'      => '$(top_srcdir)',
              'TAO_ROOT'      => '$(top_srcdir)',
              'CIAO_ROOT'     => '$(top_srcdir)',
              'ACE_BUILDDIR'  => '$(top_builddir)',
              'TAO_BUILDDIR'  => '$(top_builddir)',
              'CIAO_BUILDDIR' => '$(top_builddir)',
              'TAO_IDL'       => 'ACE_ROOT=$(ACE_ROOT) TAO_ROOT=$(TAO_ROOT) $(TAO_BUILDDIR)/TAO_IDL/tao_idl' . "\n" .
				 'TAO_IDL_DEP = $(TAO_BUILDDIR)/TAO_IDL/tao_idl' . "\n" .
                                 'TAO_IDLFLAGS = -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h -I$(TAO_ROOT) -I$(srcdir) -g $(ACE_BUILDDIR)/apps/gperf/src/gperf',
             );
my(%addon) = ('ACE_ROOT'     => {'CIAO_ROOT'     => '/../..',
                                 'TAO_ROOT'      => '/..',
                                 'CIAO_BUILDDIR' => '/../..',
                                 'TAO_BUILDDIR'  => '/..'},
              'ACE_BUILDDIR' => {'CIAO_ROOT'     => '/../..',
                                 'TAO_ROOT'      => '/..',
                                 'CIAO_BUILDDIR' => '/../..',
                                 'TAO_BUILDDIR'  => '/..'},
              'TAO_ROOT'     => {'CIAO_ROOT'     => '/..',
                                 'CIAO_BUILDDIR' => '/..'},
              'TAO_BUILDDIR' => {'CIAO_ROOT'     => '/..',
                                 'CIAO_BUILDDIR' => '/..'},
             );

# ************************************************************
# Subroutine Section
# ************************************************************

sub modify_value {
  my($self)  = shift;
  my($name)  = shift;
  my($value) = shift;

  if ($name eq 'includedir') {
    ## TAO/orbsvcs and TAO are like separate projects, so first
    ## remove the TAO/orbsvcs part and if that doesn't work try
    ## removing the TAO part.  The ACE related values don't need
    ## any modification.
    if (!($value =~ s/^\/TAO\/orbsvcs//)) {
      $value =~ s/^\/TAO//;
    }
  }

  return $value;
}

sub modify_libpath {
  my($self)    = shift;
  my($str)     = shift;
  my($reldir)  = shift;
  my($libname) = shift;

  if ($libname =~ /libace/i) {
    $str =~ s!$libname!\$(ACE_BUILDDIR)/$reldir/$libname!;
    return $str;
  }
  elsif ($libname =~ /libtao/i) {
    $reldir =~ s!TAO/!!;
    $str =~ s!$libname!\$(TAO_BUILDDIR)/$reldir/$libname!;
    return $str;
  }
  elsif ($libname =~ /libciao/i) {
    $reldir =~ s!TAO/CIAO/!!;
    $str =~ s!$libname!\$(CIAO_BUILDDIR)/$reldir/$libname!;
    return $str;
  }

  return undef;
}

sub write_settings {
  my($self)   = shift;
  my($wsc)    = shift;
  my($fh)     = shift;
  my(@locals) = @_;
  my($status) = 1;
  my($error)  = undef;
  my($crlf)   = $wsc->crlf();
  my($pfh)    = new FileHandle();
  my(%seen)   = ();
  my($outdir) = $wsc->get_outdir();

  foreach my $local (reverse @locals) {
    if (open($pfh, "$outdir/$local")) {
      while(<$pfh>) {
        foreach my $key (keys %vals) {
          if (/\$\($key\)/) {
            $seen{$key} = $vals{$key};
          }
        }
      }
      close($pfh);
    }
    else {
      $status = 0;
      $error  = "Unable to open $local for reading.";
    }
  }

  foreach my $key (sort keys %seen) {
    print $fh "$key = $seen{$key}";
    if (defined $addon{$key}) {
      foreach my $add (
        sort { length($addon{$key}->{$b}) <=>
               length($addon{$key}->{$a}) } keys %{$addon{$key}}) {
        if ($seen{$add}) {
          print $fh $addon{$key}->{$add};
          last;
        }
      }
    }
    print $fh $crlf;
  }

  print $fh $crlf;

  return $status, $error;
}


1;
