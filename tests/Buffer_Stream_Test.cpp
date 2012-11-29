
//=============================================================================
/**
 *  @file    Buffer_Stream_Test.cpp
 *
 *  $Id$
 *
 *   This program illustrates an implementation of the classic
 *   "bounded buffer" program using an ASX STREAM containing two
 *   Modules.  Each ACE_Module contains two Tasks.  Each ACE_Task
 *   contains a ACE_Message_Queue and a pointer to a
 *   ACE_Thread_Manager.  Note how the use of these reusable
 *   components reduces the reliance on global variables.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Stream.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"



#if defined (ACE_HAS_THREADS)

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

typedef ACE_Stream<ACE_MT_SYNCH> MT_Stream;
typedef ACE_Module<ACE_MT_SYNCH> MT_Module;
typedef ACE_Task<ACE_MT_SYNCH> MT_Task;

/**
 * @class Common_Task
 *
 * @brief Methods that are common to the Supplier and consumer.
 */
class Common_Task : public MT_Task
{
public:
  Common_Task (void) {}

  //FUZZ: disable check_for_lack_ACE_OS
  // = ACE_Task hooks.
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  virtual int close (u_long = 0);
};

/**
 * @class Supplier
 *
 * @brief Define the Supplier interface.
 */
class Supplier : public Common_Task
{
public:
  Supplier (void) {}

  /// Read data from stdin and pass to consumer.
  virtual int svc (void);
};

/**
 * @class Consumer
 *
 * @brief Define the Consumer interface.
 */
class Consumer : public Common_Task
{
public:
  Consumer (void) {}

  /// Enqueue the message on the ACE_Message_Queue for subsequent
  /// handling in the svc() method.
  virtual int put (ACE_Message_Block *mb, ACE_Time_Value *tv = 0);

  /// Receive message from Supplier and print to stdout.
  virtual int svc (void);
private:

  /// Amount of time to wait for a timeout.
  ACE_Time_Value timeout_;
};

// Spawn off a new thread.

int
Common_Task::open (void *)
{
  if (this->activate (THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn")), -1);
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

// The Supplier reads data from the stdin stream, creates a message,
// and then queues the message in the message list, where it is
// removed by the consumer thread.  A 0-sized message is enqueued when
// there is no more data to read.  The consumer uses this as a flag to
// know when to exit.

int
Supplier::svc (void)
{
  ACE_Message_Block *mb = 0;

  // Send one message for each letter of the alphabet, then send an empty
  // message to mark the end.
  for (const char *c = ACE_ALPHABET; *c != '\0'; c++)
    {
      // Allocate a new message.
      char d[2];
      d[0] = *c;
      d[1] = '\0';

      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (2),
                      -1);
      ACE_OS::strcpy (mb->wr_ptr (), d);

      mb->wr_ptr (2);

      if (this->put_next (mb) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("put_next")));
    }

  ACE_NEW_RETURN(mb, ACE_Message_Block, -1);
  if (this->put_next (mb) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("put_next")));

  return 0;
}

int
Consumer::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  // Simply enqueue the Message_Block into the end of the queue.
  return this->putq (mb, tv);
}

// The consumer dequeues a message from the ACE_Message_Queue, writes
// the message to the stderr stream, and deletes the message.  The
// Consumer sends a 0-sized message to inform the consumer to stop
// reading and exit.

int
Consumer::svc (void)
{
  ACE_Message_Block *mb = 0;
  int result;
  const char *c = ACE_ALPHABET;
  char *output = 0;

  // Keep looping, reading a message out of the queue, until we
  // timeout or get a message with a length == 0, which signals us to
  // quit.

  for (;;)
    {
      this->timeout_.set (ACE_OS::time (0) + 4, 0); // Wait for upto 4 seconds

      result = this->getq (mb, &this->timeout_);

      if (result == -1)
        break;

      size_t const length = mb->length ();

      if (length > 0)
        {
          output = mb->rd_ptr ();
          ACE_TEST_ASSERT (*c == output[0]);
          c++;
        }
      mb->release ();

      if (length == 0)
        break;
    }

  ACE_TEST_ASSERT (result == 0 || errno == EWOULDBLOCK);
  return 0;
}

#endif /* ACE_HAS_THREADS */

// Main driver function.

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Buffer_Stream_Test"));

#if defined (ACE_HAS_THREADS)
  // Control hierachically-related active objects.
  MT_Stream stream;
  MT_Module *cm = 0;
  MT_Module *sm = 0;

  // Allocate the Consumer and Supplier modules.
  ACE_NEW_RETURN (cm, MT_Module (ACE_TEXT ("Consumer"), new Consumer), -1);
  ACE_NEW_RETURN (sm, MT_Module (ACE_TEXT ("Supplier"), new Supplier), -1);

  // Create Supplier and Consumer Modules and push them onto the
  // Stream.  All processing is performed in the Stream.

  if (stream.push (cm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("push")), 1);
  else if (stream.push (sm) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("push")), 1);

  // Barrier synchronization: wait for the threads to exit, then exit
  // ourselves.
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
