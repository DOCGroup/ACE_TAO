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
  test_i (CORBA::ORB_ptr orb,
          PortableServer::POA_ptr poa,
          ACE_thread_t thr_id);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  /// A twoway operation.
  void method () override;

  /// A twoway operation.
  char *get_string () override;

protected:
  /// Our ORB.
  CORBA::ORB_var orb_;

  /// Our POA.
  PortableServer::POA_var poa_;
private:
  ACE_thread_t thr_id_;
};
