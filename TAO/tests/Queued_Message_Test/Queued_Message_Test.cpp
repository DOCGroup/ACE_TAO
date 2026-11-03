// ============================================================================
/**
 * @brief Unit test for the TAO_Queued_Message class
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
// ============================================================================

#include "tao/Asynch_Queued_Message.h"
#include "tao/ORB_Core.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include <random>
#include <cstdlib>

/// Max number of bytes on each message block
constexpr size_t max_block_length = 256;

static TAO_Queued_Message *
create_new_message (std::mt19937 &random_generator)
{
  // First create a message block
  std::uniform_int_distribution<size_t> block_size_distribution (
    64, max_block_length - 1);
  size_t const block_size = block_size_distribution (random_generator);
  ACE_Message_Block mb (block_size);
  mb.wr_ptr (block_size);

  // The queue owns this intrusive node and releases it through destroy().
  return new TAO_Asynch_Queued_Message (&mb, TAO_ORB_Core_instance (), 0, 0, 1);
}

/// Add a new message at the tail of the queue.
static void push_back_message (TAO_Queued_Message *&head,
                               TAO_Queued_Message *&tail,
                               std::mt19937 &random_generator)
{
  TAO_Queued_Message *const msg = create_new_message (random_generator);
  msg->push_back (head, tail);
}

/// Add a new message at the head of the queue.
static void push_front_message (TAO_Queued_Message *&head,
                                TAO_Queued_Message *&tail,
                                std::mt19937 &random_generator)
{
  TAO_Queued_Message *const msg = create_new_message (random_generator);
  msg->push_front (head, tail);
}

/// Remove the message at the head of the queue, and simulate the
/// behavior of the I/O subsystem when processing such messages.
static void del_message (TAO_Queued_Message *&head,
                         TAO_Queued_Message *&tail,
                         std::mt19937 &random_generator)
{
  // ACE_DEBUG ((LM_DEBUG, "Removing message\n"));
  TAO_Queued_Message *const current = head;
  current->remove_from_list (head, tail);

  // Simulate message writing: each message is 'sent' using
  // multiple write() calls, in this simulation, we call the
  // bytes_transferred() method until all messages are removed.

  size_t total_length = current->message_length ();
  while (total_length > 0)
    {
      // select how many bytes we want to 'send' in this iteration.
      std::uniform_int_distribution<size_t> bytes_distribution (1, 256);
      size_t t = bytes_distribution (random_generator);

      if (t > total_length)
        t = total_length;

      current->bytes_transferred (t);
      total_length -= t;
    }
  if (!current->all_data_sent ())
    {
      ACE_ERROR ((LM_DEBUG,
                  "ERROR: inconsistent state in Queued_Message\n"));
      std::exit (1);
    }
  current->destroy ();
}

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  // Initialize a random seed to get better coverage.
  // @@ The random seed and default values should be configurable
  // using command line options.

  ACE_hrtime_t const current_hrtime = ACE_OS::gethrtime ();
  ACE_UINT32 const seed =
    ACE_CU64_TO_CU32(current_hrtime);
  std::mt19937 random_generator (seed);

  ACE_DEBUG ((LM_DEBUG, "Running test SEED = %u\n", seed));

  // These are non-owning links into the intrusive queue.
  TAO_Queued_Message *head = nullptr;
  TAO_Queued_Message *tail = nullptr;

  int add_count = 0;
  int del_count = 0;

  constexpr int iterations = 100;
  std::uniform_int_distribution<int> percentage_distribution (0, 99);

  for (int i = 0; i != iterations; ++i)
    {
      push_back_message (head, tail, random_generator);
      add_count++;

      if (percentage_distribution (random_generator) > 90)
        {
          // every so often remove a message also.
          if (head != nullptr)
            {
              del_message (head, tail, random_generator);
              del_count++;
            }
        }
    }

  // second phase, change the probabilities of removing a message.
  for (int i = 0; i != iterations; ++i)
    {
      if (percentage_distribution (random_generator) > 90)
        {
          push_back_message (head, tail, random_generator);
          add_count++;
        }
      if (percentage_distribution (random_generator) > 90)
        {
          push_front_message (head, tail, random_generator);
          add_count++;
        }
      if (head != nullptr)
        {
          del_message (head, tail, random_generator);
          del_count++;
        }
    }

  // Go through a phase where all messages are removed.
  while (head != nullptr)
    {
      del_message (head, tail, random_generator);
      del_count++;
    }

  if (tail != nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ERROR: inconsistent state in message queue\n"),
                        1);
    }

  if (add_count != del_count)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ERROR: mismatched (%d != %d) add and del counts\n",
                         add_count, del_count),
                        1);
    }


  return 0;
}
