/* -*- C++ -*- */
//=============================================================================
/**
 *  @file BiDirGIOP.h
 *
 *  $Id$
 *
 *  Dynamic loader object for BiDir GIOP
 *
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_BIDIR_GIOP_H
#define TAO_BIDIR_GIOP_H
#include "ace/pre.h"

#include "bidirgiop_export.h"
#include "tao/BiDir_Adapter.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_BiDirGIOP_Export TAO_BiDirGIOP_Loader : public TAO_BiDir_Adapter
{
public:
  /// Constructor
  TAO_BiDirGIOP_Loader (void);


  /// Creates a  factory and returns it.
  virtual int activate (CORBA::ORB_ptr,
                        int argc,
                        char *argv []
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Parse the policy sent by the ORB_Core
  virtual int parse_policy (TAO_ORB_Core *orb_core,
                            CORBA::Policy_ptr policy
                            TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

};

ACE_STATIC_SVC_DECLARE (TAO_BiDirGIOP_Loader)
ACE_FACTORY_DECLARE (TAO_BiDirGIOP, TAO_BiDirGIOP_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_BiDirGIOP_Initializer = &TAO_BiDirGIOP_Loader::Initializer;

#else

static int
TAO_Requires_BiDirGIOP_Initializer = TAO_BiDirGIOP_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_BIDIRGIOP_SAFE_INCLUDE
#include "BiDirPolicyC.h"
#undef TAO_BIDIRGIOP_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_DYNAMICANY_H */
