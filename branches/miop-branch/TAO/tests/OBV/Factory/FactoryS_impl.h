// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Factory
//
// = FILENAME
//   FactoryS_impl.h
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef TAO_FACTORY_S_IMPL_H
#define TAO_FACTORY_S_IMPL_H

#include "FactoryS.h"

class Test_impl : public POA_OBV_FactoryTest::Test
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

  virtual OBV_FactoryTest::BaseValue * get_base_value (TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual OBV_FactoryTest::Value1 * get_value1 (TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual OBV_FactoryTest::Value2 * get_value2 (TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_FACTORY_S_IMPL_H */
