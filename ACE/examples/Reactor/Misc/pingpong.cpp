/* Simple program that illustrates many features of the ACE_Reactor:

   1. I/O event demultiplexing
   2. Signal-based demultiplexing
   3. Timer-based demultiplexing

   To test this program, compile it and then execute it as follows:

   % ./pingpong hello

   You should see lots of the following output:

   writing <4> [7860]
   writing <4> [7860]
   writing <4> [7860]
   writing <4> [7860]
   reading <5> (7860) [1] = hello
   writing <4> [7860]
   writing <5> [7861]
   reading <4> (7861) [2] = hello
   reading <5> (7860) [2] = hello
   writing <4> [7860]
   writing <5> [7861]
   reading <4> (7861) [3] = hello
   reading <5> (7860) [3] = hello

   After 10 seconds you'll see the following:

   ./pingpong: shutting down tester (pid = 7861)
   ./pingpong: shutting down tester (pid = 7860)

   and the program will stop.  If you'd like to
   stop it earlier, just hit the control-C sequence
   and you'll see the same messages. */

#include "ace/Reactor.h"
#include "ace/Pipe.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/Test_and_Set.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_unistd.h"

#include <cstring>
#if defined (ACE_WIN32)
#  include "ace/Barrier.h"
#  include "ace/Thread.h"
#endif

class Ping_Pong : public ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
{
public:
  Ping_Pong (char const *b, ACE_HANDLE f);
  Ping_Pong (Ping_Pong const &) = delete;
  Ping_Pong (Ping_Pong &&) = delete;
  Ping_Pong &operator= (Ping_Pong const &) = delete;
  Ping_Pong &operator= (Ping_Pong &&) = delete;
  ACE_HANDLE get_handle () const override;
  int handle_input (ACE_HANDLE) override;
  int handle_output (ACE_HANDLE) override;
  int handle_timeout (ACE_Time_Value const &, void const *) override;
  int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask) override;
private:
  char buf_[BUFSIZ] {};
  // Buffer to send.

  size_t buflen_ {};
  // Length of the buffer to send.

  int pid_ {};
  // Process ID.

  int count_ {};
  // Number of messages sent on stream pipes.

  ACE_HANDLE handle_ {};
  // Open handle.
};

Ping_Pong::Ping_Pong (char const *b, ACE_HANDLE f)
  : buflen_ (std::strlen (b) + 1 + (2 * sizeof (int))),
    pid_ (static_cast<int> (ACE_OS::getpid ())),
    handle_ (f)
{
  std::memcpy (this->buf_, &this->pid_, sizeof (this->pid_));
  std::memcpy (this->buf_ + sizeof (this->pid_), &this->count_, sizeof (this->count_));
  std::memcpy (this->buf_ + (2 * sizeof (int)), b, std::strlen (b) + 1);
  this->buf_[this->buflen_ - 1] = '\n';
  this->buf_[this->buflen_] = '\0';
}

ACE_HANDLE
Ping_Pong::get_handle () const
{
  return this->handle_;
}

int
Ping_Pong::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  // This handler is self-owned and deletes itself when the reactor closes it.
  delete this; // Cleanup when we're removed from the reactor.
  return 0;
}

int
Ping_Pong::handle_input (ACE_HANDLE)
{
#if defined (ACE_HAS_STREAM_PIPES)
  // We can rely on record-oriented reads...

  ssize_t const n = ACE::recv (this->handle_, this->buf_, this->buflen_);

  if (n != (ssize_t) this->buflen_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) reading [%d] %p\n"),
                       handle_,
                       ACE_TEXT ("read")),
                      -1);

  int process_id {};
  int count {};
  std::memcpy (&process_id, this->buf_, sizeof (process_id));
  std::memcpy (&count, this->buf_ + sizeof (process_id), sizeof (count));
  char const *message = this->buf_ + (2 * sizeof (int));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) reading <%d> (%d) [%d] = %C\n"),
              this->handle_,
              process_id,
              count,
              message));
