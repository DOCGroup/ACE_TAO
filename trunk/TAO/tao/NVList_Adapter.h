// -*- C++ -*-

//=============================================================================
/**
 *  @file    NVList_Adapter.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_NVLIST_ADAPTER_H
#define TAO_NVLIST_ADAPTER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class NVList;
  typedef NVList *NVList_ptr;

  class NamedValue;
  typedef NamedValue *NamedValue_ptr;
}

/**
 * @class TAO_NVList_Adapter
 */
class TAO_Export TAO_NVList_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_NVList_Adapter (void);

  virtual void create_list (CORBA::Long count, CORBA::NVList_ptr &new_list) = 0;

  virtual void create_named_value (CORBA::NamedValue_ptr &nv) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NVLIST_ADAPTER_H */
