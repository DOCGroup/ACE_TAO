package Process;

$EXE_EXT = "";

sub Create
{
  my $name = shift;
  my $args = shift;
  my $self = [];
  unless ($self->[0] = fork ()) 
  {        
    unless (fork ()) 
    {
      exec $name." ".$args;
      die "no exec";
    }
    exit 0;    
  }
  bless $self;
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
