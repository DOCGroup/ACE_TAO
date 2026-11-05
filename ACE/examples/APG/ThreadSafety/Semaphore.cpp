#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"
#include "ace/Synch.h"
#include <atomic>
#include <cstring>
#include <memory>

struct Message_Block_Deleter
{
  void operator() (ACE_Message_Block *message_block) const
  {
    message_block->release ();
  }
};

using Message_Block_Owner = std::unique_ptr<ACE_Message_Block, Message_Block_Deleter>;

// Listing 2 code/ch14
class Consumer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  static constexpr int N_THREADS {5};

  Consumer (ACE_Semaphore &psema, ACE_Semaphore &csema)
    : psema_ (psema), csema_ (csema) {}
  Consumer (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer &operator= (Consumer &&) = delete;

  int svc () override
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
        ACE_Message_Block *raw_message_block {};
        if (this->getq (raw_message_block) == -1)
          return;

        Message_Block_Owner message_block (raw_message_block);
        if (message_block->msg_type () == ACE_Message_Block::MB_HANGUP)
          {
            this->shutdown ();
            return;
          }
        else
          {
            int item {};
            std::memcpy (&item, message_block->rd_ptr (), sizeof (item));
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Consumed %d\n"),
                        item));
          }
        psema_.release ();
      }
  }

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown ()
  {
  //FUZZ: enable check_for_lack_ACE_OS
    if (exit_condition_.exchange (true))
      return;

    this->msg_queue ()->deactivate ();
    csema_.release (N_THREADS);
  }

  bool is_closed () const
  {
    return exit_condition_.load ();
  }

private:
  ACE_Semaphore &psema_;
  ACE_Semaphore &csema_;
  std::atomic_bool exit_condition_ {};
};
// Listing 2
// Listing 1 code/ch14
class Producer : public ACE_Task_Base
{
public:
  static constexpr int MAX_PROD {128};

  Producer (ACE_Semaphore &psema, ACE_Semaphore &csema,
            Consumer &consumer)
    : psema_ (psema), csema_ (csema), consumer_ (consumer) {}
  Producer (Producer const &) = delete;
  Producer (Producer &&) = delete;
  Producer &operator= (Producer const &) = delete;
  Producer &operator= (Producer &&) = delete;

  int svc () override
  {
    for (int i = 0; i <= MAX_PROD; ++i)
      if (produce_item (i) == -1)
        return -1;
    return hang_up ();
  }

  int produce_item (int const item)
  {
    Message_Block_Owner message_block (
      new ACE_Message_Block (sizeof (item), ACE_Message_Block::MB_DATA));
    psema_.acquire ();
    std::memcpy (message_block->wr_ptr (), &item, sizeof (item));
    message_block->wr_ptr (sizeof (item));
    if (this->consumer_.putq (message_block.get ()) == -1)
      {
        psema_.release ();
        return -1;
      }
    message_block.release ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Produced %d\n"), item));
    csema_.release ();
    return 0;
  }

  int hang_up ()
  {
    Message_Block_Owner message_block (
      new ACE_Message_Block (0, ACE_Message_Block::MB_HANGUP));
    psema_.acquire ();
    if (this->consumer_.putq (message_block.get ()) == -1)
      {
        psema_.release ();
        return -1;
      }
    message_block.release ();
    csema_.release ();
    return 0;
  }

private:
  ACE_Semaphore &psema_;
  ACE_Semaphore &csema_;
  Consumer &consumer_;
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
