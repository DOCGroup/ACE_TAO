/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    DynamicAny.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_DYNAMICANY_H
#define TAO_DYNAMICANY_H
#include /**/ "ace/pre.h"

#include "dynamicany_export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_DynamicAny_Export TAO_DynamicAny_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_DynamicAny_Loader (void);

  /// Creates a DynamicAny factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_DynamicAny_Loader)
ACE_FACTORY_DECLARE (TAO_DynamicAny, TAO_DynamicAny_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_DynamicAny_Initializer = &TAO_DynamicAny_Loader::Initializer;

#else

static int
TAO_Requires_DynamicAny_Initializer = TAO_DynamicAny_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_DYNAMICANY_SAFE_INCLUDE
#include "DynamicAnyC.h"
#undef TAO_DYNAMICANY_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMICANY_H */
