
#------------------------------------------------------------------------
# class: TestUtils
# Author: Stoyan Paunov
#
# Description: This is a class to help us write better and more manage-
#              able test utilities. Usually when creating a test in
#              CIAO it has to do with deploying a number of managers
#              and failure to deploy any process means failure of the
#              test. This test utility class takes care of the error
#              handling and clean-up and side of testing and allows
#              the test writer to concentrate on the core logic.
#              These are the main advantages of the TestUtils class:
#                --> Semi-Automatic error handling
#                --> Automatic process shutdown and clean-up
#                --> Semi-Automatic file (IOR) deletion
#                --> Location independent tests
#                --> Clean and concise code base
#                --> Manageable test utility suites
#------------------------------------------------------------------------

# class TestUtils
sub new;
sub DESTROY;
sub cleanup;
sub goto_dir;
sub spawn;
sub terminate;
sub required_file;
sub required_files;
sub mark_file;
sub mark_files;
sub remove_file;
sub remove_files;
sub cleanup_files;
sub cleanup_processes;

package TestUtils;
use PerlCIAO::TestUtils_Base;
use strict;
our @ISA = qw(TestUtils_Base);    # inherits from TestUtils_Base

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
use Cwd;

#------------------------------------------------------------------------
# Constructor
#------------------------------------------------------------------------
sub new {
    my $class = shift;

    #call the constructor of the parent class, TestUtils_Base.
    my $self = $class->SUPER::new();

    $self->{_wd} = getcwd;

    bless $self, $class;
    return $self;
}

#------------------------------------------------------------------------
# Destructor: performs clean-up
#------------------------------------------------------------------------

sub DESTROY {
    my $self = shift;

    #$self->cleanup (); check the CLEAN-UP description for explanation
}

#------------------------------------------------------------------------
# Cleanup: This fuction kills the spawned processes and deletes the
#          marked files. Optionally, once the cleanup is done it will
#          cause the program to exit, iff an exit code is specified.
#
#
#          NOTE: You need to call the cleanup () although it is called
#                in the destructor because this class is a wrapper around
#                PerlACE::Run_Test which also keep some internal state
#                and tries to do some clean-up. However its destructor
#                is called before this one, and this results in some
#                errors.
#------------------------------------------------------------------------

sub cleanup {
    my $self = shift;
    my $exit_code = shift;

    print "Performing clean-up ...\n";

    $self->cleanup_files ();
    $self->cleanup_processes ();

    print "Cleanup = DONE\n";

    chdir $self->wd ();

    if (defined ($exit_code)) {
       exit ($exit_code);
    }
}

#------------------------------------------------------------------------
# Goto_dir: This function allows you to change the current working
#           directory. Note that the class returns to the original
#           working directory upon exit.
#------------------------------------------------------------------------

#TODO: might want to push the dir to some stack
sub goto_dir {
   my $self = shift;
   my $dir = shift;

   if (! (chdir $dir)) {
      print STDERR "Failed to change directory to: $dir";
      $self->cleanup ();
   }

}

#------------------------------------------------------------------------
# Spawn: This function is used to spawn a process. It takes a descriptive
#        name under which it stores the process, the command line and the
#        arguments needed by the command. Optionally, you could specify
#        a timeout based on which the process would be spawned and if
#        it has not terminated after timeout seconds it will be killed.
#        If a failure occurs the function will perform clean-up and
#        terminate the program.
#------------------------------------------------------------------------

sub spawn {
    my $self = shift;
    my $name = shift;
    my $cmd = shift;
    my $args = shift;
    my $timeout = shift;

    if (!defined ($self->processes ())) {
       $self->{_processes} = {};
    }

    my $process = new PerlACE::Process ($cmd, $args);

    if (defined ($timeout)) {
       if ((my $ret = $process->SpawnWaitKill ($timeout)) == -1) {
          print STDERR "ERROR: Process $name returned $ret.\n";
          $self->cleanup (1);
       }
    }
    else {
       if ($process->Spawn () == -1) {
          $process->Kill ();
          #just in case, lets add it to the process list
          $self->processes->{$name} = $process;
          print STDERR "ERROR: Failure to spawn $name.\n";
          $self->cleanup (1);
       }
    }

  $self->processes->{$name} = $process;
  return $process;
}

