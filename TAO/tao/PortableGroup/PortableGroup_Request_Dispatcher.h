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
public:
  /// Constructor.
  PortableGroup_Request_Dispatcher (TAO_PortableGroup_Loader *loader);

  /// Destructor.
  virtual ~PortableGroup_Request_Dispatcher (void);

  /**
   * Dispatch a request.
   */
  virtual void dispatch (TAO_ORB_Core *orb_core,
                         TAO_ServerRequest &request,
                         void *context, /* unused? */
                         CORBA::Object_out forward_to,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Back pointer to the owning portable group loader.
  TAO_PortableGroup_Loader *portable_group_loader_;
};

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_REQUEST_DISPATCHER_H */
