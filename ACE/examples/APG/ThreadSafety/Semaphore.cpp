// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_string.h"
#include "ace/Task.h"
#include "ace/Synch.h"

// Listing 2 code/ch14
class Consumer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  enum { N_THREADS = 5 };

  Consumer (ACE_Semaphore& psema, ACE_Semaphore& csema)
    : psema_(psema), csema_(csema), exit_condition_(0)
  { }

  int svc (void)
  {
    while (!is_closed ())
      consume_item ();
    return 0;
  }

  void consume_item ()
  {
    csema_.acquire ();
    if (!is_closed ())
      {
        ACE_Message_Block *mb = 0;
        this->getq (mb);
        if (mb->msg_type () == ACE_Message_Block::MB_HANGUP)
          {
            this->shutdown ();
            mb->release ();
            return;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Consumed %d\n"),
                        *((int*)mb->rd_ptr ())));
            mb->release();
          }
        psema_.release ();
      }
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown (void)
  {
  //FUZZ: enable check_for_lack_ACE_OS
    exit_condition_ = 1;
    this->msg_queue ()->deactivate ();
    csema_.release (N_THREADS);
  }

  int is_closed (void)
  {
    return exit_condition_;
  }

private:
  ACE_Semaphore& psema_;
  ACE_Semaphore& csema_;
  int exit_condition_;
};
// Listing 2
// Listing 1 code/ch14
class Producer : public ACE_Task_Base
{
public:
  enum { MAX_PROD = 128 };

  Producer (ACE_Semaphore& psema, ACE_Semaphore& csema,
            Consumer &consumer)
      : psema_(psema), csema_(csema), consumer_(consumer)
  { }

  int svc (void)
  {
    for (int i = 0; i <= MAX_PROD; i++)
      produce_item (i);
    hang_up ();
    return 0;
  }

  void produce_item (int item)
  {
    psema_.acquire ();
    ACE_Message_Block *mb
      = new ACE_Message_Block (sizeof (int),
                               ACE_Message_Block::MB_DATA);
    ACE_OS::memcpy (mb->wr_ptr (), &item, sizeof item);
    mb->wr_ptr (sizeof (int));
    this->consumer_.putq (mb);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Produced %d\n"), item));
    csema_.release();
  }

  void hang_up ()
  {
    psema_.acquire ();
    ACE_Message_Block *mb =
      new ACE_Message_Block (0, ACE_Message_Block::MB_HANGUP);
    this->consumer_.putq (mb);
    csema_.release ();
  }

private:
  ACE_Semaphore& psema_;
  ACE_Semaphore& csema_;
  Consumer& consumer_;
};
// Listing 1
// Listing 3 code/ch14
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Semaphore psem (5);
  ACE_Semaphore csem (0);

  Consumer consumer (psem, csem);
  Producer producer (psem, csem, consumer);

  producer.activate ();
  consumer.activate (THR_NEW_LWP | THR_JOINABLE,
                     Consumer::N_THREADS);

  producer.wait ();
  consumer.wait ();

  return 0;
}
// Listing 3

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
