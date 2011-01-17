#!/usr/bin/perl

# $Id$

use Socket;
use IO::Socket;

$filebits = '';

OpenServer();

my $rout;
while( 1 ) {
  print STDERR "fakeserver: Loop\n";

  select( undef, undef, undef, 1 );

  ($n, $left) = select( $rout = $filebits, undef, undef, 15 ); # Time out after 15 seconds
  if ($n == 0) {
    print STDERR "fakeserver: Terminating normally\n";
    exit 0;
  }

  my $routs = unpack("b*", $rout);
  print STDERR "fakeserver: Select $routs\n";
  my $pos = index( $routs,'1');
  while ( $pos >= 0 ) {
    HandleFile( $pos );
    $pos = index( $routs,'1', $pos+1);
  }
}

sub SendMessage {
  local( $message ) = @_;

  print STDERR "fakeserver: SendMessage $message\n";
  $message .= "\r\n";

  foreach $fileno (keys %connections) {
    if ( $connections{$fileno} ) {
      my $client = $connections{$fileno}{client};
      print $client $message;
    }
  }
}


sub HandleFile {
  local( $fileno ) = @_;

  print STDERR "fakeserver: HandleFile $fileno\n";
  if ( $fileno == $server_fileno ) {
    HandleServer();
  } elsif ( $connections{$fileno} ) {
    HandleClient( $fileno );
  } else {
    print STDERR "fakeserver: Weird fileno $fileno\n";
  }
}

sub HandleServer {
  my $client = $server->accept();

  print STDERR "fakeserver: HandleServer\n";

  if ( $client ) {
    my $fileno = fileno($client);
    $client->blocking(0);
    $connections{$fileno}{client} = $client;
    $connections{$fileno}{loggedin} = 0;
    vec($filebits,$fileno,1) = 1;
#    print $client "Welcome $fileno\r\n";
#    my $n = send($client, $closeconnectionmessage, 0);
    print STDERR "fakeserver: New client on $fileno\n";
#    SendMessage( "New Client" );
  } else {
    print STDERR "fakeserver: No accept for server, reopen\n";
    CloseServer();
    OpenServer();
  }
}

sub HandleClient {
  local( $fileno ) = @_;

  print STDERR "fakeserver: HandleClient $fileno\n";
  $n = recv( $connections{$fileno}{client}, $receive, 200, 0 );
  $rlen = length($receive);
  if ( $receive ) {
    if ($receive =~ /^GIOP/) {
      print STDERR "fakeserver:   Got $rlen byte GIOP message from client $fileno\n";
    }
    else {
      print STDERR "fakeserver:   Got $rlen bytes from client $fileno\n";
    }
    $connections{$fileno}{receive} = $receive;
    $slen = length($closeconnectionmessage);
    print STDERR "fakeserver:   Sending $slen bytes GIOP closeconnection to $fileno\n";
    sleep(3);
    $n = send($connections{$fileno}{client}, $closeconnectionmessage, 0);
    print STDERR "fakeserver:   Sent $n bytes GIOP closeconnection to $fileno\n";
  } else {
    print STDERR "fakeserver: Close client $fileno\n";
    vec($filebits,$fileno,1) = 0;
    $connections{$fileno}{client}->close();
    undef $connections{$fileno};
    SendMessage( "Close Client" );
  }

}

sub CloseServer {
  vec($filebits,$server_fileno,1) = 0;
  $server->close();
  undef $server;
}

sub OpenServer {

  $server = IO::Socket::INET->new(Listen    => 5,
							LocalPort => 1192,
							Reuse => 1,
							ReuseAddr => 1,
							Timeout   => 0,
							Proto     => 'tcp');

  die "Could not create socket $!" unless $server;

  $server->blocking(0);
  $server_fileno = fileno($server);
  vec($filebits,$server_fileno,1) = 1;

  print STDERR "fakeserver: Starting $server_fileno\n";

  open (DATA, 'closeconnection.dat') or die "couldn't open closeconnection.dat: $!\n";
  my $n = read(DATA, $closeconnectionmessage, 100);
  close(DATA);

  print STDERR "fakeserver: read $n <$closeconnectionmessage> ", length($closeconnectionmessage), "\n";
}
