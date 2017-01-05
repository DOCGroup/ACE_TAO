// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Property_Set_Find.h
 *
 *  This is a companion function for the properties docoder
 *  to work around compilers that don't support templated methods.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTY_SET_FIND_H
#define TAO_PG_PROPERTY_SET_FIND_H
#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroup/PG_Property_Set.h"

#include "ace/Hash_Map_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * Find a value in a TAO::PG_Property_Set.
   * This is a work-around for the lack of
   * templated methods.
   */
  template <typename TYPE>
  int find (const PG_Property_Set & decoder, const ACE_CString & key, TYPE & value)
  {
    int result = 0;
    const PortableGroup::Value * any = 0;
    if ( decoder.find (key, any))
    {
      result = ((*any) >>= value);
    }
    return result;
  }

} //namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_PG_PROPERTY_SET_FIND_H
