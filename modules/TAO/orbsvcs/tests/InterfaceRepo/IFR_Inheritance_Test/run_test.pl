eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

my %isa = ();
$isa{"InterfaceRoot::get_root"}      = "operation";
$isa{"InterfaceRoot::get_all_roots"} = "operation";
$isa{"InterfaceRoot::root_attr1"}    = "attribute";
$isa{"InterfaceRoot::root_attr2"}    = "attribute";
$isa{"InterfaceRoot::root_attr3"}    = "attribute";
$isa{"SubInterface::get_root"}       = "operation";
$isa{"SubInterface::get_all_roots"}  = "operation";
$isa{"SubInterface::set_sub"}        = "operation";
$isa{"SubInterface::get_all_subs"}   = "operation";
$isa{"SubInterface::root_attr1"}     = "attribute";
$isa{"SubInterface::root_attr2"}     = "attribute";
$isa{"SubInterface::root_attr3"}     = "attribute";
$isa{"SubInterface::sub_attr1"}      = "attribute";
$isa{"SubInterface::sub_attr2"}      = "attribute";

my %got = ();
$got{"InterfaceRoot::get_root"}      = 13;
$got{"InterfaceRoot::get_all_roots"} = 13;
$got{"InterfaceRoot::root_attr1"}    = 13;
$got{"InterfaceRoot::root_attr2"}    = 13;
$got{"InterfaceRoot::root_attr3"}    = 13;
$got{"SubInterface::get_root"}       = 13;
$got{"SubInterface::get_all_roots"}  = 13;
$got{"SubInterface::set_sub"}        = 13;
$got{"SubInterface::get_all_subs"}   = 13;
$got{"SubInterface::root_attr1"}     = 13;
$got{"SubInterface::root_attr2"}     = 13;
$got{"SubInterface::root_attr3"}     = 13;
$got{"SubInterface::sub_attr1"}      = 13;
$got{"SubInterface::sub_attr2"}      = 13;

my $iorfile = "if_repo.ior";

my $testoutputfilename = "IFR_Inheritance_test.output";

unlink $testoutputfilename;

my $status = 0;

unlink $iorfile;

my $SV = new PerlACE::Process ("../../../IFR_Service/IFR_Service", " -o $iorfile" );

my $CL = new PerlACE::Process ("IFR_Inheritance_Test",
                            " -ORBInitRef InterfaceRepository=file://$iorfile");

# The location of the tao_idl utility - depends on O/S
my $tao_ifr = "undefined";
if ($^O eq "MSWin32")
{
   $tao_ifr = "$ENV{ACE_ROOT}/bin/tao_ifr";
}
else
{
   $tao_ifr = "../../../IFR_Service/tao_ifr";
}

# Compile the IDL
#
my $TAO_IFR    = new PerlACE::Process("$tao_ifr",
                            "IFR_Inheritance_Test.idl"
                          . " -ORBInitRef InterfaceRepository=file://$iorfile");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

if( $TAO_IFR->SpawnWaitKill(20) != 0 )
{
   print STDERR "ERROR: can't compile IDL\n";
   $TAO_IFR->Kill ();
   $status = 1;
   exit $status;
}

# Redirect STDERR to a log file so that
# we can make sure that we got a warning
open(SAVE, ">&STDOUT");
open(STDOUT, ">$testoutputfilename");

my $client = $CL->SpawnWaitKill (60);

# Close the log file and restore STDERR
close(STDOUT);
open(STDOUT, ">&SAVE");


my $type = "";
my $name = "";
open(THELOG, "< $testoutputfilename") or die "could not open the saved log";
while (<THELOG>) {
   if (/^attribute/ || "^operation") {
      chomp($_);
      ($type, $name) = split(/ /, $_);
      #print "found $type $name\n";
      if ($isa{$name} eq $type) {
        #print "got $name\n";
        $got{$name} = 1;
      }
      else {
        print STDERR "ERROR: $type $name was not expected\n";
        $status = 1;
      }
   }
   else {
     print STDOUT "ERROR: unexpected line: $_\n";
   }
}
close(THELOG);

### did we get all of the expected attributes and operations?
my $key = "";
foreach $key (keys (%got)) {
    #print "got\{$key\} = $got{$key}\n";
    if (! exists $isa{$key} ) {
        print STDERR "CONFIG ERROR: \$got has $key but \$isa does not\n";
        $status = 1;
    }
    if ($got{$key} == 13) {
        print STDERR "ERROR: $isa{$key} $key was not found\n";
        $status = 1;
    }
}

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

my $server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $testoutputfilename;

exit $status;
