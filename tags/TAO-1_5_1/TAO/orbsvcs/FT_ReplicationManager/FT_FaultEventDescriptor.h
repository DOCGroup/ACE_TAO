/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultEventDescriptor.h
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================


#ifndef FT_FAULT_EVENT_DESCRIPTOR_H_
#define FT_FAULT_EVENT_DESCRIPTOR_H_

#include /**/ "ace/pre.h"
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_CORBAC.h"
#include "orbsvcs/PortableGroupC.h"
#include "FT_ReplicationManagerLib_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /// Helper class for describing the properties in a fault event.
  struct TAO_ReplicationManagerLib_Export FT_FaultEventDescriptor
  {
  public:

    /// Default constructor.
    FT_FaultEventDescriptor ();

    /// Debugging support.
    void dump ();

    /////////////////
    /// Data members.

    /// Flags indicating the "extent" of the fault.
    int all_at_location_failed;
    int all_of_type_at_location_failed;
    int object_at_location_failed;
    int object_is_primary;

    /// The location of the fault.
    PortableGroup::Location_var location;

    /// The TypeId of the object that faulted.
    PortableGroup::TypeId_var type_id;

    /// The ObjectGroupId of the faulted object.
    PortableGroup::ObjectGroupId object_group_id;

    /// Other properties of the object group to which the fault relates.
    PortableGroup::MembershipStyleValue membership_style;
    FT::ReplicationStyleValue replication_style;
    PortableGroup::MinimumNumberMembersValue minimum_number_members;
    PortableGroup::InitialNumberMembersValue initial_number_members;

    /// The object group's factories.
    PortableGroup::FactoryInfos_var factories;

  };

} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_FAULT_EVENT_DESCRIPTOR_H_ */

