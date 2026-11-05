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
#include <memory>


#if defined (ACE_HAS_THREADS)

namespace
{
  struct Message_Block_Release
  {
    void operator() (ACE_Message_Block *message_block) const
    {
      if (message_block != nullptr)
        message_block->release ();
    }
  };

  using Message_Block_Owner = std::unique_ptr<ACE_Message_Block, Message_Block_Release>;
}

using MT_Stream = ACE_Stream<ACE_MT_SYNCH>;
using MT_Module = ACE_Module<ACE_MT_SYNCH>;
using MT_Task = ACE_Task<ACE_MT_SYNCH>;

class Common_Task : public MT_Task
  // = TITLE
  //   Methods that are common to the producer and consumer.
{
public:
  Common_Task () = default;
  Common_Task (Common_Task const &) = delete;
  Common_Task (Common_Task &&) = delete;
  Common_Task &operator= (Common_Task const &) = delete;
  Common_Task &operator= (Common_Task &&) = delete;

  //FUZZ: disable check_for_lack_ACE_OS
  // ACE_Task hooks
  int open (void * = nullptr) override;
  int close (u_long = 0) override;
  //FUZZ: enable check_for_lack_ACE_OS
};

// Define the Producer interface.

class Producer : public Common_Task
{
public:
  Producer () = default;
  Producer (Producer const &) = delete;
  Producer (Producer &&) = delete;
  Producer &operator= (Producer const &) = delete;
  Producer &operator= (Producer &&) = delete;

  // Read data from stdin and pass to consumer.
  int svc () override;
};

class Consumer : public Common_Task
  // = TITLE
  //    Define the Consumer interface.
{
public:
  Consumer () = default;
  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  int put (ACE_Message_Block *mb,
           ACE_Time_Value *tv = nullptr) override;
  // Enqueue the message on the ACE_Message_Queue for subsequent
  // handling in the svc() method.

  int svc () override;
  // Receive message from producer and print to stdout.

private:
  ACE_Time_Value timeout_ {};
};

class Filter : public MT_Task
  // = TITLE
  //    Defines a Filter that prepends a line number in front of each
  //    line.
{
public:
  Filter () = default;
  Filter (Filter const &) = delete;
  Filter (Filter &&) = delete;
  Filter &operator= (Filter const &) = delete;
  Filter &operator= (Filter &&) = delete;

  int put (ACE_Message_Block *mb,
           ACE_Time_Value *tv = nullptr) override;
  // Change the size of the message before passing it downstream.

private:
  size_t count_ {1};
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
Producer::svc ()
{
  // Keep reading stdin, until we reach EOF.

  for (;;)
    {
      // Allocate a new message (add one to avoid nasty boundary
      // conditions).

      ACE_Message_Block *mb {};

      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (BUFSIZ + 1),
                      -1);
      Message_Block_Owner message {mb};

      int const n = ACE_OS::read (ACE_STDIN, mb->wr_ptr (), BUFSIZ);

      if (n <= 0)
        {
          // Send a shutdown message to the other thread and exit.
          mb->length (0);

          int const result = this->put_next (mb);
          if (result == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("put_next")));
          else
            message.release ();
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
          else
            message.release ();
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
Consumer::svc ()
{
  int result {};

  // Keep looping, reading a message out of the queue, until we
  // timeout or get a message with a length == 0, which signals us to
  // quit.

  for (;;)
    {
      ACE_Message_Block *mb {};

      // Wait for upto 4 seconds.
      this->timeout_.sec (ACE_OS::time (0) + 4);

      result = this->getq (mb, &this->timeout_);

      if (result == -1)
        break;

      Message_Block_Owner message {mb};
      int const length = ACE_Utils::truncate_cast<int> (mb->length ());

      if (length > 0)
        ACE_OS::write (ACE_STDOUT,
                       mb->rd_ptr (),
                       ACE_OS::strlen (mb->rd_ptr ()));

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
      char buf[BUFSIZ + 1] {};

      // Stash a copy of the buffer away.
      ACE_OS::strncpy (buf, mb->rd_ptr (), sizeof buf - 1);

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

  // Each module owns its task with the default M_DELETE policy.
  std::unique_ptr<Consumer> consumer_task = std::make_unique<Consumer> ();
  std::unique_ptr<MT_Module> consumer_module;
  MT_Module *cm {};

  ACE_NEW_RETURN (cm,
                  MT_Module (ACE_TEXT ("Consumer"),
                             consumer_task.get ()),
                  -1);
  consumer_task.release ();
  consumer_module.reset (cm);

  std::unique_ptr<Filter> filter_task = std::make_unique<Filter> ();
  std::unique_ptr<MT_Module> filter_module;
  MT_Module *fm {};
  ACE_NEW_RETURN (fm,
                  MT_Module (ACE_TEXT ("Filter"),
                             filter_task.get ()),
                  -1);
  filter_task.release ();
  filter_module.reset (fm);

  std::unique_ptr<Producer> producer_task = std::make_unique<Producer> ();
  std::unique_ptr<MT_Module> producer_module;
  MT_Module *pm {};
  ACE_NEW_RETURN (pm,
                  MT_Module (ACE_TEXT ("Producer"),
                             producer_task.get ()),
                  -1);
  producer_task.release ();
  producer_module.reset (pm);

  // Create Consumer, Filter, and Producer Modules and push them onto
  // the Stream.  All processing is performed in the Stream.

  int const consumer_push_result = stream.push (consumer_module.get ());
  // push() links the module before opening its task, even when it returns an error.
  consumer_module.release ();
  if (consumer_push_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push")),
                      1);
  int const filter_push_result = stream.push (filter_module.get ());
  filter_module.release ();
  if (filter_push_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("push")),
                      1);
  int const producer_push_result = stream.push (producer_module.get ());
  producer_module.release ();
  if (producer_push_result == -1)
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
