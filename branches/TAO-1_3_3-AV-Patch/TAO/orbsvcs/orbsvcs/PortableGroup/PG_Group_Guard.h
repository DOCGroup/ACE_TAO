// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Group_Guard.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_GROUP_GUARD_H
#define TAO_PG_GROUP_GUARD_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_Factory_Set.h"

#include "tao/PortableServer/PortableServerC.h"


/// Forward declarations.
class TAO_PG_GenericFactory;
class TAO_PG_ObjectGroupManager;



/**
 * @class TAO_PG_Group_Guard
 *
 * @brief Guard implementation used to make object group cleanup
 *        exception-safe.
 *
 * This guard's destructor performs cleanup of object group
 * resources.  Thus, cleanup is performed when this guard goes out of
 * scope unless explicitly released from that responsibility.
 *
 * This guard is meant to be used internally by the
 * TAO_PG_GenericFactory class.
 */
class TAO_PG_Group_Guard
{
public:

  /// Constructor.
  TAO_PG_Group_Guard (TAO_PG_GenericFactory & generic_factory,
                      TAO_PG_Factory_Set & factory_set,
                      TAO_PG_ObjectGroupManager & group_manager,
                      const PortableServer::ObjectId & oid);

  /// Destructor.
  ~TAO_PG_Group_Guard (void);

  /// Relinquish cleanup responsibility.
  void release (void);

private:

  /// Reference to the infrastructure TAO_PG_GenericFactory that
  /// created the below TAO_PG_Factory_Set.
  TAO_PG_GenericFactory & generic_factory_;

  /// Reference to the TAO_PG_Factory_Set that contains all
  /// application-specific GenericFactory object references.
  TAO_PG_Factory_Set & factory_set_;

  /// Reference to the TAO_PG_ObjectGroupManager that maintains the
  /// object group map.
  TAO_PG_ObjectGroupManager & group_manager_;

  /// Reference to the ObjectId that is the map key necessary to
  /// unbind the corresponding object group map entry from the map
  /// upon destruction.
  const PortableServer::ObjectId & oid_;

  /// Flag that dictates whether or not the destructor will perform
  /// cleanup.
  int released_;
    
};


#include /**/ "ace/post.h"

#endif  /* TAO_PG_GROUP_GUARD_H */
