# $Id$
package Process;

use POSIX "sys_wait_h";

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
      die "ERROR: exec failed for <$name> <$args>";
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
      print STDERR "ERROR: Can't fork: $!\n";
    }
  }
}

sub Terminate
{
  my $self = shift;
  kill ('TERM', $self->[0]);
  # print STDERR "Process_Unix::Kill 'TERM' $self->[0]\n";
}

sub Kill
{
  my $self = shift;
  kill ('KILL', $self->[0]);
  # print STDERR "Process_Unix::Kill 'TERM' $self->[0]\n";
}

sub Wait
{
  my $self = shift;
  waitpid ($self->[0], 0);
}

sub TimedWait
{
  my $self = shift;
  my $maxtime = shift;
  while ($maxtime-- != 0) {
    my $pid = waitpid ($self->[0], &WNOHANG);
    if ($pid != 0 && $? != -1) {
      return $?;
    }
    sleep 1;
  }
  return -1;
}

1;
