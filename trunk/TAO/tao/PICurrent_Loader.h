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

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Object_Loader.h"
#include "ace/Service_Config.h"

class TAO_Export TAO_PICurrent_Loader
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
ACE_FACTORY_DECLARE (TAO, TAO_PICurrent_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PICurrent_Initializer = &TAO_PICurrent_Loader::Initializer;

#else

static int
TAO_Requires_PICurrent_Initializer = TAO_PICurrent_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#include /**/ "ace/post.h"

#endif /* TAO_PICURRENT_LOADER_H */
