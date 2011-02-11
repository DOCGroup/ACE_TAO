// -*- C++ -*-


//=============================================================================
/**
 *  @file    IORManip_Loader.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_IORMANIP_LOADER_H
#define TAO_IORMANIP_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/IORManipulation/ior_manip_export.h"
#include "tao/Object_Loader.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IORManip_Export TAO_IORManip_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_IORManip_Loader (void);

  /// Creates a IORManip factory and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_IORManip_Initializer = TAO_IORManip_Loader::Initializer ();


ACE_STATIC_SVC_DECLARE (TAO_IORManip_Loader)
ACE_FACTORY_DECLARE (TAO_IORManip, TAO_IORManip_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_IORMANIP_SAFE_INCLUDE
#include "tao/IORManipulation/IORC.h"
#undef TAO_IORMANIP_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_IORManip_H */
