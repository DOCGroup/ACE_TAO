eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $srv_arg = "-o server.ior ";
}
else {
    $srv_arg = "-o $iorfile ";
}
$clt_arg = "-k file://$iorfile ";

@configurations =
    (
     {
         description => "Testing server with '-ORBConnectIPV6Only 1'.\n",
         server => "$srv_arg -ORBConnectIPV6Only 1",
         client => "$clt_arg",
         error => 0,
         url => 0,
         url_address => "",
     },
     {
         description => "Testing client with '-ORBPreferIPV6Interfaces 1'.\n",
         server => "$srv_arg ",
         client => "$clt_arg -ORBPreferIPV6Interfaces 1",
         error => 0,
         url => 0,
         url_address => "",
     },
     {
         description => "Testing IPV4 server with client with '-ORBConnectIPV6Only 1'.\n",
         server => "$srv_arg -ORBListenEndpoints iiop://127.0.0.1",
         client => "$clt_arg -x -ORBConnectIPV6Only 1",
         error => 1,
         url => 0,
         url_address => "",
     },
     {
         description => "Testing IPV4 server with client and URL-style IOR",
         server => "$srv_arg -ORBListenEndpoints iiop://0.0.0.0 -ORBObjRefStyle url",
         client => "$clt_arg",
         error => 0,
         url => 0,
         url_address => "",
     },
     {
         description => "Testing IPV4 server with client and IPV6-forced URL",
         server => "$srv_arg -ORBListenEndpoints iiop://0.0.0.0 -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@[::1]:",
     },
     {
         description => "Testing IPV6 server with client and IPV4-forced URL",
         server => "$srv_arg -ORBListenEndpoints iiop://[::1] -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },
     {
         description => "Testing IPV6Only server with client and IPV4-forced URL",
         server => "$srv_arg -ORBConnectIPV6Only 1 -ORBObjRefStyle url",
         client => "-x",
         error => 1,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },
     {
         description => "Testing server with client and IPV4-forced URL",
         server => "$srv_arg -ORBObjRefStyle url",
         client => "",
         error => 0,
         url => 1,
         url_address => "corbaloc:iiop:1.2\@127.0.0.1:",
     },
    );


sub run_test_
{
    my $srvargs = @_[0]->{server};
    my $cltargs = @_[0]->{client};
    my $error = @_[0]->{error};
    my $stat  = 0;

    print STDERR "\n******************************************************\n";
    print @_[0]->{description};

    print "\nRunning server with the following args:\n$srvargs\n\n";

    if (PerlACE::is_vxworks_test()) {
        $SV = new PerlACE::ProcessVX ("server", $srvargs);
    }
    else {
        $SV = new PerlACE::Process ("server", $srvargs);
    }

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile,
                            $PerlACE::wait_interval_for_process_creation) == -1) {
        print STDERR "ERROR: cannot find file <$iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if (@_[0]->{url}) {
        my $fh;
        open $fh, $iorfile;
        my $url = <$fh>;
        close $fh;

        print "Changing corbaloc url from:\n$url\nto\n";

        my $url_addr = @_[0]->{url_address};
        if ($url =~ /corbaloc:iiop:1.[01234]\@[\[].*[\]]\:.*/) {
          $url =~ s/(corbaloc:iiop:1.[01234]\@[\[].*[\]]\:)(.*)/$url_addr$2/;
        }
        else {
          $url =~ s/(corbaloc:iiop:1.[01234]\@[^:]*\:)(.*)/$url_addr$2/;
        }
        print $url . "\n";

        $cltargs .= " -k \"$url\"";
    }

    print "\nRunning client with the following args:\n$cltargs\n\n";

    $CL = new PerlACE::Process ("client", $cltargs);

    $client = $CL->SpawnWaitKill (300);

    if ($client != 0) {
        print STDERR "ERROR: client returned $client\n";
        $stat = 1;
    }

    if ($error) {
      $server = $SV->TerminateWaitKill (10);
    }
    else {
      $server = $SV->WaitKill (10);

      if ($server != 0) {
          print STDERR "ERROR: server returned $server\n";
          $stat = 1;
      }
    }

    return $stat;
}


for $test (@configurations)
{
  if (run_test_($test) != 0) {
    $status = 1;
  }

  unlink $iorfile;
}

exit $status;
