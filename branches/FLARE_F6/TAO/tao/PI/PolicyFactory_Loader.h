// -*- C++ -*-


//=============================================================================
/**
 *  @file    PolicyFactory_Loader.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_POLICYFACTORY_LOADER_H
#define TAO_POLICYFACTORY_LOADER_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PolicyFactory_Registry_Factory.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_PI_Export TAO_PolicyFactory_Loader
  : public TAO_PolicyFactory_Registry_Factory
{
public:
  /// Creates a Codec factory and returns it.
  virtual TAO::PolicyFactory_Registry_Adapter* create (void);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_PolicyFactory_Initializer = TAO_PolicyFactory_Loader::Initializer ();


ACE_STATIC_SVC_DECLARE (TAO_PolicyFactory_Loader)
ACE_FACTORY_DECLARE (TAO_PI, TAO_PolicyFactory_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_POLICYFACTORY_LOADER_H */
