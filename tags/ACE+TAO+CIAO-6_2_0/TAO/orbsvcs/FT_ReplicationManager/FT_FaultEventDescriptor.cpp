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

#include "orbsvcs/Log_Macros.h"
#include "FT_FaultEventDescriptor.h"

#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Default constructor.
TAO::FT_FaultEventDescriptor::FT_FaultEventDescriptor ()
  : all_at_location_failed (0)
  , all_of_type_at_location_failed (0)
  , object_at_location_failed (0)
  , object_is_primary (0)
  , type_id (CORBA::string_dup (""))
  , object_group_id (PortableGroup::ObjectGroupId (0))
{
}

// Debugging support.
void TAO::FT_FaultEventDescriptor::dump ()
{
  // Get the location as a string.
  ACE_CString loc_as_string;
  for (CORBA::ULong li = 0; li < this->location->length(); ++li)
  {
    if (li > 0) loc_as_string += "/";
    // Assume only the "id" field of the CosNaming::Name is used.
    loc_as_string += CORBA::string_dup (this->location[li].id);
  }

  if (this->all_at_location_failed == 1)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO::FT_FaultEventDescriptor::dump: ")
      ACE_TEXT ("All objects at location <%s> failed.\n"),
        loc_as_string.c_str()
    ));
  }

  if (this->all_of_type_at_location_failed == 1)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO::FT_FaultEventDescriptor::dump: ")
      ACE_TEXT ("All objects of type <%s> at location <%s> failed.\n"),
        this->type_id.in(),
        loc_as_string.c_str()
    ));
  }

  if (this->object_at_location_failed == 1)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO::FT_FaultEventDescriptor::dump: ")
      ACE_TEXT ("Replica of type <%s> with ObjectGroupId <%Q> ")
      ACE_TEXT ("at location <%s> failed.\n"),
        this->type_id.in(),
        this->object_group_id,
        loc_as_string.c_str()
    ));
  }

  if (this->object_is_primary == 1)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO::FT_FaultEventDescriptor::dump: ")
      ACE_TEXT ("Primary replica of ObjectGroupId <%Q> failed.\n"),
        this->object_group_id
    ));
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
