// $Id$

#include "../test_config.h"
#include "ace/RMCast/RMCast_Proxy.h"
#include "ace/RMCast/RMCast_Reordering.h"

#include "ace/Task.h"

ACE_RCSID(tests, RMCast_Reordering_Test, "$Id$")

// ****************************************************************

class Tester;

//! Simple proxy for the ACE_RMCast_Reordering test harness
/*!
 * Implement a simple version of the ACE_RMCast_Proxy class used in
 * the ACE_RMCast_Reordering test harness.
 */
class Test_Proxy : public ACE_RMCast_Proxy
{
public:
  Test_Proxy (void);

  void set_tester (Tester *tester)
  {
    this->tester_ = tester;
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
  virtual int reply_ack_join (ACE_RMCast::Ack_Join &)
  {
    return 0;
  }
  virtual int reply_ack_leave (ACE_RMCast::Ack_Leave &)
  {
    return 0;
  }
  //! Must check that that sequence number is reasonable
  virtual int reply_ack (ACE_RMCast::Ack &);
  virtual int reply_join (ACE_RMCast::Join &)
  {
    return 0;
  }
  virtual int reply_leave (ACE_RMCast::Leave &)
  {
    return 0;
  }
  //@}

private:
  //! Keep a reference to the main testing class so it can be called
  //! back.
  Tester *tester_;
};

// ****************************************************************

//! A simple module to receive the messages from ACE_RMCast_Reordering
/*!
 * The ACE_RMCast_Reordering layer pushes messages to its next module
 * when all the members have acked a message, when a new member joins,
 * when a member leaves, etc.
 * This class will verify that the messages are exactly what we
 * expect.
 */
class Tester : public ACE_RMCast_Module
{
public:
  Tester (void);

  //! Run the test for \iterations times
  void run (int iterations);

  //! The proxy has received an Ack message, we need to validate it
  int reply_ack (ACE_RMCast::Ack &ack);

  //! Receive the data messages and validate their order
  virtual int data (ACE_RMCast::Data &data);

private:
  //! Generate a new message to drive the test
  void generate_messages (int count);

private:
  //! The array of proxies
  Test_Proxy proxy_;

  //! The Reordering layer
  ACE_RMCast_Reordering reordering_;

  //! The test is randomized to get better coverage.  This is the seed
  //! variable for the test
  ACE_RANDR_TYPE seed_;

  //! The lowest sequence number used to generate messages
  ACE_UINT32 lowest_sequence_number_;

  //! The next expected sequence number
  ACE_UINT32 next_expected_;

  //! Synchronization
  ACE_SYNCH_MUTEX mutex_;
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
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_Reordering_Test"));

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
  }

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

Tester::Tester (void)
  : seed_ (ACE_static_cast(ACE_RANDR_TYPE,ACE_OS::time (0)))
  , lowest_sequence_number_ (0)
  , next_expected_ (0)
{
  // Initialize the stack...
  this->reordering_.next (this);

  this->proxy_.set_tester (this);
}

void
Tester::run (int iterations)
{
  ACE_RMCast::Ack_Join ack_join;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
    ack_join.source = &this->proxy_;
    ack_join.next_sequence_number = this->lowest_sequence_number_;
  }
  int result = this->reordering_.ack_join (ack_join);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Reordering::ack_join returned %d\n"),
                  result));
    }

  for (int i = 0; i < iterations; ++i)
    {
      // Push data
      this->generate_messages (iterations / 10);
    }
  if (this->next_expected_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Tester::run - no messages received\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Tester::run (%t) - %d messages received\n"),
                  this->next_expected_));
    }
}

int
Tester::reply_ack (ACE_RMCast::Ack &ack)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received Ack message (%d, %d)\n"),
  //             ack.next_expected, this->lowest_sequence_number_));

  const int success_ratio = 95;
  int c = ACE_OS::rand_r (this->seed_) % 100;

  if (c < success_ratio)
    {
      this->lowest_sequence_number_ = ack.next_expected;
    }
  return 0;
}

void
Tester::generate_messages (int count)
{
  ACE_Message_Block payload (1024);
  payload.wr_ptr (1024);

  ACE_RMCast::Data data;
  data.source = &this->proxy_;
  data.payload = &payload;

  ACE_UINT32 min_sn;
  ACE_UINT32 max_sn;
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
    min_sn = this->lowest_sequence_number_;
    max_sn = this->lowest_sequence_number_ + count;
  }
  for (ACE_UINT32 i = min_sn; i != max_sn; ++i)
    {
      data.sequence_number = i;

      const int success_ratio = 95;
      int c;
      {
        ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->mutex_);
        c = ACE_OS::rand_r (this->seed_) % 100;
      }
      if (c < success_ratio)
        {
          int result = this->reordering_.data (data);
          if (result != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Reordering::data returned %d\n"),
                          result));
            }
        }
    }
}

int
Tester::data (ACE_RMCast::Data &data)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  if (this->next_expected_ != data.sequence_number)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Tester::data - ")
                  ACE_TEXT ("out of sequence message (%d != %d)\n"),
                  this->next_expected_,
                  data.sequence_number));
      return -1;
    }

  this->next_expected_++;
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
  : tester_ (0)
{
}

int
Test_Proxy::reply_ack (ACE_RMCast::Ack & ack)
{
  return this->tester_->reply_ack (ack);
}
