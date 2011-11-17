// -*- C++ -*-
//
// $Id$
//

#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Task.h"

class ORB_Killer : public ACE_Task_Base
{
public:
  /// Constructor
  ORB_Killer (CORBA::ORB_ptr orb);
  virtual int svc (void);
private:
  CORBA::ORB_var orb_;
};

/// Implement the Test::Hello interface
class test_i
  : public virtual POA_Test::IORTable_Shutdown_Race
{
public:
  /// Constructor
  test_i (ORB_Killer *k);

  // = The skeleton methods
  virtual void ping (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  ORB_Killer *killer_;
};

#include /**/ "ace/post.h"
#endif /* IOR_RACE_I_H */
