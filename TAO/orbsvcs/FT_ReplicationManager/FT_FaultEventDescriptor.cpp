/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultEventDescriptor.cpp
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *  This file provides the implementation of the
 *  TAO::FT_FaultEventDescriptor structure.  The
 *  TAO::FT_FaultEventDescriptor is a helper type used during
 *  analysis of fault events.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/FT_ReplicationManager/FT_FaultEventDescriptor.h"

ACE_RCSID (FT_FaultEventDescriptor,
           FT_FaultEventDescriptor,
           "$Id$")

/// Default constructor.
TAO::FT_FaultEventDescriptor::FT_FaultEventDescriptor ()
  : all_at_location_failed (0)
  , all_of_type_at_location_failed (0)
  , object_at_location_failed (0)
  , object_is_primary (0)
  , object_group_id (PortableGroup::ObjectGroupId (0))
{
  type_id = CORBA::string_dup ("");
}
