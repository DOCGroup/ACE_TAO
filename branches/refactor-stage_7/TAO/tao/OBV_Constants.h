// -*- C++ -*-

//=============================================================================
/**
 *  @file     OBV_Constants.h
 *
 *  $Id$
 *
 *  Constants related to valuetypes, but also used by type codes.
 *
 *  @author  Jeff Parsons
 */
//=============================================================================

#ifndef TAO_OBV_CONSTANTS_H
#define TAO_OBV_CONSTANTS_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"

namespace CORBA
{
  typedef CORBA::Short Visibility;
  typedef CORBA::Short_out Visibility_out;

  extern TAO_Export const CORBA::Short PRIVATE_MEMBER;
  extern TAO_Export const CORBA::Short PUBLIC_MEMBER;

  typedef CORBA::Short ValueModifier;
  typedef CORBA::Short_out ValueModifier_out;
};

#include /**/ "ace/post.h"

#endif  /* TAO_OBV_CONSTANTS_H */
