eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";

require ACEutils;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);

$persistent = "-p";

$ifr_service = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
               "..".$DIR_SEPARATOR."IFR_Service".$DIR_SEPARATOR.
	       "IFR_Service".$EXE_EXT, " $persistent";

$init_ref = 
  "-ORBInitRef InterfaceRepository=file://if_repo.ior";

$iorfile = "$cwd$DIR_SEPARATOR" . "if_repo.ior";
$backing_file = "$cwd$DIR_SEPARATOR" . "ifr_default_backing_store";

$debug = "";
$query_opt = "-q";
$other = "";

for ($i = 0; $i <= $#ARGV; $i++)
{
  SWITCH:
  {
    if ($ARGV[$i] eq "-d")
    {
      $debug = "-d";
      last SWITCH;
    }
    $other .= $ARGV[$i];
  }
}

unlink $iorfile;
unlink $backing_file;

print "\t IFR Persistence Test\n\n";

$NEW_IFR = Process::Create ($EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
               "..".$DIR_SEPARATOR."IFR_Service".$DIR_SEPARATOR.
	       "IFR_Service".$EXE_EXT, " $persistent");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) 
{
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $NEW_IFR->Kill (); 
  $NEW_IFR->TimedWait (1);
  exit 1;
}

$POPULATE = Process::Create ($EXEPREFIX."Persistence_Test".$EXE_EXT,
                             " $init_ref");

$populate = $POPULATE->TimedWait (60);
if ($populate == -1) 
{
  print STDERR "ERROR: populate timedout\n";
  $POPULATE->Kill (); 
  $POPULATE->TimedWait (1);
}

$NEW_IFR->Kill (); 
$NEW_IFR->TimedWait (1);

unlink $iorfile;

$FILE_IFR = Process::Create ($EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.
               "..".$DIR_SEPARATOR."IFR_Service".$DIR_SEPARATOR.
	       "IFR_Service".$EXE_EXT, " $persistent");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) 
{
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $FILE_IFR->Kill (); 
  $FILE_IFR->TimedWait (1);
  exit 1;
}

$QUERY = Process::Create ($EXEPREFIX."Persistence_Test".$EXE_EXT,
                          " $init_ref $debug $query_opt");

$query = $QUERY->TimedWait (60);
if ($query == -1) 
{
  print STDERR "ERROR: query timedout\n";
  $QUERY->Kill (); 
  $QUERY->TimedWait (1);
}

$FILE_IFR->Kill (); 
$FILE_IFR->TimedWait (1);

#unlink $iorfile;
#unlink $backing_file;

exit 0;

