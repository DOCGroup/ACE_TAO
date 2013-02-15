//
// $Id$
//

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
  Sleeper_i (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void delay (CORBA::Short sec);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;

};


/// Implement the Test::Sleeper interface
class Middle_i : public virtual POA_Test::Middle
{
public:
  /// Constructor
  Middle_i (CORBA::ORB_ptr orb, Test::Sleeper_ptr sleeper);

  // = The skeleton methods
  virtual void call_delay (CORBA::Short sec);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  Test::Sleeper_var sleeper_;

  ACE_Thread_Mutex lock_;
  ACE_Unbounded_Set<ACE_hthread_t> threads_;

};

#include /**/ "ace/post.h"
#endif /* TEST_I_H */
