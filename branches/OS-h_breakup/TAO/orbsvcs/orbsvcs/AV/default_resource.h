//=============================================================================
/**
 *  @file   default_resource.h
 *
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy
 */
//=============================================================================


#ifndef TAO_AV_DEFAULT_RESOURCE_H
#define TAO_AV_DEFAULT_RESOURCE_H
#include "ace/pre.h"

#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Resource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

/**
 * @class TAO_AV_Default_Resource_Factory
 *
 * @brief TAO AV Streams default resource factory
 *        The AV Streams resource factory loads the
 *        pluggable transport and flow protocol factories
 *        in the corresponding factory sets.
 *  
 *
 */

class TAO_AV_Export TAO_AV_Default_Resource_Factory
: public TAO_AV_Resource_Factory
{
public:

  /// Constructor.
  TAO_AV_Default_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_AV_Default_Resource_Factory (void);

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  virtual int init (int argc, char *argv[]);
  
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_AV, TAO_AV_Default_Resource_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_Default_Resource_Factory)

#include "ace/post.h"
#endif /* TAO_AV_DEFAULT_RESOURCE_H */