#else
  ssize_t n = ACE::recv (this->handle_,
                         this->buf_,
                         this->buflen_);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("[%d] %p\n"),
                       handle_,
                       ACE_TEXT ("read")),
                      -1);
  n -= (2 * sizeof (int));
  char const *buf = this->buf_ + (2 * sizeof (int));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) reading <%d> = %*C\n"),
              this->handle_,
              n,
              buf));
#endif /* ACE_HAS_STREAM_PIPES */
  return 0;
}

int
Ping_Pong::handle_output (ACE_HANDLE)
{
#if defined (ACE_HAS_STREAM_PIPES)
  // We can rely on record-oriented reads...

  ++this->count_;
  std::memcpy (this->buf_, &this->pid_, sizeof (this->pid_));
  std::memcpy (this->buf_ + sizeof (this->pid_), &this->count_, sizeof (this->count_));
  if (ACE::send (this->handle_,
                 this->buf_,
                 this->buflen_) == -1)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) writing <%d> [%d]\n"),
                  this->handle_,
                  this->pid_));
      return 0;
    }
#else
  if (ACE::send (this->handle_,
                 this->buf_,
                 this->buflen_) == -1)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) writing <%d>\n"),
                  this->handle_));
      return 0;
    }
#endif /* ACE_HAS_STREAM_PIPES */
}

int
Ping_Pong::handle_timeout (ACE_Time_Value const &, void const *)
{
  this->set (1);
  return 0;
}

// Contains the string to "pingpong" back and forth...
static ACE_TCHAR const *string_name {};

// Wait for 10 seconds and then shut down.
static ACE_Time_Value const SHUTDOWN_TIME (10);

static void
run_svc (ACE_HANDLE handle)
{
  Ping_Pong *callback {};
  // The reactor calls handle_close(), which deletes the registered handler.
  ACE_NEW (callback,
           Ping_Pong (ACE_TEXT_ALWAYS_CHAR (string_name),
                      handle));

  ACE_Reactor reactor;

  // Register the callback object for the various I/O, signal, and
  // timer-based events.

  if (reactor.register_handler (callback,
                                ACE_Event_Handler::READ_MASK
                                | ACE_Event_Handler::WRITE_MASK) == -1
      || reactor.register_handler (SIGINT,
                                   callback) == -1
      || reactor.schedule_timer (callback,
                                 nullptr,
                                 SHUTDOWN_TIME) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("reactor")));
      ACE_OS::exit (1);
    }

  // Main event loop (one per process).

  while (callback->is_set () == 0)
    if (reactor.handle_events () == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("handle_events")));
}

#if defined (ACE_WIN32)
static ACE_Barrier barrier (3);

static void *
worker (void *arg)
{
  ACE_HANDLE handle = reinterpret_cast<ACE_HANDLE> (arg);

  run_svc (handle);

  // Wait for the threads to exit.
  barrier.wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) %n: shutting down tester\n")));
  return nullptr;
}
#endif /* ACE_WIN32 */

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: pingpong <string>\n")),
                       -1);

  ACE_LOG_MSG->open (argv[0]);

  string_name = argv[1];

  ACE_HANDLE handles[2] {};

  //FUZZ: disable check_for_lack_ACE_OS
  // Create a pipe and initialize the handles.
  ACE_Pipe pipe (handles);
  //FUZZ: enable check_for_lack_ACE_OS

#if defined (ACE_WIN32)
  if (ACE_Thread::spawn (ACE_THR_FUNC (worker),
                         (void *) handles[0],
                         THR_DETACHED) == -1
      || ACE_Thread::spawn (ACE_THR_FUNC (worker),
                            (void *) handles[1],
                            THR_DETACHED) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n%a"),
                  ACE_TEXT ("spawn"),
                  1));
  barrier.wait ();
#else
  pid_t pid = ACE_OS::fork (argv[0]);

  if (pid == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n%a"),
                ACE_TEXT ("fork"),
                1));
  run_svc (handles[pid == 0]);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) %n: shutting down tester\n")));
#endif /* ACE_WIN32 */

  if (pipe.close () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("close")));
  return 0;
}
