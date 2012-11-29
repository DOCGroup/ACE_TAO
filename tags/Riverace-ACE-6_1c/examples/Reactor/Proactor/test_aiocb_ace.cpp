
//=============================================================================
/**
 *  @file    test_aiocb_ace.cpp
 *
 *  $Id$
 *
 *  This program helps you to test the <aio_*> calls on a
 *  platform.
 *
 *  Before running this test, make sure the platform can
 *  support POSIX <aio_> calls, using
 *  ACE_ROOT/tests/Aio_Platform_Test.
 *
 *  This program tests the AIOCB (AIO Control Blocks) based
 *  completion approach which uses <aio_suspend> for completion
 *  querying.
 *
 *  If this test is successful, ACE_POSIX_AIOCB_PROACTOR
 *  can be used on this platform.
 *
 * = COMPILE and RUN
 *  % make
 *  % ./test_aiocb_ace
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/os_include/os_aio.h"
#include "ace/OS_NS_string.h"

class Test_Aio
{
public:
  /// Default constructor.
  Test_Aio (void);

  /// Initting the output file and the buffer.
  int init (void);

  /// Doing the testing stuff.
  int do_aio (void);

  /// Destructor.
  ~Test_Aio (void);
private:
  /// Output file descriptor.
  int out_fd_;

  /// For writing to the file.
  struct aiocb *aiocb_write_;

  /// Reading stuff from the file.
  struct aiocb *aiocb_read_;

  /// The buffer to be written to the out_fd.
  char *buffer_write_;

  /// The buffer to be read back from the file.
  char *buffer_read_;
};

Test_Aio::Test_Aio (void)
  : aiocb_write_ (0),
    aiocb_read_ (0),
    buffer_write_ (0),
    buffer_read_ (0)
{
  ACE_NEW (this->aiocb_write_,
           struct aiocb);
  ACE_NEW (this->aiocb_read_,
           struct aiocb);
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
  this->out_fd_ = ACE_OS::open ("test_aio.log",
                                O_RDWR | O_CREAT | O_TRUNC,
                                0666);
  if (this->out_fd_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error: Opening file\n"),
                      -1);

  // Init the buffers.
  this->buffer_write_ = ACE::strnew ("Welcome to the world of AIO... AIO Rules !!!");
  ACE_DEBUG ((LM_DEBUG,
              "The buffer : %s\n",
              this->buffer_write_));

  // Allocate memory for the read buffer.
  ACE_NEW_RETURN (this->buffer_read_,
                  char [ACE_OS::strlen (this->buffer_write_)],
                  -1);

  return 0;
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
  this->aiocb_write_->aio_nbytes = ACE_OS::strlen (this->buffer_write_);
  this->aiocb_write_->aio_reqprio = 0;
  this->aiocb_write_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_write_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_write_;

  // Fire off the aio write.
  if (aio_write (this->aiocb_write_) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "aio_write"),
                      -1);

  // = Read from that file.

  // Setup AIOCB.
  this->aiocb_read_->aio_fildes = this->out_fd_;
  this->aiocb_read_->aio_offset = 0;
  this->aiocb_read_->aio_buf = this->buffer_read_;
  this->aiocb_read_->aio_nbytes = ACE_OS::strlen (this->buffer_write_);
  this->aiocb_read_->aio_reqprio = 0;
  this->aiocb_read_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_read_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_read_;

  // Fire off the aio write. If it doesnt get queued, carry on to get
  // the completion for the first one.
  if (aio_read (this->aiocb_read_) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "aio_read"),
                      -1);

  // Wait for the completion on aio_suspend.
  struct aiocb *list_aiocb[2];
  list_aiocb [0] = this->aiocb_write_;
  list_aiocb [1] = this->aiocb_read_;

  // Do suspend till all the aiocbs in the list are done.
  int to_finish = 2;
  int return_val = 0;
  while (to_finish > 0)
    {
      return_val = aio_suspend (list_aiocb,
                                to_finish,
                                0);
      ACE_DEBUG ((LM_DEBUG,
                  "Result of <aio_suspend> : %d\n",
                  return_val));

      // Analyze return and error values.
      if (to_finish > 1)
        {
          if (aio_error (list_aiocb [1]) != EINPROGRESS)
            {
              if (aio_return (list_aiocb [1]) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "%p\n",
                                   "aio_return, item 1"),
                                  -1);
              else
                {
                  // Successful. Remember we have one less thing to finish.
                  --to_finish;
                  list_aiocb [1] = 0;
                }
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        "aio_error says aio 1 is in progress\n"));
        }

      if (aio_error (list_aiocb [0]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [0]) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "aio_return, item 0"),
                              -1);
          else
            {
              // Successful. Store the pointer somewhere and bump the
              // read entry up to the front, if it is still not done.
              --to_finish;
              list_aiocb [0] = this->aiocb_read_;
            }
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "aio_error says aio 0 is in progress\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "Both the AIO operations done.\n"
              "The buffer is : %s\n",
              this->buffer_read_));

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  Test_Aio test_aio;

  if (test_aio.init () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "AIOCB test failed:\n"
                       "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n"),
                      -1);

  if (test_aio.do_aio () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "AIOCB test failed:\n"
                       "ACE_POSIX_AIOCB_PROACTOR may not work in this platform\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "AIOCB test successful:\n"
              "ACE_POSIX_AIOCB_PROACTOR should work in this platform\n"));

  return 0;
}
