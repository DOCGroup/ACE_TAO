// -*- C++ -*-

/**
 *  @file RT_Notify_Service.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_RT_NOTIFY_SERVICE_H
#define TAO_Notify_RT_NOTIFY_SERVICE_H

#include /**/ "ace/pre.h"
#include "orbsvcs/Notify/rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/CosNotify_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Notify_Service
 *
 * @brief Implemetation of the TAO_Notify_Service interface for RT Notification.
 */
class TAO_RT_Notify_Export TAO_RT_Notify_Service : public TAO_CosNotify_Service
{
public:
  /// Constructor
  TAO_RT_Notify_Service (void);

  /// Destructor
  ~TAO_RT_Notify_Service () override;

  /// Init the service.
  void init_service (CORBA::ORB_ptr orb) override;

protected:
  /// Init the data members
  void init_i (CORBA::ORB_ptr orb) override;

  /// Create the Factory for RT Notify objects.
  TAO_Notify_Factory* create_factory (void) override;

  /// Creates the Builder for RT Notify objects.
  TAO_Notify_Builder* create_builder (void) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DECLARE (TAO_RT_Notify,TAO_RT_Notify_Service)

#include /**/ "ace/post.h"
#endif /* TAO_Notify_RT_NOTIFY_SERVICE_H */
