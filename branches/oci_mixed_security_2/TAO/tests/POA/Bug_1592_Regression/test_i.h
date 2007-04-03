// -*- C++ -*-

//=============================================================================
/**
 * @file test_i.h
 *
 * $Id$
 *
 * Implementation header for the "test" IDL interface for the
 * "PortableInterceptor / ServantLocator" test.
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
class test_i
  : public virtual POA_test
{
public:

  /// Constructor.
  test_i (CORBA::ORB_ptr orb);

  virtual void op (void);

  /// Shutdown the ORB.
  virtual void shutdown (void);

protected:

  /// Destructor.
  ~test_i (void);

private:

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

};

#endif  /* TEST_I_H */
