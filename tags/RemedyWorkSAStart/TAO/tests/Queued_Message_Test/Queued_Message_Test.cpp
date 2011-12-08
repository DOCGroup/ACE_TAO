// ============================================================================
/**
 * @brief Unit test for the TAO_Queued_Message class
 *
 * $Id$
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
#include "ace/OS_NS_stdlib.h"

/// Max number of bytes on each message block
const size_t max_block_length = 256;

static TAO_Queued_Message *
create_new_message (void)
{
  // First create a message block
  size_t block_size =
    64 + ACE_OS::rand () % (max_block_length - 64);
  ACE_Message_Block mb (block_size);
  mb.wr_ptr (block_size);

  return new TAO_Asynch_Queued_Message (&mb, TAO_ORB_Core_instance (), 0, 0, 1);
}

/// Add a new message at the tail of the queue.
static void push_back_message (TAO_Queued_Message *&head,
                               TAO_Queued_Message *&tail)
{
  TAO_Queued_Message *msg = create_new_message ();
  msg->push_back (head, tail);
}

/// Add a new message at the head of the queue.
static void push_front_message (TAO_Queued_Message *&head,
                                TAO_Queued_Message *&tail)
{
  TAO_Queued_Message *msg = create_new_message ();
  msg->push_front (head, tail);
}

/// Remove the message at the head of the queue, and simulate the
/// behavior of the I/O subsystem when processing such messages.
static void del_message (TAO_Queued_Message *&head,
                         TAO_Queued_Message *&tail)
{
  // ACE_DEBUG ((LM_DEBUG, "Removing message\n"));
  TAO_Queued_Message *current = head;
  current->remove_from_list (head, tail);

  // Simulate message writing: each message is 'sent' using
  // multiple write() calls, in this simulation, we call the
  // bytes_transferred() method until all messages are removed.

  size_t total_length = current->message_length ();
  while (total_length > 0)
    {
      // select how many bytes we want to 'send' in this iteration.
      size_t t = ACE_OS::rand () % 256 + 1;

      if (t > total_length)
        t = total_length;

      current->bytes_transferred (t);
      total_length -= t;
    }
  if (!current->all_data_sent ())
    {
      ACE_ERROR ((LM_DEBUG,
                  "ERROR: inconsistent state in Queued_Message\n"));
      ACE_OS::exit (1);
    }
  current->destroy ();
}

int
ACE_TMAIN(int, ACE_TCHAR *[])
{

  // Initialize a random seed to get better coverage.
  // @@ The random seed and default values should be configurable
  // using command line options.

  ACE_hrtime_t current_hrtime = ACE_OS::gethrtime ();
#if defined(ACE_HRTIME_T_IS_BASIC_TYPE)
  ACE_UINT32 seed = current_hrtime;
#else
  ACE_UINT32 seed =
    ACE_CU64_TO_CU32(current_hrtime);
#endif
  ACE_OS::srand (seed);

  ACE_DEBUG ((LM_DEBUG, "Running test SEED = %d\n", seed));

  TAO_Queued_Message *head = 0;
  TAO_Queued_Message *tail = 0;

  int add_count = 0;
  int del_count = 0;

  const int iterations = 100;
  int i;

  for (i = 0; i != iterations; ++i)
    {
      push_back_message (head, tail);
      add_count++;

      if (ACE_OS::rand () % 100 > 90)
        {
          // every so often remove a message also.
          if (head != 0)
            {
              del_message (head, tail);
              del_count++;
            }
        }
    }

  // second phase, change the probabilities of removing a message.
  for (i = 0; i != iterations; ++i)
    {
      if (ACE_OS::rand () % 100 > 90)
        {
          push_back_message (head, tail); add_count++;
        }
      if (ACE_OS::rand () % 100 > 90)
        {
          push_front_message (head, tail); add_count++;
        }
      if (head != 0)
        {
          del_message (head, tail);
          del_count++;
        }
    }

  // Go through a phase where all messages are removed.
  while (head != 0)
    {
      del_message (head, tail);
      del_count++;
    }

  if (tail != 0)
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
