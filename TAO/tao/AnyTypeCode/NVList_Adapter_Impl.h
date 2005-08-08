// This may look like C, but it's really -*- C++ -*-

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

/**
 * @class TAO_NVList_Adapter
 */
class TAO_AnyTypeCode_Export TAO_NVList_Adapter_Impl 
  : public TAO_NVList_Adapter
{
public:
  virtual void create_list (
      CORBA::Long count,
      CORBA::NVList_ptr &new_list
      ACE_ENV_ARG_DECL);

  virtual void create_named_value (
      CORBA::NamedValue_ptr &nv
      ACE_ENV_ARG_DECL);
};


#include /**/ "ace/post.h"
#endif /* TAO_NVLIST_ADAPTER_IMPL_H */
