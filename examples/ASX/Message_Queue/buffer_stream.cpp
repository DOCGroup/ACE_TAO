// $Id$

// This short program copies stdin to stdout via the use of an ASX
// Stream.  It illustrates an implementation of the classic "bounded
// buffer" program using an ASX Stream containing two Modules.  Each
// ACE_Module contains two Tasks.  Each ACE_Task contains a
// ACE_Message_Queue and a pointer to a ACE_Thread_Manager.  Note how
// the use of these reusable components reduces the reliance on global
// variables, as compared with the bounded_buffer.C example.

#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Service_Config.h"
#include "ace/Stream.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Truncate.h"



#if defined (ACE_HAS_THREADS)

typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;
typedef ACE_Module<ACE_MT_SYNCH> MT_Module;
typedef ACE_Task<ACE_MT_SYNCH> MT_Task;

class Common_Task : public MT_Task
  // = TITLE
  //   Methods that are common to the producer and consumer.
{
public:
  Common_Task (void) {}

  //FUZZ: disable check_for_lack_ACE_OS
  // ACE_Task hooks
  virtual int open (void * = 0);
  virtual int close (u_long = 0);
  //FUZZ: enable check_for_lack_ACE_OS
};

// Define the Producer interface.

class Producer : public Common_Task
{
public:
  Producer (void) {}

  // Read data from stdin and pass to consumer.
  virtual int svc (void);
};

class Consumer : public Common_Task
  // = TITLE
  //    Define the Consumer interface.
{
public:
  Consumer (void) {}

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
  // Enqueue the message on the ACE_Message_Queue for subsequent
  // handling in the svc() method.

  virtual int svc (void);
  // Receive message from producer and print to stdout.

private:

  ACE_Time_Value timeout_;
};

class Filter : public MT_Task
  // = TITLE
  //    Defines a Filter that prepends a line number in front of each
  //    line.
{
public:
  Filter (void): count_ (1) {}

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
  // Change the size of the message before passing it downstream.

private:
  size_t count_;
  // Count the number of lines passing through the filter.
};

// Spawn off a new thread.

int
Common_Task::open (void *)
{
  if (this->activate (THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn")),
                      -1);
  return 0;
}

int
Common_Task::close (u_long exit_status)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) thread is exiting with status %d in module %s\n"),
              exit_status,
              this->name ()));

  // Can do anything here that is required when a thread exits, e.g.,
  // storing thread-specific information in some other storage
  // location, etc.
  return 0;
}

// The Consumer reads data from the stdin stream, creates a message,
// and then queues the message in the message list, where it is
// removed by the consumer thread.  A 0-sized message is enqueued when
// there is no more data to read.  The consumer uses this as a flag to
// know when to exit.

int
Producer::svc (void)
{
  // Keep reading stdin, until we reach EOF.

  for (int n; ; )
    {
      // Allocate a new message (add one to avoid nasty boundary
      // conditions).

      ACE_Message_Block *mb = 0;

      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (BUFSIZ + 1),
                      -1);

      n = ACE_OS::read (ACE_STDIN, mb->wr_ptr (), BUFSIZ);

      if (n <= 0)
        {
          // Send a shutdown message to the other thread and exit.
          mb->length (0);

          if (this->put_next (mb) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("put_next")));
          break;
        }

      // Send the message to the other thread.
      else
        {
          mb->wr_ptr (n);
          // NUL-terminate the string (since we use strlen() on it
          // later).
          mb->rd_ptr ()[n] = '\0';

          if (this->put_next (mb) == -1)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p\n"),
                      ACE_TEXT ("put_next")));
        }
    }

  return 0;
}

// Simply enqueue the Message_Block into the end of the queue.

int
Consumer::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// Consumer sends a 0-sized message to inform the consumer to stop
// reading and exit.

int
Consumer::svc (void)
{
  int result = 0;

  // Keep looping, reading a message out of the queue, until we
  // timeout or get a message with a length == 0, which signals us to
  // quit.

  for (;;)
    {
      ACE_Message_Block *mb = 0;

      // Wait for upto 4 seconds.
      this->timeout_.sec (ACE_OS::time (0) + 4);

      result = this->getq (mb, &this->timeout_);

      if (result == -1)
        break;

      int length = ACE_Utils::truncate_cast<int> (mb->length ());

      if (length > 0)
        ACE_OS::write (ACE_STDOUT,
                       mb->rd_ptr (),
                       ACE_OS::strlen (mb->rd_ptr ()));

      mb->release ();

      if (length == 0)
        break;
    }

  if (result == -1 && errno == EWOULDBLOCK)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) %p\n%a"),
                ACE_TEXT ("timed out waiting for message"),
                1));
  return 0;
}

int
Filter::put (ACE_Message_Block *mb,
             ACE_Time_Value *tv)
{
  if (mb->length () == 0)
    return this->put_next (mb, tv);
  else
    {
      char buf[BUFSIZ];

      // Stash a copy of the buffer away.
      ACE_OS::strncpy (buf, mb->rd_ptr (), sizeof buf);

      // Increase the size of the buffer large enough that it will be
      // reallocated (in order to test the reallocation mechanisms).

      mb->size (mb->length () + BUFSIZ);
      mb->length (mb->size ());

      // Prepend the line count in front of the buffer.
      ACE_OS::sprintf (mb->rd_ptr (),
                       ACE_SIZE_T_FORMAT_SPECIFIER
                       ": %s",
                       this->count_++,
                       buf);
      return this->put_next (mb, tv);
    }
}

// Main driver function.

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon (argv[0]);

  // This Stream controls hierachically-related active objects.
  MT_Stream stream;

  MT_Module *pm = 0;
  MT_Module *fm = 0;
  MT_Module *cm = 0;

  ACE_NEW_RETURN (cm,
                  MT_Module (ACE_TEXT ("Consumer"),
                             new Consumer),
                  -1);
  ACE_NEW_RETURN (fm,
                  MT_Module (ACE_TEXT ("Filter"),
                             new Filter),
                  -1);
  ACE_NEW_RETURN (pm,
                  MT_Module (ACE_TEXT ("Producer"),
                             new Producer),
                  -1);

  // Create Consumer, Filter, and Producer Modules and push them onto
  // the Stream.  All processing is performed in the Stream.

  if (stream.push (cm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push")),
                      1);
  else if (stream.push (fm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push")),
                      1);
  else if (stream.push (pm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push")),
                      1);
  // Barrier synchronization: wait for the threads to exit, then exit
  // ourselves.
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("threads not supported on this platform\n")));
  return 0;
}
#endif /* ACE_HAS_THREADS */
