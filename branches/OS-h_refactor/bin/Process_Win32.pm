# $Id$
package Process;

use Win32::Process;

sub Create
{
  my $name = shift;
  my $args = shift;
  my $self = [];

  my $console = 0;

  if ($newwindow && $newwindow eq "yes")
  {
    $console = CREATE_NEW_CONSOLE;
  }
  else
  {
    $console = 0;
  }

  Win32::Process::Create ($self->[0], $name, $name." ".$args, 0,
                          $console, ".");
  bless $self;
}

sub Kill
{
  my $self = shift;
  Win32::Process::Kill ($self->[0], -1);
}

sub Terminate
{
  my $self = shift;
  Win32::Process::Kill ($self->[0], -1);
}

sub Wait
{
  my $self = shift;
  Win32::Process::Wait ($self->[0], INFINITE);
}

sub TimedWait
{
  my $self = shift;
  my $maxtime = shift;
  if (Win32::Process::Wait ($self->[0], $maxtime * 1000) == 0) {
    return -1;
  }
  $status = 0;
  Win32::Process::GetExitCode ($self->[0], $status);
  return $status;
}

1;
