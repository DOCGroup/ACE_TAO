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

#ifndef TAO_ORV_CONSTANTS_H
#define TAO_ORV_CONSTANTS_H

#include "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  typedef CORBA::Short Visibility;
  typedef CORBA::Short_out Visibility_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_Visibility;

  extern TAO_Export const CORBA::Short PRIVATE_MEMBER;
  extern TAO_Export const CORBA::Short PUBLIC_MEMBER;

  typedef CORBA::Short ValueModifier;
  typedef CORBA::Short_out ValueModifier_out;
  extern TAO_Export CORBA::TypeCode_ptr _tc_ValueModifier;
};

#include "ace/post.h"

#endif  /* TAO_ORBCONF_H */
