/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    RecursDef_i.h
//
// = DESCRIPTION
//    StructDef & UnionDef support class.
//
// = AUTHOR
//    Simon Massey <sma@prismtech.com>
//
// ============================================================================

#ifndef TAO_RECURSDEF_I_H
#define TAO_RECURSDEF_I_H

#include "TypedefDef_i.h"
#include "ace/OS_NS_stdlib.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

//-----------------------------------------------------------------------------
// Class TAO_RecursiveDef_OuterScopes is a support class for StructDef_i.h and
// UnionDef_i.h files. It creates a stack of outer structure definitions, used
// by TAO_StructDef_i::type_i(), and TAO_UnionDef_i::type_i() during nested
// structure processing.
//-----------------------------------------------------------------------------

class TAO_RecursiveDef_OuterScopes
{
public:
  TAO_RecursiveDef_OuterScopes( const ACE_TString &id ); // "Push" scope's ID
  ~TAO_RecursiveDef_OuterScopes(); // "Pop" this scope

  static bool SeenBefore( const ACE_TString &id ); // Check for outer ID

private: // Data
  const ACE_TString id_;
  const TAO_RecursiveDef_OuterScopes *const pNextOuterID_;

  static const TAO_RecursiveDef_OuterScopes *pIDsSeenAlready;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_RECURSDEF_I_H */
