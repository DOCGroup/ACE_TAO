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
#include "ace/pre.h"

#include "tao/Request_Dispatcher.h"
#include "portablegroup_export.h"

#include "Portable_Group_Map.h"
#include "PortableGroup_Acceptor_Registry.h"

// Forward declarations.
class TAO_PortableGroup_Loader;

/**
 * @class PortableGroup_Request_Dispatcher
 *
 * @brief A class that strategizes the request dispatching procedure.
 *
 */
class TAO_PortableGroup_Export PortableGroup_Request_Dispatcher :
  public TAO_Request_Dispatcher
{
  friend class TAO_POA_Hooks;

public:
  /// Destructor.
  virtual ~PortableGroup_Request_Dispatcher (void);

  /**
   * Dispatch a request.
   */
  virtual void dispatch (TAO_ORB_Core *orb_core,
                         TAO_ServerRequest &request,
                         CORBA::Object_out forward_to
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Hash map containing the GroupId->ObjectKey mappings.
  TAO_Portable_Group_Map group_map_;

  /// Registry for all of the group (multicast) acceptors.
  TAO_PortableGroup_Acceptor_Registry acceptor_registry_;
};

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_REQUEST_DISPATCHER_H */
