/**
 *  @file MC_Notify_Service.h
 *
 *  $Id$
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */

#ifndef TAO_MC_NOTIFY_SERVICE_H
#define TAO_MC_NOTIFY_SERVICE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/CosNotify_Service.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/MonitorControlExt/notify_mc_ext_export.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_MC_Notify_Service
 *
 * @brief Implemetation of the TAO_Notify_Service interface for Monitoring
 *
 */
class TAO_Notify_MC_Ext_Export TAO_MC_Notify_Service: public TAO_CosNotify_Service
{
public:
  virtual ~TAO_MC_Notify_Service (void);

protected:
  virtual void init_i (CORBA::ORB_ptr orb);
  virtual void init_i2 (CORBA::ORB_ptr orb, CORBA::ORB_ptr dispatching_orb);
  virtual void finalize_service (
                 CosNotifyChannelAdmin::EventChannelFactory_ptr factory);

private:
  virtual TAO_Notify_Factory* create_factory (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_Notify_MC_Ext, TAO_MC_Notify_Service)

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* TAO_MC_NOTIFY_SERVICE_H */
