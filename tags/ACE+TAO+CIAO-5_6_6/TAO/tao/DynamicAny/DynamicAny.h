// -*- C++ -*-

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

#include "tao/DynamicAny/dynamicany_export.h"

#define TAO_DYNAMICANY_SAFE_INCLUDE
#include "tao/DynamicAny/DynamicAnyC.h"
#undef TAO_DYNAMICANY_SAFE_INCLUDE

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DynamicAny_Export TAO_DynamicAny_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_DynamicAny_Loader (void);

  /// Creates a DynamicAny factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_DynamicAny_Initializer = TAO_DynamicAny_Loader::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_DynamicAny_Loader)
ACE_FACTORY_DECLARE (TAO_DynamicAny, TAO_DynamicAny_Loader)

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMICANY_H */
