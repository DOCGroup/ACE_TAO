// -*- C++ -*-
#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_Collocated_ForwardRequestTest::test
{
public:
  /// Constructor.
  test_i (CORBA::Short id,
          CORBA::ORB_ptr orb);

  /// A way to setup forwarding in case of direct collocation.
  void forward (CORBA::Object_ptr to,
                CORBA::ULong request_pass_count);

  /// Return the number assigned to this object.
  virtual CORBA::Short collocated_call ();

private:
  /// Id of this instance.
  CORBA::Short id_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// A place where to forward.
  CORBA::Object_var to_;

  /// How much requests to pass thru before forwarding.
  CORBA::ULong request_pass_count_;

  /// The number of requests intercepted by this interceptor.
  CORBA::ULong request_count_;
};

#endif  /* TEST_I_H */
