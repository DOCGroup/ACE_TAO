// -*- C++ -*-

//=============================================================================
/**
 *  @file    Valuetype_Adapter_Factory_Impl.h
 *
 *  $Id$
 *
 *  @author  Kees van Marle <kvmarle@remedy.nl>
 */
//=============================================================================


#ifndef TAO_VALUETYPE_ADAPTER_FACTORY_IMPL_H
#define TAO_VALUETYPE_ADAPTER_FACTORY_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/Valuetype/valuetype_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Valuetype_Adapter_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Valuetype_Adapter;

/**
 * @class TAO_Valuetype_Adapter_Factory_Impl
 *
 * @brief TAO_Valuetype_Adapter_Factory_Impl.
 *
 * Class that creates instances of TAO_Valuetype_Adapter (one per ORB).
 * This is the derived class that contains the actual implementations.
 */
class TAO_Valuetype_Export TAO_Valuetype_Adapter_Factory_Impl
  : public TAO_Valuetype_Adapter_Factory
{
public:
  virtual ~TAO_Valuetype_Adapter_Factory_Impl (void);

  virtual TAO_Valuetype_Adapter * create (void);

  // Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_ValueType_Initializer =
  TAO_Valuetype_Adapter_Factory_Impl::Initializer ();


ACE_STATIC_SVC_DECLARE (TAO_Valuetype_Adapter_Factory_Impl)
ACE_FACTORY_DECLARE (TAO_Valuetype, TAO_Valuetype_Adapter_Factory_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_VALUETYPE_SAFE_INCLUDE
// #include "tao/ValueType/ValueTypeC.h"
#undef TAO_VALUETYPE_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_VALUETYPE_ADAPTER_FACTORY_IMPL_H */
