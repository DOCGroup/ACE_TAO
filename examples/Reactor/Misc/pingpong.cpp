/* Simple program that illustrates all the features of the ACE_Reactor:
// $Id$


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


#include "ace/Synch.h"
#include "ace/Reactor.h"
#include "ace/Pipe.h"

class Ping_Pong : public ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
{
public:
  Ping_Pong (char b[], ACE_HANDLE f);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &, const void *);

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
  *((int *) this->buf_) = (int) this->pid_;
  *((int *) (this->buf_ + sizeof (int))) = 0;
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
Ping_Pong::handle_input (ACE_HANDLE)
{
#if defined (ACE_HAS_STREAM_PIPES)
  // We can rely on record-oriented reads...

  ssize_t n = ACE::recv (this->handle_, this->buf_, this->buflen_);

  if (n != this->buflen_)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) reading [%d] %p\n", handle_, "read"), -1);

  ACE_DEBUG ((LM_DEBUG, 
	      "(%P|%t) reading <%d> (%d) [%d] = %s\n",
	      this->handle_, 
	      *(int *) this->buf_, 
	      *(int *) (this->buf_ + sizeof (int)), 
	      this->buf_ + (2 * sizeof (int))));
#else
  ssize_t n = ACE::recv (this->handle_, this->buf_, sizeof this->buf_);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "[%d] %p\n", handle_, "read"), -1);

  n -= (2 * sizeof (int));
  char *buf = this->buf_ + (2 * sizeof (int));

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) reading <%d> = %*s\n",
	      this->handle_, n, buf));
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
  if (ACE::send (this->handle_, this->buf_, this->buflen_) == -1)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) writing <%d> [%d]\n", 
		  this->handle_, this->pid_));
      return 0;
    }
#else
  if (ACE::send (this->handle_, this->buf_, this->buflen_) == -1)
    return -1;
  else
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) writing <%d>\n", this->handle_));
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
static char *string_name;

// Wait for 10 seconds and then shut down.
static const int SHUTDOWN_TIME = 10;

#if defined (ACE_WIN32)
static ACE_Barrier barrier (3);

static void *
worker (void *arg)
{
  ACE_HANDLE handle = (ACE_HANDLE) arg;

  run_svc (handle);
  barrier.wait ();
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %n: shutting down tester\n"));
  return 0;
}
#endif /* ACE_WIN32 */

static void
run_svc (ACE_HANDLE handle)
{
  ACE_Reactor reactor;

  Ping_Pong callback (string_name, handle);

  // Register the callback object for the various I/O, signal, and
  // timer-based events.

  if (reactor.register_handler (&callback,
				ACE_Event_Handler::READ_MASK 
				| ACE_Event_Handler::WRITE_MASK) == -1
      || reactor.register_handler (SIGINT, &callback) == -1
      || reactor.schedule_timer (&callback, 0, SHUTDOWN_TIME) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "reactor", 1));

  // Main event loop (one per process).

  while (callback.is_set () == 0)
    reactor.handle_events ();
}

int 
main (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc != 2)
    ACE_ERROR ((LM_ERROR, "usage: %n string\n%a", 1));

  string_name = argv[1];

  ACE_HANDLE handles[2];

  // Create a pipe and initialize the handles.
  ACE_Pipe pipe (handles);

#if defined (ACE_WIN32)
  if (ACE_Thread::spawn (ACE_THR_FUNC (worker), 
			 (void *) handles[0], 
			 THR_DETACHED) == -1
      || ACE_Thread::spawn (ACE_THR_FUNC (worker),
			    (void *) handles[1],
			    THR_DETACHED) == -1)
      ACE_ERROR ((LM_ERROR, "%p\n%a", "spawn", 1));

  barrier.wait ();

#else
  pid_t pid = ACE_OS::fork ();

  if (pid == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "fork", 1));

  // Resync the ACE_Log_Msg notion of process id and program name.
  ACE_LOG_MSG->sync (argv[0]); 

  run_svc (handles[pid == 0]);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %n: shutting down tester\n"));
#endif /* ACE_WIN32 */

  if (pipe.close () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "close"));

  return 0;
}
