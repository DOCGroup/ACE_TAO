// ============================================================================
/**
 * @brief Unit test for the TAO_Queued_Message class
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
// ============================================================================

#include "tao/Queued_Message.h"

ACE_RCSID(tests, Queued_Message_Test, "$Id$")

/// Max number of bytes on each message block
const size_t max_block_length = 256;

/// Create a message block chain of at most 64 elements
const size_t max_chain_length = 64;

/// Add a new message at the tail of the queue.
static void add_message (TAO_Queued_Message *&head,
                         TAO_Queued_Message *&tail,
                         ACE_RANDR_TYPE &seed)
{
  // ACE_DEBUG ((LM_DEBUG, "Adding message\n"));
  // First build a message block chain
  size_t chain_length =
    1 + ACE_OS::rand_r(seed) % max_chain_length;
  ACE_Message_Block *mb = 0;
  for (size_t j = 0; j != chain_length; ++j)
    {
      size_t block_size =
        64 + ACE_OS::rand_r(seed) % (max_block_length - 64);
      ACE_Message_Block *cont = mb;
      mb = new ACE_Message_Block (block_size);
      mb->wr_ptr (block_size);
      mb->cont (cont);
    }
  TAO_Queued_Message *msg =
    new TAO_Queued_Message (mb, 1);
  msg->push_back (head, tail);
}

/// Remove the message at the head of the queue, and simulate the
/// behavior of the I/O subsystem when processing such messages.
static void del_message (TAO_Queued_Message *&head,
                         TAO_Queued_Message *&tail,
                         ACE_RANDR_TYPE &seed)
{
  // ACE_DEBUG ((LM_DEBUG, "Removing message\n"));
  TAO_Queued_Message *current = head;
  current->remove_from_list (head, tail);

  // Simulate message writing: each message is 'sent' using
  // multiple write() calls, in this simulation, we call the
  // bytes_transferred() method until all messages are removed.

  size_t total_length = current->mb ()->total_length ();
  while (total_length > 0)
    {
      // select how many bytes we want to 'send' in this iteration.
      size_t t = ACE_OS::rand_r(seed) % 256 + 1;
      if (t > total_length)
        t = total_length;

      current->bytes_transferred (t);
      total_length -= t;
    }
  if (!current->done ())
    {
      ACE_ERROR ((LM_DEBUG,
                  "ERROR: inconsistent state in Queued_Message\n"));
      ACE_OS::exit (1);
    }
  current->destroy ();
}

int
main (int, ACE_TCHAR *[])
{

  // Initialize a random seed to get better coverage.
  // @@ The random seed and default values should be configurable
  // using command line options.

  ACE_hrtime_t current_hrtime = ACE_OS::gethrtime ();
  ACE_UINT32 low_bits =
    ACE_CU64_TO_CU32(current_hrtime);
  ACE_RANDR_TYPE seed =
    ACE_static_cast(ACE_RANDR_TYPE,low_bits);

  ACE_DEBUG ((LM_DEBUG, "Running test SEED = %d\n", seed));

  TAO_Queued_Message *head = 0;
  TAO_Queued_Message *tail = 0;

  int add_count = 0;
  int del_count = 0;

  const int iterations = 100;
  int i;
  for (i = 0; i != iterations; ++i)
    {
      add_message (head, tail, seed);
      add_count++;
      if (ACE_OS::rand_r(seed) % 100 > 90)
        {
          // every so often remove a message also.
          if (head != 0)
            {
              del_message (head, tail, seed);
              del_count++;
            }
        }
    }

  // second phase, change the probabilities of removing a message.
  for (i = 0; i != iterations; ++i)
    {
      if (ACE_OS::rand_r(seed) % 100 > 90)
        {
          add_message (head, tail, seed); add_count++;
        }
      if (head != 0)
        {
          del_message (head, tail, seed);
          del_count++;
        }
    }

  // Go through a phase where all messages are removed.
  while (head != 0)
    {
      del_message (head, tail, seed);
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
