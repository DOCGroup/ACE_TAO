eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../../bin';
require ACEutils;
require Uniqueid;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);
$status = 0;

$conf_file = 'exhaustive.conf';
@dispatching_configs = ("-ECDispatching reactive",
			"-ECDispatching mt -ECDispatchingThreads 4");
@collection_strategies = ("copy_on_read",
			  "copy_on_write",
			  "delayed");
@collection_types = ("list", "rb_tree");
@filtering_configs = ("-ECFiltering prefix -ECSupplierFilter per-supplier",
		      "-ECFiltering prefix -ECSupplierFilter null");

foreach $d (@dispatching_configs) {
  foreach $f (@filtering_configs) {
    foreach $c (@collection_strategies) {
      foreach $t (@collection_types) {
	local $collection = "mt:".$c.":".$t;
	local $config =
	  'static EC_Factory "'
	  .$d
	  ." -ECProxyPushConsumerCollection ".$collection
	  ." -ECProxyPushSupplierCollection ".$collection
	  ." ".$f
	  .'"';
	open (CONFIG,">$conf_file")
	  || die "Cannot open $conf_file\n";
	print CONFIG $config, "\n";
	close (CONFIG);

	print STDERR "\n\n", $config, "\n";

	$T = Process::Create ($EXEPREFIX . "Random".$EXE_EXT,
			      " -ORBSvcConf "
			      .$cwd.$DIR_SEPARATOR.
			      "$conf_file"
			      ." -suppliers 16"
			      ." -consumers 16"
			      ." -max_recursion 0");
	if ($T->TimedWait (900) == -1) {
	  print STDERR "ERROR: Test timedout\n";
	  $status = 1;
	  $T->Kill (); $T->TimedWait (1);
	}
      }
    }
  }
}

unlink "$conf_file";

exit $status;
