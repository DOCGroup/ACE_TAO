// $Id$

// ============================================================================
//
// = LIBRARY
//    proactor
//
// = FILENAME
//    test_aiocb.cpp
//
// = DESCRIPTION
//    This program helps you to test the <aio_*> calls on a
//    platform. Before running this test, make sure the platform can 
//    support POSIX <aio_> calls. use $ACE_ROOT/tests for this.
//    This is for testing the AIOCB (AIO Control Blocks) based
//    completion approach which uses <aio_suspend> for completion
//    querying. 
//    If this test is successful, ACE_POSIX_AIOCB_PROACTOR
//    can be used on this platform.
// 
// = COMPILE and RUN
//    % CC -g -o test_aiocb -lrt test_aiocb.cpp
//    % ./test_aiocb
// 
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream.h>

#include <aio.h>

class Test_Aio
{
public:
  Test_Aio (void);
  // Default constructor.

  int init (void);
  // Initting the output file and the buffer.

  int do_aio (void);
  // Doing the testing stuff.

  ~Test_Aio (void);
  // Destructor.
private:
  int out_fd_;
  // Output file descriptor.

  struct aiocb *aiocb_write_;
  // For writing to the file.

  struct aiocb *aiocb_read_;
  // Reading stuff from the file.

  char *buffer_write_;
  // The buffer to be written to the out_fd.

  char *buffer_read_;
  // The buffer to be read back from the file.
};

Test_Aio::Test_Aio (void)
  : aiocb_write_ (new struct aiocb),
    aiocb_read_ (new struct aiocb),
    buffer_write_ (0),
    buffer_read_ (0)
{
}

Test_Aio::~Test_Aio (void)
{
  delete aiocb_write_;
  delete aiocb_read_;
  delete buffer_write_;
  delete buffer_read_;
}

// Init the output file and init the buffer.
int
Test_Aio::init (void)
{
  // Open the output file.
  this->out_fd_ = open ("test_aio.log", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (this->out_fd_ == 0)
    {
      cout << "Error : Opening file" << endl;
      return -1;
    }

  // Init the buffers.
  this->buffer_write_ = strdup ("Welcome to the world of AIO... AIO Rules !!!");
  cout << "The buffer : " << this->buffer_write_ << endl;
  this->buffer_read_ = new char [strlen (this->buffer_write_)];
}

// Set the necessary things for the AIO stuff.
// Write the buffer asynchly.hmm Disable signals.
// Go on aio_suspend. Wait for completion.
// Print out the result.
int
Test_Aio::do_aio (void)
{
  // = Write to the file.

  // Setup AIOCB.
  this->aiocb_write_->aio_fildes = this->out_fd_;
  this->aiocb_write_->aio_offset = 0;
  this->aiocb_write_->aio_buf = this->buffer_write_;
  this->aiocb_write_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_write_->aio_reqprio = 0;
  this->aiocb_write_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_write_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_write_;

  // Fire off the aio write.
  if (aio_write (this->aiocb_write_) != 0)
    {
      perror ("aio_write");
      return -1;
    }

  // = Read from that file.

  // Setup AIOCB.
  this->aiocb_read_->aio_fildes = this->out_fd_;
  this->aiocb_read_->aio_offset = 0;
  this->aiocb_read_->aio_buf = this->buffer_read_;
  this->aiocb_read_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_read_->aio_reqprio = 0;
  this->aiocb_read_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_read_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_read_;

  // Fire off the aio write. If it doesnt get queued, carry on to get
  // the completion for the first one.
  if (aio_read (this->aiocb_read_) < 0)
    perror ("aio_read");
  
  // Wait for the completion on aio_suspend.
  struct aiocb *list_aiocb[2];
  list_aiocb [0] = this->aiocb_write_;
  list_aiocb [1] = this->aiocb_read_;

  // Do suspend till all the aiocbs in the list are done.
  int done = 0;
  int return_val = 0;
  while (!done)
    {
      return_val = aio_suspend (list_aiocb,
                                2,
                                0);
      cerr << "Return value :" << return_val << endl;

      // Analyze return and error values.
      if (aio_error (list_aiocb [0]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [0]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              this->aiocb_write_ = list_aiocb [0];
              list_aiocb [0] = 0;
            }
        }
      else
        cout << "AIO in progress" << endl;

      if (aio_error (list_aiocb [1]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [1]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              this->aiocb_read_ = list_aiocb [1];
              list_aiocb [1] = 0;
            }
        }
      else
        cout << "AIO in progress" << endl;

      // Is it done?
      if ((list_aiocb [0] == 0) && (list_aiocb [1] == 0))
        done = 1;
    }

  cout << "Both the AIO operations done." << endl;
  cout << "The buffer is :" << this->buffer_read_ << endl;
  
  return 0;
}

int
main (int argc, char **argv)
{
  Test_Aio test_aio;

  if (test_aio.init () != 0)
    {
      printf ("AIOCB test failed:\n"
              "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n");
      return -1;
    }
  
  if (test_aio.do_aio () != 0)
    {
      printf ("AIOCB test failed:\n"
              "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n"); 
      return -1;
    }
  printf ("AIOCB test successful:\n"
          "ACE_POSIX_AIOCB_PROACTOR should work in this platform\n");
  return 0;
}
