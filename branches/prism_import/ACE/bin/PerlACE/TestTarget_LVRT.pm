# $Id$
#
# TestTarget_LVRT - how to manage the test environment on a LabVIEW RT target.
#
# We can FTP files to and from the LabVIEW target, but there's no NFS or
# SMB shares.
# Most information about the target itself is specified via environment
# variables. The current environment variables are:
#   ACE_RUN_LVRT_TGTHOST - the host name/IP of the target.
#   ACE_RUN_LVRT_FSROOT  - the root of the filesystem on the target where
#                          ACE files will be created from (cwd, if you will).
#                          If not specified, "\ni-rt" is used as the root.

package PerlACE::TestTarget_LVRT;
our @ISA = "PerlACE::TestTarget";

### Constructor and Destructor

sub new
{
    my $proto = shift;
    my $class = ref ($proto) || $proto;
    my $self = {};

    my $targethost;
    if (defined $ENV{'ACE_RUN_LVRT_TGTHOST'}) {
        $targethost = $ENV{'ACE_RUN_LVRT_TGTHOST'};
    }
    else {
        print STDERR "You must define target hostname/IP with ",
                     "ACE_RUN_LVRT_TGTHOST\n";
        return -1;
    }

    my $fsroot = $ENV{'ACE_RUN_LVRT_FSROOT'};
    if (!defined $fsroot) {
        $fsroot = '\\ni-rt';
    }
    $self->{FSROOT} = $fsroot;

    $self->{FTP} = new Net::FTP ($targethost);
    $self->{TGTHOST} = $targethost;
    if (!defined $self->{FTP}) {
        print STDERR "$@\n";
        return -1;
    }
    $self->{FTP}->login("","");
    $self->{TARGET} = undef;
    $self->{REBOOT_TIME} = $ENV{"ACE_RUN_LVRT_REBOOT_TIME"};
    if (!defined $self->{REBOOT_TIME}) {
        $self->{REBOOT_TIME} = 200;
    }

    bless ($self, $class);
    return $self;
}

##################################################################

sub LocalFile ($)
{
    my $self = shift;
    my $file = shift;
    my $newfile = $self->{FSROOT} . '\\' . $file;
    print STDERR "LVRT LocalFile for $file is $newfile\n";
    return $newfile;
}

sub DeleteFile ($)
{
    my $self = shift;
    my $file = shift;
    $self->{FTP}->login("","");
    $self->{FTP}->delete($file);
}

sub WaitForFileTimed ($)
{
    my $self = shift;
    my $file = shift;
    my $timeout = shift;
    my $targetport = 8888;
    my $target = new Net::Telnet(Timeout => 600, Errmode => 'return');
    if (!$target->open(Host => $self->{TGTHOST}, Port => $targetport)) {
        print STDERR "ERROR: target $self->{TGTHOST}:$targetport: ",
                      $target->errmsg(), "\n";
        return -1;
    }
    my $cmdline = "waitforfile $file $timeout";
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print "-> $cmdline\n";
    }
    $target->print("$cmdline");
    my $reply;
    $reply = $target->getline();
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
      print "<- $reply\n";
    }
    $target->close();
    if ($reply eq "OK\n") {
        return 0;
    }
    return -1;
}

sub CreateProcess ($)
{
    my $self = shift;
    my $process = new PerlACE::ProcessLVRT (@_);
    return $process;
}

sub GetStderrLog ($)
{
    my $self = shift;
    # Tell the target to snapshot the stderr log; if there is one, copy
    # it up here and put it out to our stderr.
    my $targetport = 8888;
    my $target = new Net::Telnet(Timeout => 600, Errmode => 'return');
    if (!$target->open(Host => $self->{TGTHOST}, Port => $targetport)) {
        print STDERR "ERROR: target $self->{TGTHOST}:$targetport: ",
                      $target->errmsg(), "\n";
        return;
    }
    my $cmdline = "snaplog";
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print "-> $cmdline\n";
    }
    $target->print("$cmdline");
    my $reply;
    $reply = $target->getline();
    if (defined $ENV{'ACE_TEST_VERBOSE'}) {
        print "<- $reply\n";
    }
    $target->close();
    if ($reply eq "NONE\n") {
        return;
    }
    chomp $reply;
    $self->{FTP}->ascii();
    if ($self->{FTP}->get($reply, "stderr.txt")) {
        $self->{FTP}->delete($reply);
        open(LOG, "stderr.txt");
        while (<LOG>) {
            print STDERR;
        }
        close LOG;
        unlink "stderr.txt";
    }
    return;
}

# Copy a file to the target. Adjust for different types (DLL, EXE, TEXT)
# and debug/non (for DLLs). Additionally, a file can be removed when this
# object is deleted, or left in place.
sub NeedFile ($)
{
    my $self = shift;
}

1;
