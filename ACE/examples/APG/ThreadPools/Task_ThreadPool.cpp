// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/SString.h"

// Listing 2 code/ch16
class Workers : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Workers ()
  { }

  virtual int svc (void)
  {
    while (1)
      {
        ACE_Message_Block *mb = 0;
        if (this->getq (mb) == -1)
          {
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%t) Shutting down\n")));
            break;
          }

        // Process the message.
        process_message (mb);
      }

    return 0;
  }
  // Listing 2

private:
  void process_message (ACE_Message_Block *mb)
  {
    ACE_TRACE (ACE_TEXT ("Workers::process_message"));
    int msgId;
    ACE_OS::memcpy (&msgId, mb->rd_ptr (), sizeof(int));
    mb->release ();

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Started processing message %d\n"),
                msgId));
    ACE_OS::sleep (3);
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Finished processing message %d\n"),
                msgId));
  }
};

// Listing 1 code/ch16
class Manager : public ACE_Task<ACE_MT_SYNCH>
{
public:
  enum {POOL_SIZE = 5, MAX_TIMEOUT = 5};

  Manager () : shutdown_(0)
  {
    ACE_TRACE (ACE_TEXT ("Manager::Manager"));
  }

  int svc (void)
  {
    ACE_TRACE (ACE_TEXT ("Manager::svc"));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) Manager started\n")));

    // Create pool.
    Workers pool;
    pool.activate (THR_NEW_LWP | THR_JOINABLE, POOL_SIZE);

    while (!done ())
      {
        ACE_Message_Block *mb = 0;
        ACE_Time_Value tv ((long)MAX_TIMEOUT);
        tv += ACE_OS::time (0);

        // Get a message request.
        if (this->getq (mb, &tv) < 0)
          {
            pool.msg_queue ()->deactivate ();
            pool.wait ();
            break;
          }

        // Ask the worker pool to do the job.
        pool.putq (mb);
      }

    return 0;
  }

private:
  int done (void);

  int shutdown_;
};
// Listing 1

int Manager::done (void)
{
  return (shutdown_ == 1);
}


int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Manager tp;
  tp.activate ();

  // Wait for a moment every time you send a message.
  ACE_Time_Value tv;
  tv.msec (100);

  ACE_Message_Block *mb = 0;
  for (int i = 0; i < 30; i++)
    {
      ACE_NEW_RETURN
        (mb, ACE_Message_Block(sizeof(int)), -1);

      ACE_OS::memcpy (mb->wr_ptr (), &i, sizeof(int));

      ACE_OS::sleep (tv);

      // Add a new work item.
      tp.putq (mb);
    }

  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
