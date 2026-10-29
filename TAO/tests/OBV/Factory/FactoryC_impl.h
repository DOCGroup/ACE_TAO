// -*- C++ -*-

// ===================================================================
/**
 *  @file   FactoryC_impl.h
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
public:
  Value2_impl () = default;

  Value2_impl (Value2_impl const&) = delete;
  Value2_impl (Value2_impl&&) = delete;
  Value2_impl& operator= (Value2_impl const&) = delete;
  Value2_impl& operator= (Value2_impl&&) = delete;
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
  Value2_init_impl () = default;

  Value2_init_impl (Value2_init_impl const&) = delete;
  Value2_init_impl (Value2_init_impl&&) = delete;
  Value2_init_impl& operator= (Value2_init_impl const&) = delete;
  Value2_init_impl& operator= (Value2_init_impl&&) = delete;

  OBV_FactoryTest::Value2 *create_default (CORBA::ULong id) override;

  OBV_FactoryTest::Value2 *create (
      CORBA::ULong id,
      OBV_FactoryTest::BaseValue::BV_Data const &data) override;

  CORBA::ValueBase *create_for_unmarshal () override;
};

#endif /* TAO_FACTORY_FACTORY_C_IMPL_H */
