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

#ifndef TAO_NS_FILTERFACTORY_H
#define TAO_NS_FILTERFACTORY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

/**
 * @class TAO_NS_FilterFactory
 *
 * @brief Service Object to obtain a CosNotifyFilter::FilterFactory reference.
 *
 */
class /*TAO_Notify_Export*/ TAO_NS_FilterFactory : public ACE_Service_Object
{
public:
  /// Factory method to create a FilterFactory reference
  /// The Factory is activated in the default POA.
  virtual CosNotifyFilter::FilterFactory_ptr create (ACE_ENV_SINGLE_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NS_FILTERFACTORY_H */
