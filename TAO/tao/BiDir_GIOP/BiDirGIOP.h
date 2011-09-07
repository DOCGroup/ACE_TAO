// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#include "tao/BiDir_GIOP/bidirgiop_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/BiDir_Adapter.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_BiDirPolicy_Validator;

/**
 * @class TAO_BiDirGIOP_Loader
 *
 * @brief Class that loads the BiDir library.
 */

class TAO_BIDIRGIOP_Export TAO_BiDirGIOP_Loader : public TAO_BiDir_Adapter
{
public:

  /// Constructor
  TAO_BiDirGIOP_Loader (void);

  /// Destructor
  virtual ~TAO_BiDirGIOP_Loader (void);

  /// Initialize the BiDIR loader hooks.
  virtual int init (int argc,
                    ACE_TCHAR* []);

  virtual void load_policy_validators (TAO_Policy_Validator &validator);

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

private:

  /// Flag to indicate whether the BiDirGIOP library has been
  /// activated.
  bool initialized_;
};

static bool
TAO_Requires_BiDirGIOP_Initializer = TAO_BiDirGIOP_Loader::Initializer ();


ACE_STATIC_SVC_DECLARE (TAO_BiDirGIOP_Loader)
ACE_FACTORY_DECLARE (TAO_BIDIRGIOP, TAO_BiDirGIOP_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL


#define TAO_BIDIRGIOP_SAFE_INCLUDE
#include "tao/BiDir_GIOP/BiDirPolicyC.h"
#undef TAO_BIDIRGIOP_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_GIOP_H */
