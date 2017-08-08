// -*- C++ -*-

//=============================================================================
/**
 *  @file    PG_Utils.h
 *
 *  Utility methods
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_PORTABLEGROUP_UTILS_H
#define TAO_PORTABLEGROUP_UTILS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/portablegroup_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class PG_Utils
   *
   * @brief Simple utility class
   */
  class TAO_PortableGroup_Export PG_Utils
  {
    public:

    /// Set tagged component for the object group
    static CORBA::Boolean set_tagged_component (
        PortableGroup::ObjectGroup *ior,
        PortableGroup::TagGroupTaggedComponent &t);

    /// Get tagged component for the object group
    static CORBA::Boolean get_tagged_component (
        PortableGroup::ObjectGroup *ior,
        PortableGroup::TagGroupTaggedComponent &t);

  private:
    static CORBA::Boolean encode_properties (
        TAO_OutputCDR &cdr,
        PortableGroup::TagGroupTaggedComponent &tg);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_PORTABLEGROUP_UTILS_H*/
