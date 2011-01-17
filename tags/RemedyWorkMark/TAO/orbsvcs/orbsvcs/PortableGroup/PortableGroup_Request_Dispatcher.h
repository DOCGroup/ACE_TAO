//-*- C++ -*-

//=============================================================================
/**
 *  @file    PortableGroup_Request_Dispatcher.h
 *
 *  $Id$
 *
 *  A class that strategizes the request dispatching procedure.
 *
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PORTABLEGROUP_REQUEST_DISPATCHER_H
#define TAO_PORTABLEGROUP_REQUEST_DISPATCHER_H

#include /**/ "ace/pre.h"

#include "tao/Request_Dispatcher.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroup/Portable_Group_Map.h"
#include "orbsvcs/PortableGroup/PortableGroup_Acceptor_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_PortableGroup_Loader;

/**
 * @class PortableGroup_Request_Dispatcher
 *
 * @brief A class that strategizes the request dispatching procedure.
 *
 */
class TAO_PortableGroup_Export PortableGroup_Request_Dispatcher
  : public TAO_Request_Dispatcher
{
  friend class TAO_GOA;

public:

  /// Destructor.
  virtual ~PortableGroup_Request_Dispatcher (void);

  /**
   * Dispatch a request.
   */
  virtual void dispatch (TAO_ORB_Core *orb_core,
                         TAO_ServerRequest &request,
                         CORBA::Object_out forward_to);

private:

  /// Hash map containing the GroupId->ObjectKey mappings.
  TAO_Portable_Group_Map group_map_;

  /// Registry for all of the group (multicast) acceptors.
  TAO_PortableGroup_Acceptor_Registry acceptor_registry_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLEGROUP_REQUEST_DISPATCHER_H */
