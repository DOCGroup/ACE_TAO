// $Id$
//=============================================================================
//
//
// = FILENAME
//     test_i.h
//
// = DESCRIPTION
//     Defines test_i class for the test interface
//
// = AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#include "testS.h"

class GENERIC_SERVANT_Export test_i : public POA_test
{
public:
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa);
  // Constructor - takes a POA and a value parameter

  PortableServer::POA_ptr _default_POA (void);
  // Returns the Default POA of this servant

  void method (void);
  // A twoway operation.

  void oneway_method (void);
  // A oneway operation.

  void timed_method (CORBA::ULong timeout);
  // A timed twoway operation.

  void timed_oneway_method (CORBA::ULong timeout);
  // A timed twoway operation.

  void shutdown (void);
  // Shutdown the ORB

  void sleep (CORBA::ULong timeout,
              const char *operation);
  // Sleep for some time.

protected:
  CORBA::ORB_var orb_;
  // Our ORB.

  PortableServer::POA_var poa_;
  // Our POA.
};
