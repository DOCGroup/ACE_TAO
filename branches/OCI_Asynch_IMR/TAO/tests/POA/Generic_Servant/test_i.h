//=============================================================================
/**
 *  @file     test_i.h
 *
 *  $Id$
 *
 *   Defines test_i class for the test interface
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

class GENERIC_SERVANT_Export test_i : public POA_test
{
public:
  /// Constructor - takes a POA and a value parameter
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);

  /// Returns the Default POA of this servant
  PortableServer::POA_ptr _default_POA (void);

  /// A twoway operation.
  void method (void);

  /// A oneway operation.
  void oneway_method (void);

  /// A timed twoway operation.
  void timed_method (CORBA::ULong timeout);

  /// A timed twoway operation.
  void timed_oneway_method (CORBA::ULong timeout);

  /// Shutdown the ORB
  void shutdown (void);

  /// Sleep for some time.
  void sleep (CORBA::ULong timeout,
              const char *operation);

protected:
  /// Our ORB.
  CORBA::ORB_var orb_;

  /// Our POA.
  PortableServer::POA_var poa_;
};
