// -*- C++ -*-

// ===================================================================
/**
 *  @file   FactoryC_impl.h
 *
 *  $Id$
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
// ===================================================================

#ifndef TAO_FACTORY_FACTORY_C_IMPL_H
#define TAO_FACTORY_FACTORY_C_IMPL_H

#include "FactoryC.h"

/**
 * @class Value2_impl
 *
 * @brief Value2 implementation.
 *
 * Implements the Value2 valuetype.
 *
 * @note Just need to mix-in RefCounter
 */
class Value2_impl : public virtual OBV_OBV_FactoryTest::Value2,
                    public virtual CORBA::DefaultValueRefCountBase
{
};

/**
 * @class Value2_init_impl
 *
 * @brief OBV factory implementation.
 *
 * Implements the Value2 factory implementation.
 */
class Value2_init_impl : public OBV_FactoryTest::Value2_init
{
public:

  virtual OBV_FactoryTest::Value2* create_default (
      CORBA::ULong id
      );

  virtual OBV_FactoryTest::Value2* create (
      CORBA::ULong id,
      const OBV_FactoryTest::BaseValue::BV_Data & data
      );

public:

  virtual CORBA::ValueBase * create_for_unmarshal (void);

};

#endif /* TAO_FACTORY_FACTORY_C_IMPL_H */
