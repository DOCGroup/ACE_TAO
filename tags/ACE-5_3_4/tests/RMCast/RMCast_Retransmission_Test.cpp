// $Id$

#include "../test_config.h"
#include "ace/RMCast/RMCast_Proxy.h"
#include "ace/RMCast/RMCast_Retransmission.h"

#include "ace/Task.h"

ACE_RCSID(tests, RMCast_Retransmission_Test, "$Id$")

// ****************************************************************

class Tester;

//! Simple proxy for the ACE_RMCast_Retransmission test harness
/*!
 * Implement a simple version of the ACE_RMCast_Proxy class used in
 * the ACE_RMCast_Retransmission test harness.
 */
class Test_Proxy : public ACE_RMCast_Proxy
{
public:
  Test_Proxy (void);

  void set_tester (Tester *tester)
  {
    this->tester_ = tester;
  }

  //! Get the flag to remember if this proxy has joined the group or
  //! not.
  int joined (void) const
  {
    return this->joined_;
  }

  //! Set the flag to remember if this proxy has joined the group or
  //! not.
  void joined (int j)
  {
    this->joined_ = j;
  }

  //! Most of the reply_ methods just return 0, there is no real remote
  //! peer, this is just a test harness
  //@{
  virtual int reply_data (ACE_RMCast::Data &)
  {
    return 0;
  }
  virtual int reply_poll (ACE_RMCast::Poll &)
  {
    return 0;
  }
  //! Must check that that sequence number is reasonable
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &);
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &)
  {
    return 0;
  }
  virtual int reply_ack (ACE_RMCast::Ack &)
  {
    return 0;
  }
  virtual int reply_join (ACE_RMCast::Join &)
  {
    return 0;
  }
  virtual int reply_leave (ACE_RMCast::Leave &)
  {
    return 0;
  }
  //@}

  int data (ACE_RMCast::Data &data);
  int ack (ACE_RMCast::Ack &ack);

private:
  //! Remember if we joined the group already.
  int joined_;

  //! Keep a reference to the main testing class so it can be called
  //! back.
  Tester *tester_;

  //! The test is randomized to get better coverage.  This is the seed
  //! variable for the test
  ACE_RANDR_TYPE seed_;

  //! Synchronize internal data structures.
  ACE_SYNCH_MUTEX lock_;
};

// ****************************************************************

//! The number of proxies used in the test
/*!
 * Not all member will be present in the group at the same time. But
 * this variable controls the maximum number
 */
const size_t nproxy = 16;
//! A simple module to receive the messages from ACE_RMCast_Retransmission
/*!
 * The ACE_RMCast_Retransmission layer pushes messages to its next module
 * when all the members have acked a message, when a new member joins,
 * when a member leaves, etc.
 * This class will verify that the messages are exactly what we
 * expect.
 */
class Tester : public ACE_RMCast_Module
{
public:
  Tester (void);

  //! Run the test for \param iterations times
  void run (int iterations);

  //! Validate the number of messages received by each proxy
  void validate_message_count (void);

  //! One of the proxies has received an Ack_Join message, we need to
  //! validate it
  int reply_ack_join (Test_Proxy *proxy, ACE_RMCast::Ack_Join &ack_join);

  //! One of the proxies has received an Ack message.
  /*!
   * In this method we simulate the role of the Membership layer,
   * however we just do brute force instead of trying to optimize the
   * Ack processing
   */
  int proxy_received_ack (Test_Proxy *proxy, ACE_RMCast::Ack &ack);

  virtual int data (ACE_RMCast::Data &data);

private:
  //! Generate a new message to drive the test
  void generate_one_message (void);

  //! Send a single Ack that summarizes the state of all the proxies.
  int send_ack (void);

  //! Ask the retransmission layer to resend lost messages
  int resend (void);

private:
  //! The array of proxies
  Test_Proxy proxy_[nproxy];

  //! The Retransmission layer
  ACE_RMCast_Retransmission retransmission_;

