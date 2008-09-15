# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$nsiorfile = PerlACE::LocalFile("ns.ior");
unlink $nsiorfile;

my $exec_extn="";
if ($^O eq "MSWin32") {
  $exec_extn=".exe";
}

$nslist = "../../../../bin/nslist";
if (! -e $nslist . $exec_extn ) {
    $nslist = "../../../../TAO/utils/nslist/nslist";
    if (! -e $nslist . $exec_extn ) {
        print STDERR "ERROR: nslist utility not found.\n";
        exit 1;
    }
}

$status = 0;

# Fire up the Name Service
$NS   = new PerlACE::Process ("../../../../TAO/orbsvcs/Naming_Service/Naming_Service");
$NS->Arguments("-o $nsiorfile -ORBObjRefStyle URL -ORBEndpoint shmiop:// -ORBSvcConfDirective \"dynamic SHMIOP_Factory Service_Object *TAO_Strategies:_make_TAO_SHMIOP_Protocol_Factory () ''\"");
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsiorfile, 10) == -1)
{
   print STDERR "ERROR: cannot find Name Service IOR file <$nsiorfile>\n";
   $NS->Kill (); $NS->TimedWait (1);
   exit 1;
}

# Call nslist
$NSLIST  = new PerlACE::Process ($nslist);
$NSLIST->Arguments("-ORBSvcConfDirective \"dynamic SHMIOP_Factory Service_Object *TAO_Strategies:_make_TAO_SHMIOP_Protocol_Factory () ''\" -ORBInitRef NameService=file://$nsiorfile --ior --ctxior --node \"\" --tree \"\"");

$list_result = $NSLIST->SpawnWaitKill (300);

# Check return
if ($list_result != 0)
{
   print STDERR "ERROR: Regression - nslist returned $listresult\n";
   $status = 1;
}

# Shutting down NameService and clean up
$server = $NS->TerminateWaitKill (5);

if ($server != 0)
{
    print STDERR "ERROR: Closing Name Service returned $server\n";
    $status = 1;
}

unlink $nsiorfile;

if ($status == 0)
{
    print "Test passed !!\n";
}

exit $status;
