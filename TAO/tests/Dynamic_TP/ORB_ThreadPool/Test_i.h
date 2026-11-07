
#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Synch.h"
#include "ace/Unbounded_Set.h"

/// Implement the Test::Sleeper interface
class Sleeper_i : public virtual POA_Test::Sleeper
{
public:
  /// Constructor
  explicit Sleeper_i (CORBA::ORB_ptr orb);
  Sleeper_i (Sleeper_i const &) = delete;
  Sleeper_i (Sleeper_i &&) = delete;
  Sleeper_i &operator= (Sleeper_i const &) = delete;
  Sleeper_i &operator= (Sleeper_i &&) = delete;

  // = The skeleton methods
  void delay (CORBA::Short sec) override;

  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};


/// Implement the Test::Sleeper interface
class Middle_i : public virtual POA_Test::Middle
{
public:
  /// Constructor
  Middle_i (CORBA::ORB_ptr orb, Test::Sleeper_ptr sleeper);
  Middle_i (Middle_i const &) = delete;
  Middle_i (Middle_i &&) = delete;
  Middle_i &operator= (Middle_i const &) = delete;
  Middle_i &operator= (Middle_i &&) = delete;

  // = The skeleton methods
  void call_delay (CORBA::Short sec) override;

  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
  Test::Sleeper_var const sleeper_;

  TAO_SYNCH_MUTEX lock_;
  ACE_Unbounded_Set<ACE_hthread_t> threads_;
};

#include /**/ "ace/post.h"
#endif /* TEST_I_H */
