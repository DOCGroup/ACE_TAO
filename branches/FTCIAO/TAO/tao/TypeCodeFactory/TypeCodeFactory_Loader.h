// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#include "tao/TypeCodeFactory/typecodefactory_export.h"
#include "tao/Object_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_TypeCodeFactory_Loader
 *
 * @brief Create an instance of TypeCodeFactory and return it to the caller.
 */
class TAO_TypeCodeFactory_Export TAO_TypeCodeFactory_Loader
  : public TAO_Object_Loader
{
public:
  /// Constructor.
  TAO_TypeCodeFactory_Loader (void);

  /// Overload the base class method to create a new instance
  /// of a TypeCodeFactory object.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_TCF_Initializer = TAO_TypeCodeFactory_Loader::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_TypeCodeFactory_Loader)
ACE_FACTORY_DECLARE (TAO_TypeCodeFactory, TAO_TypeCodeFactory_Loader)

#define TAO_TYPECODEFACTORY_SAFE_INCLUDE
#include "tao/TypeCodeFactory/TypeCodeFactoryC.h"
#undef TAO_TYPECODEFACTORY_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_TYPECODEFACTORY_LOADER_H */
