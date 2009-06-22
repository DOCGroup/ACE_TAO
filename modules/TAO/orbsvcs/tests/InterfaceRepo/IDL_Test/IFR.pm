use warnings;
use strict;

require PerlACE::Process;

package IFR::Service;

sub new {
    my $class = shift;

    my $self = {@_};
    bless ($self, $class);
    $self->{iorfile} = "ifr.ior" if !defined($self->{iorfile});
    $self;
}

sub start {
    my $self = shift;
    $self->{ifr} = new PerlACE::Process("../../../IFR_Service/IFR_Service",
					"-o ifr.ior");
    $self->{ifr}->Spawn();
    if (PerlACE::waitforfile_timed ($self->{iorfile}, 15) == -1) {
	$self->{ifr}->Kill();
	return undef;
    }
    return $self->{ifr};
}

sub stop {
    my $self = shift;
    if (defined $self->{ifr}) {
	$self->{ifr}->Kill();
	unlink($self->{iorfile});
    }
}

sub is_running {
    my $self = shift;
    if (defined $self->{ifr}) {
	my $sigsent = kill(0, $self->{ifr}->{PROCESS});
	if ($sigsent == 1) {
	    return 1;
	}
    }
    return 0;
}

package IFR::Compiler;

sub new {
    # options that will be used:
    # - initref == initial reference for the service
    # - options == string containing options to be passed to compiler
    #   on every compilation
    my $class = shift;

    my $self = {@_};
    $self->{initref} = "file://ifr.ior" if !defined($self->{initref});
    bless ($self, $class);
    $self;
}

sub compile {
    my $self = shift;
    my $args = {@_};
    my @files = $args->{files};
    my $options = $args->{options};

    my @cmd = ("$ENV{ACE_ROOT}/bin/tao_ifr");
    
    push(@cmd, "-ORBinitref", "InterfaceRepository=".$self->{initref})
	if defined($self->{initref});
    push(@cmd, $self->{options}) if defined($self->{options});
    push(@cmd, $options) if defined($options);

    push(@cmd, @files);
    
    my $cmd = join(' ', @cmd);

    print "IFR::Compiler: running $cmd\n";
    system $cmd;
    return $?;
}

1;
