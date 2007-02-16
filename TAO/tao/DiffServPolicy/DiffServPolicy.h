// -*- C++ -*-

//=============================================================================
/**
 *  @file   DiffServPolicy.h
 *
 *  $Id$
 *
 *  @author  Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *           Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_DIFFSERVPOLICY_H
#define TAO_DIFFSERVPOLICY_H

#include /**/ "ace/pre.h"

#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DiffServPolicy_Export TAO_DiffServPolicy_Initializer
{
public:
  /// Used to force the initialization of the ORB code.
  static int init (void);
};

static int
TAO_Requires_DiffServPolicy_Initializer = 
  TAO_DiffServPolicy_Initializer::init ();

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#define TAO_DIFFSERVPOLICY_SAFE_INCLUDE
#include "tao/DiffServPolicy/DiffServPolicyC.h"
#undef TAO_DIFFSERVPOLICY_SAFE_INCLUDE

#endif /* TAO_DIFFSERVPOLICY_H */