#------------------------------------------------------------------------
# Terminate: This function takes in the descriptive process name passed
#            to Spawn, looks up the process corresponding to it and
#            kills it.
#------------------------------------------------------------------------

sub terminate {
   my $self = shift;
   my $pname = shift;

   $self->processes ()->{$pname}->Kill ();
   $self->processes ()->{$pname}->TimedWait (1);

   print STDERR "$pname teminated!\n";
}

#------------------------------------------------------------------------
# Required_file: This function checks if a required file is present in
#                the current working directory. If the file is missing
#                it performs cleanup and causes the program to exit.
#------------------------------------------------------------------------

sub required_file {
   my $self = shift;
   my $file = shift;

   if (PerlACE::waitforfile_timed
        ($file, $PerlACE::wait_interval_for_process_creation) == -1) {

       print STDERR
       "ERROR: Required file $file could not be found.\n";

       $self->cleanup (1);
   }
   return 1;
}

#------------------------------------------------------------------------
# Required_filez: This function does the same as required_file above
#                 except that it works on a reference (REF) to a list
#                 of required files.
#------------------------------------------------------------------------

sub required_files {
   my $self = shift;
   my $files = shift;
   my $pname = shift;

   foreach my $file (@{$files}) {

      if (PerlACE::waitforfile_timed
           ($file, $PerlACE::wait_interval_for_process_creation) == -1) {

         print STDERR
         "ERROR: Required file $file could not be found.\n";

         $self->cleanup (1);
      }
   }
   return 1;
}

#------------------------------------------------------------------------
# Mark_file: This function marks a file from the current working
#            directory for deletion. Once the file is marked it will be
#            deleted upon program termination. If the file cannot be
#            found, it is ignored.
#------------------------------------------------------------------------

sub mark_file {
   my $self = shift;
   my $file = shift;

   if (!defined $self->files ()) {
      $self->{_files} = [];
   }

   push @{$self->files ()}, $file;
}

#------------------------------------------------------------------------
# Mark_filez: This function does the same as mark_file above except
#             that it works on a reference (REF) to an array/list of
#             required files.
#------------------------------------------------------------------------

sub mark_files {
   my $self = shift;
   my $files = shift;

   if (!defined $self->files ()) {
      $self->{_files} = [];
   }

   foreach my $file (@{$files}) {
      push @{$self->files ()}, $file;
   }
}

#------------------------------------------------------------------------
# Remove_file: This fuction removes a file from the current working
#              directory. If the file is not there, it is ignored.
#------------------------------------------------------------------------

sub remove_file {
   my $self = shift;
   my $file = shift;

   my $path = PerlACE::LocalFile ($file);
   unlink $path;
}

#------------------------------------------------------------------------
# Remove_filez: This fuction removes a list of file from the current
#               working directory. It takes a REF of a list of files
#               and ignores files which are not found.
#------------------------------------------------------------------------

sub remove_files {
   my $self = shift;
   my $files = shift;

   foreach my $file (@{$files}) {
      my $path = PerlACE::LocalFile ($file);
      unlink $path;
   }
}

#------------------------------------------------------------------------
# Cleanup_files: clean us the files :)
#------------------------------------------------------------------------

sub cleanup_files {
   my $self = shift;

   if (defined ($self->files ())) {
      foreach my $file (@{$self->files ()}) {
         $self->remove_file ($file);
      }
   }
}

#------------------------------------------------------------------------
# Cleanup_processes: clean us the processes :)
#------------------------------------------------------------------------

sub cleanup_processes {
   my $self = shift;

   if (defined ($self->processes ())) {
      foreach my $pname ( keys %{$self->processes ()}) {
         $self->terminate ($pname);
         delete ($self->processes ()->{$pname});
      }
   }
}

#return value of the class
1;