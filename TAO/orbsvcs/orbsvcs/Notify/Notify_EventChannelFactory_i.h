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
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_EventChannelFactory_i
 *
 * @brief Class maintained for Backward compatibility.
 *
 */
class TAO_Notify_Export TAO_Notify_EventChannelFactory_i
{
public:
  /// Create a factory servant and activates it with the default POA.
  /// Also creates a resource factory and assigns it this default_POA.
  static CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL);
};

#include /**/ "ace/post.h"
#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
