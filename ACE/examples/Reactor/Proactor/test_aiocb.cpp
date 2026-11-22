
/**
 *  @file    test_aiocb.cpp
 *
 *  Checkout $ACE_ROOT/examples/Reactor/Proactor/test_aiocb_ace.cpp,
 *  which is the ACE'ified version of this program.
 *
 * = COMPILE and RUN
 *  % CC -g -o test_aiocb -lrt test_aiocb.cpp
 *  % ./test_aiocb
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */

//FUZZ: disable check_for_lack_ACE_OS
//FUZZ: disable check_for_improper_main_declaration

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <aio.h>

class Test_Aio
{
public:
  Test_Aio () = default;
  Test_Aio (Test_Aio const &) = delete;
  Test_Aio (Test_Aio &&) = delete;
  Test_Aio &operator= (Test_Aio const &) = delete;
  Test_Aio &operator= (Test_Aio &&) = delete;

  /// Initting the output file and the buffer.
  int init ();

  /// Doing the testing stuff.
  int do_aio ();

  ~Test_Aio ();
private:
  /// Output file descriptor.
  int out_fd_ {-1};

  /// For writing to the file.
  struct aiocb aiocb_write_ {};

  /// Reading stuff from the file.
  struct aiocb aiocb_read_ {};

  /// The buffer to be written to the out_fd.
  std::string buffer_write_ {"Welcome to the world of AIO... AIO Rules !!!"};

  /// The buffer to be read back from the file.
  std::vector<char> buffer_read_;
};

Test_Aio::~Test_Aio ()
{
  if (this->out_fd_ != -1)
    {
      close (this->out_fd_);
    }
}

// Init the output file and init the buffer.
int
Test_Aio::init ()
{
  // Open the output file.
  this->out_fd_ = open ("test_aio.log", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (this->out_fd_ == -1)
    {
      std::cout << "Error : Opening file" << std::endl;
      return -1;
    }

  // Init the buffers.
  std::cout << "The buffer : " << this->buffer_write_ << std::endl;
  this->buffer_read_.resize (this->buffer_write_.size () + 1);
  return 0;
}

// Set the necessary things for the AIO stuff.
// Write the buffer asynchly.hmm Disable signals.
// Go on aio_suspend. Wait for completion.
// Print out the result.
int
Test_Aio::do_aio ()
{
  // Write to the file.

  // Setup AIOCB.
  this->aiocb_write_.aio_fildes = this->out_fd_;
  this->aiocb_write_.aio_offset = 0;
  this->aiocb_write_.aio_buf = this->buffer_write_.data ();
  this->aiocb_write_.aio_nbytes = this->buffer_write_.size ();
  this->aiocb_write_.aio_reqprio = 0;
  this->aiocb_write_.aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_write_.aio_sigevent.sigev_value.sival_ptr =
    std::addressof (this->aiocb_write_);

  // Fire off the aio write.
  if (aio_write (std::addressof (this->aiocb_write_)) != 0)
    {
      perror ("aio_write");
      return -1;
    }

  // Read from that file.

  // Setup AIOCB.
  this->aiocb_read_.aio_fildes = this->out_fd_;
  this->aiocb_read_.aio_offset = 0;
  this->aiocb_read_.aio_buf = this->buffer_read_.data ();
  this->aiocb_read_.aio_nbytes = this->buffer_write_.size ();
  this->aiocb_read_.aio_reqprio = 0;
  this->aiocb_read_.aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_read_.aio_sigevent.sigev_value.sival_ptr =
    std::addressof (this->aiocb_read_);

  // Fire off the aio write. If it doesnt get queued, carry on to get
  // the completion for the first one.
  if (aio_read (std::addressof (this->aiocb_read_)) < 0)
    {
      perror ("aio_read");
    }

  // Wait for the completion on aio_suspend.
  struct aiocb *list_aiocb[2];
  list_aiocb [0] = std::addressof (this->aiocb_write_);
  list_aiocb [1] = std::addressof (this->aiocb_read_);

  // Do suspend till all the aiocbs in the list are done.
  bool done {false};
  while (!done)
    {
      int const return_val = aio_suspend (list_aiocb,
                                          2,
                                          nullptr);
      std::cerr << "Return value :" << return_val << std::endl;

      // Analyze return and error values.
      if (list_aiocb[0] != nullptr)
        {
          if (aio_error (list_aiocb [0]) != EINPROGRESS)
            {
              if (aio_return (list_aiocb [0]) == -1)
                {
                  perror ("aio_return");
                  return -1;
                }
              else
                {
                  // Mark the request complete in the list.
                  list_aiocb [0] = nullptr;
                }
            }
          else
            {
              std::cout << "AIO write in progress" << std::endl;
            }
        }

      if (list_aiocb[1] != nullptr)
        {
          if (aio_error (list_aiocb [1]) != EINPROGRESS)
            {
              ssize_t const read_return = aio_return (list_aiocb[1]);
              if (read_return == -1)
                {
                  perror ("aio_return");
                  return -1;
                }
              else
                {
                  // Mark the request complete in the list.
                  list_aiocb [1] = nullptr;
                  this->buffer_read_[static_cast<size_t> (read_return)] = '\0';
                }
            }
          else
            {
              std::cout << "AIO read in progress" << std::endl;
            }
        }

      // Is it done?
      if (list_aiocb [0] == nullptr && list_aiocb [1] == nullptr)
        {
          done = true;
        }
    }

  std::cout << "Both the AIO operations done." << std::endl;
  std::cout << "The buffer is :" << this->buffer_read_.data () << std::endl;

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
