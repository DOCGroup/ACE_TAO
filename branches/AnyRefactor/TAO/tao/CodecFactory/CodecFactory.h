/* -*- C++ -*- */


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

#include "codecfactory_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

class TAO_CodecFactory_Export TAO_CodecFactory_Loader
  : public TAO_Object_Loader
{
public:
  /// Creates a Codec factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_CodecFactory_Loader)
ACE_FACTORY_DECLARE (TAO_CodecFactory, TAO_CodecFactory_Loader)

static int
TAO_Requires_CodecFactory_Initializer = TAO_CodecFactory_Loader::Initializer ();

#define TAO_CODECFACTORY_SAFE_INCLUDE
#include "IOP_CodecC.h"
#undef TAO_CODECFACTORY_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_CODECFACTORY_H */
