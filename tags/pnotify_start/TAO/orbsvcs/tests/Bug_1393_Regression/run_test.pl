eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-
# $Id$

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

# The location of the IFR_Service binary
$ifr_service_bin = "../../IFR_Service";

# The location of the tao_ifr utility binary
if ($^O eq "MSWin32") 
{
   $tao_ifr_bin = "../../../../bin";
}
else 
{
   $tao_ifr_bin = $ifr_service_bin;
}

# The location of the tao_idl utility binary
$tao_idl_bin = "../../../../TAO/TAO_IDL";

# The idl file to be used for the test
$test_idl = PerlACE::LocalFile("test.idl");

# Create an idl file contaning an interface with a C++ reserved word name
open(HANDLE, ">$test_idl");
print HANDLE <<_EOF_
interface int 
{
};
_EOF_
;
close(HANDLE);

$TAO_IDL    = new PerlACE::Process("$tao_idl_bin/tao_idl");
$TAO_IDL->Arguments("$test_idl");
$result = $TAO_IDL->SpawnWaitKill (30);

# Search one of the stub files for the munged for of the interface name
if ($result == 0) 
{
   $result = 1;
   open (HANDLE, "<testC.h");
   while (<HANDLE>)
   {
      if (/^class\s+_cxx_int/)
      {
         $result = 0;
         last;
      }
   }
   close(HANDLE);  
}

# Check result of idl compiler test
if ($result != 0)
{
   print STDERR "ERROR: Bug #1393 Idl compiler test failed\n";
   $status = 1;
}

# Delete files created during IDL compilation
unlink <test*.h>;
unlink <test*.cpp>;
unlink <test*.i>;
   
# The file name for the IFR service IOR
$ifr_service_ior_file = PerlACE::LocalFile("ifr.ior");
unlink $ifr_service_ior_file;   
   
# Start the IFR service
$IFR_SERVICE = new PerlACE::Process("$ifr_service_bin/IFR_Service");
$IFR_SERVICE->Arguments("-o $ifr_service_ior_file");
$IFR_SERVICE->Spawn();

if (PerlACE::waitforfile_timed ($ifr_service_ior_file, 10) == -1) 
{
   print STDERR "ERROR: cannot find $ifr_service_ior_file\n";
   $IFR_SERVICE->Kill ();
   $status = 1; 
}

# Use the iao_ifr utility to add our test IDL 
$TAO_IFR = new PerlACE::Process("$tao_ifr_bin/tao_ifr");
$TAO_IFR->Arguments("-ORBInitRef InterfaceRepository=file://$ifr_service_ior_file $test_idl"); 
$result = $TAO_IFR->SpawnWaitKill (30);

if ($result != 0)
{
   print STDERR "ERROR: Failed to add IDL to interface repository\n";
   $status = 1;
}

# Invoke the client code that retrieves the interface and checks it's not munged   
$CLIENT = new PerlACE::Process("client");
$CLIENT->Arguments("-ORBInitRef InterfaceRepository=file://$ifr_service_ior_file");
$result = $CLIENT->SpawnWaitKill (30);

if ($result != 0)
{
   print STDERR "ERROR: Interface definition incorrect in IFR\n";
   $status = 1;
}

# Tidy up
$IFR_SERVICE->TerminateWaitKill (10);
unlink $test_idl;
unlink $ifr_service_ior_file;

exit $status;
