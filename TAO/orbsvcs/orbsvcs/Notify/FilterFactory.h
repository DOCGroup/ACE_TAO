/* -*- C++ -*- */
/**
 *  @file FilterFactory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_FILTERFACTORY_H
#define TAO_Notify_FILTERFACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_FilterFactory
 *
 * @brief Service Object to obtain a CosNotifyFilter::FilterFactory reference.
 *
 */
class /*TAO_Notify_Serv_Export*/ TAO_Notify_FilterFactory : public ACE_Service_Object
{
public:
  /// Factory method to create a FilterFactory reference
  /// The Factory is activated in the default POA. The filters created are activated in the <filter_poa>.
  virtual CosNotifyFilter::FilterFactory_ptr create (
      PortableServer::POA_ptr filter_poa) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_FILTERFACTORY_H */
