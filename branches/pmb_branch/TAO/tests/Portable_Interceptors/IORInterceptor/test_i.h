// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * $Id$
 *
 * Implementation header for the "test" IDL interface for the
 * IORInterceptor test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class exists only to make it possible to generate an IOR to
 * which the test IOR interceptor will add a tagged component.
 */
class test_i : public virtual POA_FOO::test
{
public:

  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  /// Shutdown the ORB.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

};

#endif  /* TEST_I_H */
