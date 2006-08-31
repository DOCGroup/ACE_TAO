# $Id$
# -*- perl -*-

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# import module and libraries
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use Sys::Hostname;
use Cwd;

# get the CIAO_ROOT and WLG_ROOT
$CIAO_ROOT = $ENV{'CIAO_ROOT'};

# default values
$hostname = hostname();
$port = 10000;
$spawn = 1;

# parse the command line arguments
while ($arg = shift)
{
    $arg eq "--localhost" and $hostname = "localhost";
    $arg eq "--port" and !($port = int (shift)) and die "*** syntax error: invalid port number\n";
    $arg eq "--spawn" and !($spawn = int (shift)) and die "*** syntax error: invalid number of daemons to spawn\n";
    $arg eq "--help" and print_help () and exit;
}

$current_dir = cwd;
print "\nNode Daemon Profile\n";
print "------------------------------------\n";
print "Current Directory  : $current_dir\n";
print "NodeDaemon Name    : $hostname\n";
if ($spawn == 1) {
    print "NodeDaemon Port    : $port\n";
}
elsif ($spawn > 1) {
    $last_port = $port + $spawn - 1;
    print "NodeDaemon Ports   : $port - $last_port\n";
}
else {
    die "*** syntax error: spawn value cannot be less than or equal to 0";
}

# spawn a new Node_Daemon
$node_daemon = "$CIAO_ROOT/DAnCE/NodeManager/NodeManager";
@daemons = ();
for (1..$spawn) {
    $node_args = "-ORBEndpoint iiop://$hostname:$port -s $CIAO_ROOT/DAnCE/NodeApplication/NodeApplication";
    $port ++;
    print $node_args;

    $temp = new PerlACE::Process ($node_daemon, $node_args);
    if ($temp->Spawn () != -1) {
  push @daemons, $temp;
    }
}

# sleep indefinitely
sleep(999999999);
1;

#
# print_help
#
sub print_help
{
    $syntax = "wlg_node.pl [--localhost] [--port value] [--spawn value]";
    %help = (
       'localhost'     => "run as localhost instead of name returned by hostname()",
       'port [value]'  => "set the port value; default = 10000",
       'spawn [value]' => "number of Node_Daemon's to spawn; port values are contiguous",
       'help'          => "display this help message"
       );

    print "SYNTAX: $syntax\n\n";
    while (($key, $value) = each %help) {
  print "  --$key       $value\n";
    }
    1;
}

1;

