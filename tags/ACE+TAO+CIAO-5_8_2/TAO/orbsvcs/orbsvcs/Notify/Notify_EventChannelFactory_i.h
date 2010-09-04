// -*- C++ -*-

//=============================================================================
/**
 * @file Notify_EventChannelFactory_i.h
 *
 * $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_EVENTCHANNELFACTORY_I_H
#define NOTIFY_EVENTCHANNELFACTORY_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_EventChannelFactory_i
 *
 * @brief Class maintained for Backward compatibility.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_EventChannelFactory_i
{
public:
  /// Create a factory servant and activates it with the default POA.
  /// Also creates a resource factory and assigns it this default_POA.
  static CosNotifyChannelAdmin::EventChannelFactory_ptr
    create (PortableServer::POA_ptr default_POA,
            const char* factory_name = "EventChannelFactory");
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
