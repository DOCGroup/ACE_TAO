// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Factory
//
// = FILENAME
//   FactoryC_impl.h
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef TAO_FACTORY_FACTORY_C_IMPL_H
#define TAO_FACTORY_FACTORY_C_IMPL_H

#include "FactoryC.h"

// Just need to mix-in RefCounter
class Value2_impl : public virtual OBV_OBV_FactoryTest::Value2,
                    public virtual CORBA_DefaultValueRefCountBase
{
  // = TITLE
  //   Value2 implementation
  //
  // = DESCRIPTION
  //   Implements the Value2 valuetype
  //

};

// Factory implementation
class Value2_init_impl : public OBV_FactoryTest::Value2_init
{
  // = TITLE
  //   OBV factory implementation
  //
  // = DESCRIPTION
  //   Implements the Value2 factory implementation
  //
public:

  virtual OBV_FactoryTest::Value2* create_default (
      CORBA::ULong id
      );

  virtual OBV_FactoryTest::Value2* create (
      CORBA::ULong id,
      const OBV_FactoryTest::BaseValue::BV_Data & data
      );

public:

  virtual CORBA_ValueBase* create_for_unmarshal (void);

};

#endif /* TAO_FACTORY_FACTORY_C_IMPL_H */
