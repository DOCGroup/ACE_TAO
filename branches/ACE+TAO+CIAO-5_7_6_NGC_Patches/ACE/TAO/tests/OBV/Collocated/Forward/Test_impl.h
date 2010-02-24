// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Collocated/Forward
//
// = FILENAME
//   Test_impl.h
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "TreeControllerS.h"
#include "TreeBaseS.h"
#include "TreeNodeS.h"

class Test_impl : public POA_Test
{
  // = TITLE
  //   Implementation of Test interface
  //
  // = DESCRIPTION
  //   This interface is provided to produce valuetypes and test
  //   marshaling
  //
public:
  Test_impl (CORBA::ORB_ptr orb);
  // ctor

  virtual TreeController * reflect (
      TreeController * tc);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_TEST_IMPL_H */
