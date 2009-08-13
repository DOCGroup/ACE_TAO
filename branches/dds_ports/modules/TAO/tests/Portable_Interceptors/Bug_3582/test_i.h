// -*- C++ -*-

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"
#include "tao/PortableServer/POAC.h"

/**
 * @class test_i
 *
 * @brief Simple test class.
 *
 * This class implements the "test" interface used in this test.
 *
 * $Id$
 */
class test_i : public virtual POA_RTTest::test
{
public:

  /// Constructor.
  test_i (CORBA::Short num,
          CORBA::ORB_ptr orb);

  /// Destructor.
  ~test_i (void);

  /// Return the number assigned to this object.
  virtual CORBA::Short number (void);

  /// Shutdown the ORB.
  virtual void shutdown (void);

private:

  /// The number assigned to this object.
  CORBA::Short number_;

  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* TEST_I_H */
