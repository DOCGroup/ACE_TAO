// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Any
//
// = FILENAME
//   AnyS_impl.h
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef TAO_ANY_S_IMPL_H
#define TAO_ANY_S_IMPL_H

#include "AnyS.h"

class Test_impl : public POA_OBV_AnyTest::Test
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

  virtual CORBA::Any * get_something (
      CORBA::Boolean need_vb
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  CORBA::Boolean even_;
  // The ORB
};

#endif /* TAO_ANY_S_IMPL_H */
