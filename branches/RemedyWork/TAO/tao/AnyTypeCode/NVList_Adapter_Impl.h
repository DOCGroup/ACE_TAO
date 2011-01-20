// -*- C++ -*-

//=============================================================================
/**
 *  @file    NVList_Adapter_Impl.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_NVLIST_ADAPTER_IMPL_H
#define TAO_NVLIST_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/NVList_Adapter.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_NVList_Adapter
 */
class TAO_AnyTypeCode_Export TAO_NVList_Adapter_Impl
  : public TAO_NVList_Adapter
{
public:
  virtual void create_list (CORBA::Long count, CORBA::NVList_ptr &new_list);

  virtual void create_named_value (CORBA::NamedValue_ptr &nv);

  /// Used to force the initialization of the NVList adapter
  static int Initializer (void);
};

static int TAO_Requires_NVList_Adapter_Impl_Initializer =
  TAO_NVList_Adapter_Impl::Initializer ();

ACE_STATIC_SVC_DECLARE (TAO_NVList_Adapter_Impl)
ACE_FACTORY_DECLARE (TAO_AnyTypeCode, TAO_NVList_Adapter_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NVLIST_ADAPTER_IMPL_H */
