/* -*- C++ -*- */
/**
 *  @file Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SERVICE_H
#define TAO_NS_SERVICE_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class TAO_NS_Service
 *
 * @brief Abstract ACE_Service_Object interface that is a factory for creating a Notify EventChannelFactory.
 *        Note that a Service creates only 1 Factory object.
 *
 */
class TAO_Notify_Export TAO_NS_Service : public ACE_Service_Object
{
public:
  /// Init the service.
  virtual void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL) = 0;

  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NS_SERVICE_H */