  //! Synchronize internal data structures
  ACE_SYNCH_MUTEX lock_;

  //! The test is randomized to get better coverage.  This is the seed
  //! variable for the test
  ACE_RANDR_TYPE seed_;

  //! Generate sequence numbers for the Retransmission layer
  ACE_UINT32 sequence_number_generator_;
};

// ****************************************************************

//! An Adapter to run Tester::run the test is a separate thread
class Task : public ACE_Task_Base
{
public:
  Task (Tester *tester);

  // = Read the documentation in "ace/Task.h"
  int svc (void);

private:
  //! The tester object.
  Tester *tester_;
};

// ****************************************************************

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_Retransmission_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Running single threaded test\n")));
    //! Run the test in single threaded mode
    Tester tester;
    tester.run (100);
    tester.validate_message_count ();
  }
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Running multi threaded test\n")));
    //! Run the test in multi-threaded mode
    Tester tester;
    Task task (&tester);
    if (task.activate (THR_NEW_LWP|THR_JOINABLE, 4) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Cannot activate the threads\n")),
                        1);
    ACE_Thread_Manager::instance ()->wait ();
    tester.validate_message_count ();
  }

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

Tester::Tester (void)
  : seed_ (ACE_static_cast(ACE_RANDR_TYPE,ACE_OS::time (0)))
  , sequence_number_generator_ (0)
{
  // Initialize the stack...
  this->retransmission_.next (this);

  for (size_t i = 0; i != nproxy; ++i)
    {
      this->proxy_[i].set_tester (this);
      this->proxy_[i].joined (1);
    }
}

void
Tester::run (int iterations)
{
  for (int i = 0; i < iterations; ++i)
    {
      // Push data
      this->generate_one_message ();

      this->send_ack ();

      // Re-send unacked data
      if (i % 5 == 0)
        {
          this->resend ();
          this->send_ack ();
        }
    }

  while (this->resend () != 0)
    {
      this->send_ack ();
    }
}

void
Tester::validate_message_count (void)
{
  for (size_t i = 0; i != nproxy; ++i)
    {
      if (this->proxy_[i].joined () == 0)
        continue;
      if (this->proxy_[i].next_expected () != this->sequence_number_generator_)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Invalid message count for proxy %d, ")
                      ACE_TEXT ("it is %d, should be %d\n"),
                      i, this->proxy_[i].next_expected (),
                      this->sequence_number_generator_));
        }
    }
}

int
Tester::reply_ack_join (Test_Proxy *, ACE_RMCast::Ack_Join &ack_join)
{
  if (ack_join.next_sequence_number > this->sequence_number_generator_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Unexpected sequence number in ack_join ")
                  ACE_TEXT ("(%d,%d)\n"),
                  ack_join.next_sequence_number,
                  this->sequence_number_generator_));
      return -1;
    }
  return 0;
}

int
Tester::proxy_received_ack (Test_Proxy *, ACE_RMCast::Ack &)
{
  return this->send_ack ();
}

int
Tester::send_ack ()
{
  ACE_RMCast::Ack ack;

  int set = 0;
  for (size_t i = 0; i != nproxy; ++i)
    {
      if (this->proxy_[i].joined () == 0)
        continue;
      if (!set)
        {
          ack.next_expected = this->proxy_[i].next_expected ();
          ack.highest_received = this->proxy_[i].highest_received ();
          set = 1;
        }
      else
        {
          if (ack.next_expected > this->proxy_[i].next_expected ())
            ack.next_expected = this->proxy_[i].next_expected ();
          if (ack.highest_received < this->proxy_[i].highest_received ())
            ack.highest_received = this->proxy_[i].highest_received ();
        }
    }
  if (!set)
    return 0;
  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Tested::ack - (%d,%d)\n"),
  //             ack.next_expected, ack.highest_received));
  return this->retransmission_.ack (ack);
}

