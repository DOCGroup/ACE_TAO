// -*- C++ -*-

//=============================================================================
/**
 * @file    Current_Test_Impl.h
 *
 * $Id$
 *
 * Implementation header for the "test" IDL interface for the
 * TAO::TransportCurrent test.
 *
 * @author  Iliyan Jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef CURRENT_TEST_IMPL_H
#define CURRENT_TEST_IMPL_H

#include "Current_TestS.h"
#include "tao/TransportCurrent/TCC.h"

/**
 * @class Current_Test_Impl
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class Current_Test_Impl : public virtual POA_Test::Transport::CurrentTest
{
public:

  /// Constructor.
  Current_Test_Impl (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, int do_collocated_calls = 1);

  /// Destructor.
  ~Current_Test_Impl (void);

  /// Main servant test method.
  virtual void invoked_by_client (void);

  /// No-op method used so that a client request interceptor will be
  /// invoked when invoking this method from the above invoke_me()
  /// method.  Say that 10 times fast. :-)
  virtual void invoked_during_upcall (void);

  /// Shutdown the ORB.
  virtual void shutdown (void);

  // A self-test for coherency
  virtual ::CORBA::Long self_test (void);

public:
  /// Reports if unexpected errors have been encountered for the
  /// lifetime of the instance
  bool is_successful (void) const;

private:
  void test_transport_current (void);

  /// Main servant test method.
  void invoked_by_client_i (void);

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// Pseudo-reference to the POA (cache)
  PortableServer::POA_var poa_;

  /// To do or not to do ...
  int do_collocated_calls_;
};

#endif  /* CURRENT_TEST_IMPL_H */
