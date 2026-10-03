package PerlACE::ProcessNodeJS;

use strict;

use PerlACE::Process;
our @ISA = qw(PerlACE::Process);

sub new {
  my $class = shift();
  my $name = shift();
  my $args = shift();
  my $node_args = shift() // '';

  my $self = $class->SUPER::new("$name.js", $node_args);
  bless($self, $class);
  $self->IgnoreExeSubDir(1);
  $self->{VALGRIND_CMD} = "node $node_args";
  return $self;
}

1;
