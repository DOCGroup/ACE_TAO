/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynamicAny.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =========================================================================

#ifndef TAO_IORMANIP_LOADER_H
#define TAO_IORMANIP_LOADER_H
#include /**/ "ace/pre.h"

#include "ior_manip_export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_IORManip_Export TAO_IORManip_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_IORManip_Loader (void);

  /// Creates a IORManip factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_IORManip_Loader)
ACE_FACTORY_DECLARE (TAO_IORManip, TAO_IORManip_Loader)

static int
TAO_Requires_IORManip_Initializer = TAO_IORManip_Loader::Initializer ();

#define TAO_IORMANIP_SAFE_INCLUDE
#include "IORC.h"
#undef TAO_IORMANIP_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_IORManip_H */
