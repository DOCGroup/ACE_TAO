// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Forward
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
      TreeController * tc,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_TEST_IMPL_H */
