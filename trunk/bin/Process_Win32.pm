package Process;

use Win32::Process;

$EXE_EXT = ".exe";

$newwindow = "yes";

sub Create
{
  my $name = shift;
  my $args = shift;
  my $self = [];

  if ($newwindow eq "no")
  {
    my $console = 0;
  }
  else
  {
    my $console = CREATE_NEW_CONSOLE;
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

sub Wait
{
  my $self = shift;
  Win32::Process::Wait ($self->[0], INFINITE);
}

1;