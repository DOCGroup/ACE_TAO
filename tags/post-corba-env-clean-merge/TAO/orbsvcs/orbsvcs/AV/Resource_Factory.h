//=============================================================================
/**
 *  @file   Resource_Factory.h
 *  $Id$
 *
 *  @author Yamuna Krishnamurthy
 */
//=============================================================================


#ifndef TAO_AV_DEFAULT_RESOURCE_FACTORY_H
#define TAO_AV_DEFAULT_RESOURCE_FCATORY_H
#include "ace/pre.h"

#include "orbsvcs/AV/AVStreams_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

/**
 * @class TAO_AV_Resource_Factory
 *
 * @brief TAO AV Streams default resource factory abstract
 *        The AV Streams resource factory loads the
 *        pluggable transport and flow protocol factories
 *        in the corresponding factory sets.
 *  
 *
 */

class TAO_AV_Export TAO_AV_Resource_Factory
: public ACE_Service_Object 
{
public:

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  virtual int init (int argc, char *argv[]) = 0;
  
};

#include "ace/post.h"
#endif /* TAO_AV_DEFAULT_RESOURCE_FACTORY_H */
