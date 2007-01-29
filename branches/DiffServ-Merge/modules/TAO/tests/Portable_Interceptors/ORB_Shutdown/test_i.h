// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * $Id$
 *
 * Implementation header for the "test" IDL interface for the
 * PortableInterceptor ORB::shutdown() test.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
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
 * This class implements the "test" interface used in this test.
 */
class test_i : public virtual POA_test
{
public:

  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  /// Destructor.
  ~test_i (void);

  /// Say hello.
  virtual void hello (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Shutdown the ORB.
  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

};

#endif  /* TEST_I_H */
