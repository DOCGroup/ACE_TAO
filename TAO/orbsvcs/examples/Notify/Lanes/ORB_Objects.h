/* -*- C++ -*- */
/**
 *  @file ORB_Objects.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_ORB_OBJECTS_H
#define TAO_NS_ORB_OBJECTS_H
#include "ace/pre.h"

#include "tao/corba.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/orbsvcs/NotifyExtC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

/**
 * @class TAO_NS_ORB_Objects
 *
 * @brief Handy Objects that we keep asking the ORB for.
 *
 */
class TAO_NS_ORB_Objects
{
public:
  /// Constuctor
  TAO_NS_ORB_Objects (void);

  /// Destructor
  ~TAO_NS_ORB_Objects ();

  /// Resolves all the references.
  void init (CORBA::ORB_var& orb ACE_ENV_ARG_DECL);

  /// Resolve Notification
  CosNotifyChannelAdmin::EventChannelFactory_ptr notify_factory (ACE_ENV_SINGLE_ARG_DECL);

  ///= Public Data
  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;

  RTCORBA::RTORB_var rt_orb_;

  RTCORBA::Current_var current_;

  CosNaming::NamingContextExt_var naming_;
};

#include "ace/post.h"
#endif /* TAO_NS_ORB_OBJECTS_H */
