// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCodeFactory_Loader.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODEFACTORY_LOADER_H
#define TAO_TYPECODEFACTORY_LOADER_H
#include "ace/pre.h"

#include "typecodefactory_export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_TypeCodeFactory_Export TAO_TypeCodeFactory_Loader
  : public TAO_Object_Loader
{
  // = TITLE
  //     TAO_TypeCodeFactory_Loader
  //
  // = DESCRIPTION
  //     Create an instance of TypeCodeFactory and return
  //     it to the caller.
  //
public:
  TAO_TypeCodeFactory_Loader (void);
  // Constructor.

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           char *argv []
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Overload the base class method to create a new instance
  // of a TypeCodeFactory object.

  static int Initializer (void);
  // Used to force the initialization of the ORB code.
};

ACE_STATIC_SVC_DECLARE (TAO_TypeCodeFactory_Loader)
ACE_FACTORY_DECLARE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_TCF_Initializer = &TAO_TypeCodeFactory_Loader::Initializer;

#else

static int
TAO_Requires_TCF_Initializer = TAO_TypeCodeFactory_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_TYPECODEFACTORY_SAFE_INCLUDE
#include "TypeCodeFactoryC.h"
#undef TAO_TYPECODEFACTORY_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_TYPECODEFACTORY_LOADER_H */
