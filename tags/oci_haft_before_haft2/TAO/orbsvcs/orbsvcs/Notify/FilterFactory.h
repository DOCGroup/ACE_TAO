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

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class TAO_Notify_FilterFactory
 *
 * @brief Service Object to obtain a CosNotifyFilter::FilterFactory reference.
 *
 */
class /*TAO_Notify_Export*/ TAO_Notify_FilterFactory : public ACE_Service_Object
{
public:
  /// Factory method to create a FilterFactory reference
  /// The Factory is activated in the default POA. The filters created are activated in the <filter_poa>.
  virtual CosNotifyFilter::FilterFactory_ptr create (PortableServer::POA_var& filter_poa ACE_ENV_ARG_DECL) = 0;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_FILTERFACTORY_H */
