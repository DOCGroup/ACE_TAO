// $Id$

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
#include "ace/OS_NS_string.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_unistd.h"
#if defined (ACE_WIN32) || defined (CHORUS)
#  include "ace/Barrier.h"
#  include "ace/Thread.h"
#endif



class Ping_Pong : public ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
{
public:
  Ping_Pong (char b[], ACE_HANDLE f);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
private:
  char buf_[BUFSIZ];
  // Buffer to send.

  size_t buflen_;
  // Length of the buffer to send.

  int pid_;
  // Process ID.

  ACE_HANDLE handle_;
  // Open handle.
};

Ping_Pong::Ping_Pong (char b[], ACE_HANDLE f)
  : buflen_ (ACE_OS::strlen (b) + 1 + (2 * sizeof (int))),
    pid_ (ACE_OS::getpid ()),
    handle_ (f)
{
  int *pi_buf = (int *) this->buf_;
  *(pi_buf) = (int) this->pid_;
  pi_buf = (int *) (this->buf_ + sizeof (int));
  *(pi_buf) = 0;
  ACE_OS::strcpy (this->buf_ + (2 * sizeof (int)), b);
  this->buf_[this->buflen_ - 1] = '\n';
  this->buf_[this->buflen_] = '\0';
}

ACE_HANDLE
Ping_Pong::get_handle (void) const
{
  return this->handle_;
}

int
Ping_Pong::handle_close (ACE_HANDLE,
                         ACE_Reactor_Mask)
{
  delete this; // Cleanup when we're removed from the reactor.
  return 0;
}

int
Ping_Pong::handle_input (ACE_HANDLE)
{
#if defined (ACE_HAS_STREAM_PIPES)
  // We can rely on record-oriented reads...

  ssize_t n = ACE::recv (this->handle_, this->buf_, this->buflen_);

  if (n != (ssize_t) this->buflen_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) reading [%d] %p\n"),
                       handle_,
                       ACE_TEXT ("read")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) reading <%d> (%d) [%d] = %C\n"),
              this->handle_,
              *(int *) this->buf_,
              *(int *) (this->buf_ + sizeof (int)),
              this->buf_ + (2 * sizeof (int))));
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
  char *buf = this->buf_ + (2 * sizeof (int));

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

  (*(int *) (this->buf_)) = this->pid_;
  (*(int *) (this->buf_ + sizeof (int)))++;
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
Ping_Pong::handle_timeout (const ACE_Time_Value &,
                           const void *)
{
  this->set (1);
  return 0;
}

// Contains the string to "pingpong" back and forth...
static ACE_TCHAR *string_name;

// Wait for 10 seconds and then shut down.
static const ACE_Time_Value SHUTDOWN_TIME (10);

static void
run_svc (ACE_HANDLE handle)
{
  Ping_Pong *callback = 0;
  ACE_NEW (callback,
           Ping_Pong (ACE_TEXT_ALWAYS_CHAR (string_name),
                      handle));

  ACE_Reactor reactor;

  // Register the callback object for the various I/O, signal, and
  // timer-based events.

  if (reactor.register_handler (callback,
                                ACE_Event_Handler::READ_MASK
                                | ACE_Event_Handler::WRITE_MASK) == -1
#if !defined (CHORUS)
      || reactor.register_handler (SIGINT,
                                   callback) == -1
#endif /* CHORUS */
      || reactor.schedule_timer (callback,
                                 0,
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

#if defined (ACE_WIN32) || defined (CHORUS)
static ACE_Barrier barrier (3);

static void *
worker (void *arg)
{
  ACE_HANDLE handle = (ACE_HANDLE) arg;

  run_svc (handle);

  // Wait for the threads to exit.
  barrier.wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) %n: shutting down tester\n")));
  return 0;
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

  ACE_HANDLE handles[2];

  //FUZZ: disable check_for_lack_ACE_OS
  // Create a pipe and initialize the handles.
  ACE_Pipe pipe (handles);
  //FUZZ: enable check_for_lack_ACE_OS

#if defined (ACE_WIN32) || defined (CHORUS)
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
