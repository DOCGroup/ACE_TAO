# $Id$

package fakeserver;

use Socket;
use IO::Socket;

sub new {
  my $self = { };
  $self->{'filebits'} = '';
  $self->{'connections'} = { };
  $self->{'server'} = '';
  $self->{'server_fileno'} = '';
  $self->{''} = '';
}

sub run {
  my $self = shift;
  my $timeoutafter = shift; # in seconds
  my $rout;
  while ( 1 ) {
    print STDERR "fakeserver: Loop\n";

    select( undef, undef, undef, 1 );

    my ($n, $left) = select( $rout = $filebits, undef, undef, $timeoutafter); # Time out after 15 seconds
    if ($n == 0) {
      print STDERR "fakeserver: Terminating normally\n";
      exit 0;
    }

    my $routs = unpack("b*", $rout);
    print STDERR "fakeserver: Select $routs\n";
    my $pos = index( $routs,'1');
    while ( $pos >= 0 ) {
      $self->HandleFile( $pos );
      $pos = index( $routs,'1', $pos+1);
    }
  }
}

sub CloseServer {
  my $self = shift;
  vec($self->{'filebits'},$self->{'server_fileno'},1) = 0;
  $server->close();
  undef $server;
}

sub OpenServer {
  my $self = shift;
  my $server = IO::Socket::INET->new(Listen    => 5,
                                     LocalPort => 1192,
                                     Reuse => 1,
                                     ReuseAddr => 1,
                                     Timeout   => 0,
                                     Proto     => 'tcp');

  die "Could not create socket $!" unless $server;

  $server->blocking(0);
  my $server_fileno = fileno($server);
  vec($filebits,$server_fileno,1) = 1;

  print STDERR "fakeserver: Starting $server_fileno\n";

  open (DATA, 'closeconnection.dat') or die "couldn't open closeconnection.dat: $!\n";
  my $n = read(DATA, $closeconnectionmessage, 100);
  close(DATA);

  print STDERR "fakeserver: read $n <$closeconnectionmessage> ", length($closeconnectionmessage), "\n";
  $self->{'server'} = $server;
  $self->{'server_fileno'} = $server_fileno;
}

sub SendMessage {
  my $self = shift;
  my( $message ) = @_;

  print STDERR "fakeserver: SendMessage $message\n";
  $message .= "\r\n";

  foreach my $fileno (keys %{$self->{'connections'}}) {
    if ( ${$self->{'connections'}}{$fileno} ) {
      my $client = ${$self->{'connections'}}{$fileno}{client};
      print $client $message;
    }
  }
}

sub HandleFile {
  my $self = shift;
  my ( $fileno ) = @_;

  print STDERR "fakeserver: HandleFile $fileno\n";
  if ( $fileno == $self->{'server_fileno'} ) {
    HandleServer();
  } elsif ( ${$self->{'connections'}}{$fileno} ) {
    HandleClient( $fileno );
  } else {
    print STDERR "fakeserver: Weird fileno $fileno\n";
  }
}

sub HandleServer {
  my $self = shift;
  my $client = $server->accept();

  print STDERR "fakeserver: HandleServer\n";

  if ( $client ) {
    my $fileno = fileno($client);
    $client->blocking(0);
    ${$self->{'connections'}}{$fileno}{client} = $client;
    ${$self->{'connections'}}{$fileno}{loggedin} = 0;
    vec($self->{'filebits'},$fileno,1) = 1;
    print STDERR "fakeserver: New client on $fileno\n";
  } else {
    print STDERR "fakeserver: No accept for server, reopen\n";
    $self->CloseServer();
    $self->OpenServer();
  }
}

sub HandleClient {
  my $self = shift;
  my ( $fileno ) = @_;

  print STDERR "fakeserver: HandleClient $fileno\n";
  my $receive;
  my $n = recv( ${$self->{'connections'}}{$fileno}{client}, $receive, 200, 0 );
  my $rlen = length($receive);
  if ( $receive ) {
    if ($receive =~ /^GIOP/) {
      print STDERR "fakeserver:   Got $rlen byte GIOP message from client $fileno\n";
    }
    else {
      print STDERR "fakeserver:   Got $rlen bytes from client $fileno\n";
    }
    ${$self->{'connections'}}{$fileno}{receive} = $receive;
    my $slen = length($closeconnectionmessage);
    print STDERR "fakeserver:   Sending $slen bytes GIOP closeconnection to $fileno\n";
    sleep(3);
    $n = send(${$self->{'connections'}}{$fileno}{client}, $closeconnectionmessage, 0);
    print STDERR "fakeserver:   Sent $n bytes GIOP closeconnection to $fileno\n";
  } else {
    print STDERR "fakeserver: Close client $fileno\n";
    vec(${$self->{'filebits'}},$fileno,1) = 0;
    ${$self->{'connections'}}{$fileno}{client}->close();
    undef ${$self->{'connections'}}{$fileno};
    $self->SendMessage( "Close Client" );
  }
}

