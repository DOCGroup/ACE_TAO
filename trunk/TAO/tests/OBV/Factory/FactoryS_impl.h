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

  virtual OBV_FactoryTest::BaseValue * get_base_value (void);

  virtual OBV_FactoryTest::Value1 * get_value1 (void);

  virtual OBV_FactoryTest::Value2 * get_value2 (void);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_FACTORY_S_IMPL_H */
