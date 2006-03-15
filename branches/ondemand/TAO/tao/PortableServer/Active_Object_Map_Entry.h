// -*- C++ -*-

//=============================================================================
/**
 *  @file    Active_Object_Map_Entry.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_ACTIVE_OBJECT_MAP_ENTRY_H
#define TAO_ACTIVE_OBJECT_MAP_ENTRY_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/PS_ForwardC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @struct TAO_Active_Object_Map_Entry
 *
 * @brief Value field of the active object map.
 *
 * We need a mapping from and to all of the following fields:
 * user_id, system_id, and servant.  Therefore, we keep
 * all the fields together in the map.
 */
struct TAO_Active_Object_Map_Entry
{
  /// Default constructor.
  TAO_Active_Object_Map_Entry (void);

  /// User id.
  PortableServer::ObjectId user_id_;

  /// System id.
  PortableServer::ObjectId system_id_;

  /// Servant.
  PortableServer::Servant servant_;

  /// Reference count on outstanding requests on this servant.
  CORBA::UShort reference_count_;

  /// Has this servant been deactivated already?
  CORBA::Boolean deactivated_;

  /// Priority of this servant.
  CORBA::Short priority_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ACTIVE_OBJECT_MAP_ENTRY_H */
