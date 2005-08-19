/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    PICurrent_Loader.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_PICURRENT_LOADER_H
#define TAO_PICURRENT_LOADER_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/Object_Loader.h"

#include "ace/Service_Config.h"

class TAO_PI_Export TAO_PICurrent_Loader
  : public TAO_Object_Loader
{
public:
  /// Creates a PICurrent and returns it.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

ACE_STATIC_SVC_DECLARE (TAO_PICurrent_Loader)
ACE_FACTORY_DECLARE (TAO_PI, TAO_PICurrent_Loader)

static int
TAO_Requires_PICurrent_Initializer = TAO_PICurrent_Loader::Initializer ();

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PICURRENT_LOADER_H */
