// -*- C++ -*-

#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Task.h"

class ORB_Killer : public ACE_Task_Base
{
public:
  /// Constructor
  explicit ORB_Killer (CORBA::ORB_ptr orb);
  ORB_Killer (ORB_Killer const &) = delete;
  ORB_Killer &operator= (ORB_Killer const &) = delete;
  ORB_Killer (ORB_Killer &&) = delete;
  ORB_Killer &operator= (ORB_Killer &&) = delete;

  int svc () override;
private:
  CORBA::ORB_var orb_;
};

/// Implement the Test::Hello interface
class test_i
  : public virtual POA_Test::IORTable_Shutdown_Race
{
public:
  /// Constructor
  explicit test_i (ORB_Killer *killer);
  test_i (test_i const &) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i &&) = delete;

  // = The skeleton methods
  void ping () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  ORB_Killer *killer_;
};

#include /**/ "ace/post.h"
#endif /* IOR_RACE_I_H */
