//=============================================================================
/**
 *  @file     test_i.h
 *
 *   Defines test_i class for the test interface
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"

class test_i : public POA_test
{
public:
  /// Constructor - takes a POA and a value parameter
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          ACE_thread_t thr_id);

  /// A twoway operation.
  void method ();

  /// A twoway operation.
  char *
  get_string ();

protected:
  /// Our ORB.
  CORBA::ORB_var orb_;

  /// Our POA.
  PortableServer::POA_var poa_;
private:
  ACE_thread_t thr_id_;
};
