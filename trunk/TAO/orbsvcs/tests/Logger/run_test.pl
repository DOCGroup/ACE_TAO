# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This is a Perl script that runs the Logger client and server

unshift @INC, '../../../../bin';
require Process;

# amount of delay between running the servers
$sleeptime = 3;

sub server
{
  my $args = "";
  my $prog = "server".$Process::EXE_EXT;
    
  $SV = Process::Create ($prog, $args);
}

sub client
{
  my $args = "";
  my $prog = "client".$Process::EXE_EXT;
    
  system ("$prog $args");
}

server ();

sleep $sleeptime;

client ();

sleep 10;

$SV->Kill ();
