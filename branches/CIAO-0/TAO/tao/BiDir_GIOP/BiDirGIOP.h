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


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/BiDir_Adapter.h"
#include "ace/Service_Config.h"

class TAO_BiDirPolicy_Validator;

/**
 * @class TAO_BiDirGIOP_Loader
 *
 * @brief Class that loads the BiDir library.
 */

class TAO_BiDirGIOP_Export TAO_BiDirGIOP_Loader : public TAO_BiDir_Adapter
{
public:

  /// Constructor
  TAO_BiDirGIOP_Loader (void);

  /// Destructor
  ~TAO_BiDirGIOP_Loader (void);

  /// Creates a  factory and returns it.
  virtual int activate (CORBA::ORB_ptr,
                        int argc,
                        ACE_TCHAR *argv []
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void load_policy_validators (TAO_Policy_Validator &validator)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

private:

  /// Our policy validator
  TAO_BiDirPolicy_Validator *validator_;

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
