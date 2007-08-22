// ================================================================
/**
 *  @file   PolicyFactory_Registry_Factory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
// ================================================================

#ifndef TAO_POLICYFACTORY_REGISTRY_FACTORY_H
#define TAO_POLICYFACTORY_REGISTRY_FACTORY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"
#include "ace/Service_Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class PolicyFactory_Registry_Adapter;
}

/**
 * @class TAO_PolicyFactory_Registry_Factory
 */
class TAO_Export TAO_PolicyFactory_Registry_Factory :
  public ACE_Service_Object
{
public:
  virtual ~TAO_PolicyFactory_Registry_Factory (void);

  virtual TAO::PolicyFactory_Registry_Adapter* create (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_POLICYFACTORY_REGISTRY_FACTORY_H */