void
Tester::generate_one_message (void)
{
  ACE_Message_Block payload (1024);
  payload.wr_ptr (1024);

  ACE_RMCast::Data data;
  data.payload = &payload;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
    data.sequence_number = this->sequence_number_generator_++;
  }

  int result = this->retransmission_.data (data);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Retransmission::data returned %d\n"),
                  result));
    }
}

int
Tester::resend (void)
{
  ACE_UINT32 max_sequence_number =
    this->sequence_number_generator_;
  int r = this->retransmission_.resend (max_sequence_number);
  if (r == -1)
    {
      // ACE_DEBUG ((LM_DEBUG,
      //             ACE_TEXT ("Error returned from Retransmission::resend\n")));
    }
  return r;
}

int
Tester::data (ACE_RMCast::Data &data)
{
  // After going through the Retransmission layer we got some data,
  // simulate the work of the following layers:
  //  - Fragmentation: setting message sequence numbers
  //  - IO_XXX: send to all known members
  //  - Reassembly: reconstruct the message on the receiving side.

  data.total_size = 1024;
  data.fragment_offset = 0;

  for (size_t i = 0; i != nproxy; ++i)
    {
      int result = this->proxy_[i].data (data);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Proxy::data returned %d for proxy %d\n"),
                      result, i));
          return -1;
        }
    }
  return 0;
}

// ****************************************************************

Task::Task (Tester *tester)
  :  tester_ (tester)
{
}

int
Task::svc (void)
{
  this->tester_->run (100);
  return 0;
}

// ****************************************************************

Test_Proxy::Test_Proxy (void)
  : joined_ (0)
  , tester_ (0)
  , seed_ (ACE_static_cast(ACE_RANDR_TYPE,ACE_OS::time (0)))
{
}

int
Test_Proxy::data (ACE_RMCast::Data &data)
{
  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) Proxy receives message %d\n"),
  //             data.sequence_number));
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  int c = ACE_OS::rand_r (this->seed_) % 100;

  const int success_ratio = 95;

  if (this->next_expected () > data.sequence_number)
    {
      // An old message, resend the ack...
      ACE_RMCast::Ack ack;
      ack.next_expected = this->next_expected ();
      ack.highest_received = this->highest_received ();
      // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("....it is an already accepted message\n")));
      // Ack the message....
      return this->ack (ack);
    }

  if (this->next_expected () == data.sequence_number)
    {
      // The message is the next one that we expected...
      // Two choices: accept the message as successfully received or
      // reject it, we accept them most of the time.
      if (c > success_ratio)
        {
          // We ignore the message completely as if it was lost in the
          // network
          // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("....and drops it\n")));
          return 0;
        }

      ACE_RMCast::Ack ack;
      ack.next_expected = data.sequence_number + 1;
      if (ack.next_expected < this->highest_received ())
        {
          ack.highest_received = this->highest_received ();
        }
      else
        {
          ack.highest_received = data.sequence_number;
        }
      // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("....and accepts it\n")));
      // Ack the message....
      return this->ack (ack);
    }

  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("....the message is out of order\n")));
  // This is an out of sequence number, maybe it is lost...
  if (c > success_ratio)
    {
      // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("........and is dropped\n")));
      return 0;
    }

  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("........and is accepted\n")));
  ACE_RMCast::Ack ack;
  ack.next_expected = this->next_expected ();
  if (data.sequence_number < this->highest_received ())
    {
      ack.highest_received = this->highest_received ();
    }
  else
    {
      ack.highest_received = data.sequence_number;
    }
  // Ack the message....
  return this->ack (ack);
}

int
Test_Proxy::ack (ACE_RMCast::Ack &ack)
{
  (void) this->ACE_RMCast_Proxy::ack (ack);
  (void) this->tester_->proxy_received_ack (this, ack);
  return 0;
}

int
Test_Proxy::reply_ack_join (ACE_RMCast::Ack_Join & ack_join)
{
  int r = this->tester_->reply_ack_join (this, ack_join);
  if (r == 0)
    (void) this->ACE_RMCast_Proxy::reply_ack_join (ack_join);
  return r;
}
