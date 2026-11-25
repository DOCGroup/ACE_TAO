/**
 *  @file    PG_Property_Set_Find.h
 *
 *  Typed lookup helper for a property set.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
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
  /// Find and extract a typed value from a TAO::PG_Property_Set.
  template <typename TYPE>
  int find (PG_Property_Set const & decoder, ACE_CString const & key, TYPE & value)
  {
    int result = 0;
    PortableGroup::Value const * any {};
    if (decoder.find (key, any))
    {
      result = ((*any) >>= value);
    }
    return result;
  }

} //namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // TAO_PG_PROPERTY_SET_FIND_H
