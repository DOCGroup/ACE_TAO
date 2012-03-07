// -*- C++ -*-

//=============================================================================
/**
 *  @file    CodecFactory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_CODECFACTORY_H
#define TAO_CODECFACTORY_H

#include /**/ "ace/pre.h"

#include "tao/CodecFactory/codecfactory_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_CODECFACTORY_SAFE_INCLUDE
#include "tao/CodecFactory/IOP_CodecC.h"
#undef TAO_CODECFACTORY_SAFE_INCLUDE

#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CODECFACTORY_Export TAO_CodecFactory_Loader
  : public TAO_Object_Loader
{
public:
  /// Creates a Codec factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_CodecFactory_Initializer = TAO_CodecFactory_Loader::Initializer ();

ACE_STATIC_SVC_DECLARE (TAO_CodecFactory_Loader)
ACE_FACTORY_DECLARE (TAO_CODECFACTORY, TAO_CodecFactory_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL


#include /**/ "ace/post.h"

#endif /* TAO_CODECFACTORY_H */
