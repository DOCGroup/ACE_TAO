eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";

require ACEutils;
use Cwd;

$cwd = getcwd();

################################################################################
# Program locations

$imr_ior = "$cwd$DIR_SEPARATOR" . "imr.ior";
$name_ior = "$cwd$DIR_SEPARATOR" . "name.ior";

$imr_server = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.".."
              .$DIR_SEPARATOR."ImplRepo_Service"
              .$DIR_SEPARATOR."ImplRepo_Service".$EXE_EXT;

$name_server = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.".."
               .$DIR_SEPARATOR."Naming_Service"
               .$DIR_SEPARATOR."Naming_Service".$EXE_EXT;


if ($^O eq "MSWin32") {
    # Just find it in the path
    $tao_imr = "tao_imr".$EXE_EXT;
}
else {
    $tao_imr = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR.".."
               .$DIR_SEPARATOR."ImplRepo_Service"
               .$DIR_SEPARATOR."tao_imr".$EXE_EXT;
}

$test = $EXEPREFIX."test".$EXE_EXT;

################################################################################

$errors = 0;

ACE::checkForTarget($cwd);

unlink $imr_ior;
unlink $name_ior;

$IR = Process::Create ($imr_server, "-o $imr_ior -d 0");

ACE::waitforfile ($imr_ior);

system ($tao_imr." -ORBInitRef ImplRepoService=file://$imr_ior"
                ." add NameService "
                ." -c \"$name_server"
                       ." -ORBInitRef ImplRepoService=file://$imr_ior"
                       ." -ORBUseIMR 1 .\"");

system ($tao_imr." -ORBInitRef ImplRepoService=file://$imr_ior"
                ." ior NameService -f $name_ior");

if (system ($test." -ORBInitRef NameService=file://$name_ior")) {
    ++$errors;
}

system ($tao_imr." -ORBInitRef ImplRepoService=file://$imr_ior"
                ." shutdown NameService ");

$IR->Kill (); $IR->Wait ();


if ($errors > 0) {
    print "Error: run_test.pl detected $errors errors!\n";
}
exit ($errors);
