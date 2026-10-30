// -*- C++ -*-


//=============================================================================
/**
 *  @file    RecursDef_i.h
 *
 *  StructDef & UnionDef support class.
 *
 *  @author Simon Massey <sma@prismtech.com>
 */
//=============================================================================


#ifndef TAO_RECURSDEF_I_H
#define TAO_RECURSDEF_I_H

#include "orbsvcs/IFRService/TypedefDef_i.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdlib.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//-----------------------------------------------------------------------------
// Class TAO_RecursiveDef_OuterScopes is a support class for StructDef_i.h and
// UnionDef_i.h files. It creates a stack of outer structure definitions, used
// by TAO_StructDef_i::type_i(), and TAO_UnionDef_i::type_i() during nested
// structure processing.
//-----------------------------------------------------------------------------

class TAO_RecursiveDef_OuterScopes
{
public:
  explicit TAO_RecursiveDef_OuterScopes (ACE_TString const &id); // "Push" scope's ID
  ~TAO_RecursiveDef_OuterScopes(); // "Pop" this scope

  static bool SeenBefore (ACE_TString const &id); // Check for outer ID

  TAO_RecursiveDef_OuterScopes (TAO_RecursiveDef_OuterScopes const &) = delete;
  TAO_RecursiveDef_OuterScopes (TAO_RecursiveDef_OuterScopes &&) = delete;
  TAO_RecursiveDef_OuterScopes &operator= (TAO_RecursiveDef_OuterScopes const &) = delete;
  TAO_RecursiveDef_OuterScopes &operator= (TAO_RecursiveDef_OuterScopes &&) = delete;

private: // Data
  ACE_TString const id_;
  TAO_RecursiveDef_OuterScopes const *const pNextOuterID_;

  static TAO_RecursiveDef_OuterScopes const *pIDsSeenAlready;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_RECURSDEF_I_H */
