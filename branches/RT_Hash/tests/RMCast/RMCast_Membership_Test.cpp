// $Id$

// ============================================================================
//
// = DESCRIPTION
//    Unit test for the UDP sending module of the RMCast library.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#include "../test_config.h"
#include "ace/RMCast/RMCast_Proxy.h"
#include "ace/RMCast/RMCast_Membership.h"

#include "ace/Task.h"

ACE_RCSID(tests, RMCast_Membership_Test, "$Id$")

// ****************************************************************

//! Simple proxy for the ACE_RMCast_Membership test harness
/*!
 * Implement a simple version of the ACE_RMCast_Proxy class used in
 * the ACE_RMCast_Membership test harness.
 */
class Test_Proxy : public ACE_RMCast_Proxy
{
public:
  Test_Proxy (void);

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

  //@{
  //! All the reply_ methods just return 0, there is no real remote
  //! peer, this is just a test harness
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

private:
  //! Remember if we joined the group already.
  int joined_;
};

// ****************************************************************

//! The number of proxies used in the test
/*!
 * Not all member will be present in the group at the same time. But
 * this variable controls the maximum number
 */
const size_t nproxy = 16;

//! A simple module to receive the messages from ACE_RMCast_Membership
/*!
 * The ACE_RMCast_Membership layer pushes messages to its next module
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

  virtual int join (ACE_RMCast::Join &join);
  virtual int leave (ACE_RMCast::Leave &leave);
  virtual int ack (ACE_RMCast::Ack &ack);

private:
  //! Add a few proxies to the group
  void join_random (void);

  //! Remove a few proxies from the group
  void leave_random (void);

  //! Generate a few ack messages from all the proxies currently in
  //! the group
  void generate_acks (int iterations);

private:
  //! The array of proxies
  Test_Proxy proxy_[nproxy];

  //! The Membership layer
  ACE_RMCast_Membership membership_;

  //! Synchronize internal data structures
  ACE_SYNCH_MUTEX lock_;

  //! The test is randomized to get better coverage.  This is the seed
  //! variable for the test
  ACE_RANDR_TYPE seed_;
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
  ACE_START_TEST (ACE_TEXT ("RMCast_Membership_Test"));

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

Test_Proxy::Test_Proxy (void)
  :  joined_ (0)
{
}

// ****************************************************************

Tester::Tester (void)
  : seed_ (ACE_static_cast(ACE_RANDR_TYPE,ACE_OS::time (0)))
{
  // Initialize the stack...
  this->membership_.next (this);
  for (size_t i = 0; i != nproxy; ++i)
    this->proxy_[i].next (&this->membership_);
}

void
Tester::run (int iterations)
{
  for (int i = 0; i < iterations; ++i)
    {
      // Connect a few....
      this->join_random ();

      // Push acks....
      this->generate_acks (iterations);

      // Disconnect a few
      this->leave_random ();

      // Push acks...
      this->generate_acks (iterations / 10);
    }
}

int
Tester::join (ACE_RMCast::Join &join)
{
  if (join.source == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Invalid join message in Tester\n")),
                        -1);
    }
  for (size_t i = 0; i != nproxy; ++i)
    {
      if (&this->proxy_[i] != join.source)
        continue;
      if (this->proxy_[i].joined () != 1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Invalid state for proxy %d ")
                           ACE_TEXT ("in Tester::join\n")),
                          -1);
      return 0;
    }
  // Not found
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Unknown proxy in Tester::join\n")),
                    -1);
}

int
Tester::leave (ACE_RMCast::Leave &leave)
{
  if (leave.source == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Invalid leave message in Tester\n")),
                        -1);
    }
  for (size_t i = 0; i != nproxy; ++i)
    {
      if (&this->proxy_[i] != leave.source)
        continue;
      if (this->proxy_[i].joined () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Invalid state for proxy %d ")
                           ACE_TEXT ("in Tester::leave\n")),
                          -1);
      return 0;
    }
  // Not found
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("Unknown proxy in Tester::leave\n")),
                    -1);
}

int
Tester::ack (ACE_RMCast::Ack &ack)
{
  // After the membership layer the source makes no sense....
  if (ack.source == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Invalid ack message in Tester\n")),
                        -1);
    }

  //  ACE_DEBUG ((LM_DEBUG,
  //              ACE_TEXT ("Received ack in Tester %d,%d\n"),
  //              ack.next_expected,
  //              ack.highest_received));

  // Assume the lock is held, verify that the ack message satisfy the
  // invariants...
  ACE_UINT32 next_expected = 0;
  ACE_UINT32 highest_received = 0;
  int set = 0;
  for (size_t i = 0; i != nproxy; ++i)
    {
      if (!this->proxy_[i].joined ())
        continue;
      if (!set)
        {
          set = 1;
          next_expected = this->proxy_[i].next_expected ();
          highest_received = this->proxy_[i].highest_received ();
        }
      else
        {
          if (next_expected >
              this->proxy_[i].next_expected ())
            {
              next_expected =
                this->proxy_[i].next_expected ();
            }
          if (highest_received <
              this->proxy_[i].highest_received ())
            {
              highest_received =
                this->proxy_[i].highest_received ();
            }
        }
    }
  // No local proxy just return...
  if (!set)
    return 0;

  // Check the invariants
  if (ack.next_expected != next_expected)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Invalid next_expected in Ack\n")),
                        -1);
    }
  if (ack.highest_received != highest_received)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Invalid highest_received in Ack\n")),
                        -1);
    }
  return 0;
}

void
Tester::join_random (void)
{
  for (size_t i = 0; i != nproxy; ++i)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
      int r = ACE_OS::rand_r (this->seed_) % 100;
      if (this->proxy_[i].joined () == 0 && r > 25)
        {
          this->proxy_[i].joined (1);

          ACE_RMCast::Join join;
          join.source = &this->proxy_[i];
          //          ACE_DEBUG ((LM_DEBUG,
          //                      ACE_TEXT ("Sending join mesage for proxy %d\n"),
          //                      i));
          this->proxy_[i].join (join);
        }
    }
}

void
Tester::leave_random (void)
{
  for (size_t i = 0; i != nproxy; ++i)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
      int r = ACE_OS::rand_r (this->seed_) % 100;
      if (this->proxy_[i].joined () == 1 && r > 75)
        {
          this->proxy_[i].joined (0);

          ACE_RMCast::Leave leave;
          leave.source = &this->proxy_[i];
          //          ACE_DEBUG ((LM_DEBUG,
          //                      ACE_TEXT ("Sending leave mesage for proxy %d\n"),
          //                      i));
          this->proxy_[i].leave (leave);
        }
    }
}

void
Tester::generate_acks (int iterations)
{
  int n = 0;
  for (size_t i = 0; n < iterations && i != nproxy; ++i, ++n)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
      int r = ACE_OS::rand_r (this->seed_) % 10;
      if (this->proxy_[i].joined () == 0)
        continue;

      ACE_RMCast::Ack ack;
      ack.source = &this->proxy_[i];
      ack.next_expected =
        this->proxy_[i].next_expected ();
      ack.highest_received =
        this->proxy_[i].highest_received ();

      // we randomly perform one of 3 actions, with different
      // probabilities
      switch (r)
        {
        case 0:
          // Ack the same data that we already have:
          break;
        case 1:
        case 2:
        case 3:
        case 4:
          // Simulate out of sequence messages...
          ack.highest_received++;
          break;
        default:
          if (ack.highest_received > ack.next_expected)
            ack.next_expected++;
          break;
        }
      //      ACE_DEBUG ((LM_DEBUG,
      //                  ACE_TEXT ("Sending ack message (%d,%d) through proxy %d\n"),
      //                  ack.next_expected,
      //                  ack.highest_received,
      //                  i));
      int result = this->proxy_[i].ack (ack);
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Returned %d in proxy %d\n"),
                      result, i));
        }
    }
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
