
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status =0;

# The location of the tao_idl utility - depends on O/S
if ($^O eq "MSWin32")
{
   $tao_idl = "../../../bin/tao_idl";
}
else
{
   $tao_idl = "../../../TAO/TAO_IDL/tao_idl";
}

# Generated code file names
$TestCaseC_i     = PerlACE::LocalFile("TestCaseC.i");
unlink $TestCaseC_i;


# Compile the IDL
#
$TAO_IDL    = new PerlACE::Process("$tao_idl");
$TAO_IDL->Arguments( "-Ge 1 -Sc TestCase.idl" ) ;
if( $TAO_IDL->SpawnWaitKill(10) != 0 )
{
   print STDERR "ERROR: can't compile IDL\n";
   $TAO_IDL->Kill ();
   $status = 1;
   exit $status;
}

# Test for the presence of Body_dup etc. Crude test assumes that if
# Body_dup is there, then all the Body_* methods are also there.
#
$found = 0 ;
open (I_FILE, "$TestCaseC_i") ;
while( <I_FILE> )
{
   chomp ;
   if( $_ =~ /Body_dup/ )
   {
       $found = 1 ;
       last ;
   }
}
if( $found != 1 )
{
    print STDERR "ERROR: Body_* methods not found\n" ;
    $status = 1 ;
}

exit $status;
