// -*- C++ -*-

//=============================================================================
/**
 * @file    Current_Test_Impl.h
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
  ~Current_Test_Impl () override;

  Current_Test_Impl (Current_Test_Impl const &) = delete;
  Current_Test_Impl (Current_Test_Impl &&) = delete;
  Current_Test_Impl &operator= (Current_Test_Impl const &) = delete;
  Current_Test_Impl &operator= (Current_Test_Impl &&) = delete;

  /// Main servant test method.
  void invoked_by_client () override;

  /// No-op method used so that a client request interceptor will be
  /// invoked when invoking this method from the above invoke_me()
  /// method.  Say that 10 times fast. :-)
  void invoked_during_upcall () override;

  /// Shutdown the ORB.
  void shutdown () override;

  // A self-test for coherency
  ::CORBA::Long self_test () override;

public:
  /// Reports if unexpected errors have been encountered for the
  /// lifetime of the instance
  bool is_successful () const;

private:
  void test_transport_current ();

  /// Main servant test method.
  void invoked_by_client_i ();

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// Pseudo-reference to the POA (cache)
  PortableServer::POA_var poa_;

  /// To do or not to do ...
  int do_collocated_calls_;
};

#endif  /* CURRENT_TEST_IMPL_H */
