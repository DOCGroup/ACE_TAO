# $Id$
package Process;

$EXE_EXT = "";

sub Create
{
  my $name = shift;
  my $args = shift;
  my $self = [];

  FORK:
  {
    if ($self->[0] = fork)
    {
      #parent here
      bless $self;
    }
    elsif (defined $self->[0])
    {
      #child here
      exec $name." ".$args;
      die "exec failed";
    }
    elsif ($! =~ /No more process/)
    {
      #EAGAIN, supposedly recoverable fork error
      sleep 5;
      redo FORK;
    }
    else 
    {
      # weird fork error
      die "Can't fork: $!\n";
    }
  }
}

sub Kill
{
  my $self = shift;
  kill (1, $self->[0]);
}

sub Wait
{
  my $self = shift;
  waitpid ($self->[0], 0);
}


1;
