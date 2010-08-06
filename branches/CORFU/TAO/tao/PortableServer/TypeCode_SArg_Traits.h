// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCode_SArg_Traits.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman
 */
//=============================================================================

#ifndef TAO_TYPECODE_SARG_TRAITS_H
#define TAO_TYPECODE_SARG_TRAITS_H

#include /**/ "ace/pre.h"

#include "tao/CORBA_methods.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Object_SArgument_T.h"
#include "tao/Pseudo_VarOut_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  typedef TAO_Pseudo_Var_T<TypeCode> TypeCode_var;
  typedef TAO_Pseudo_Out_T<TypeCode> TypeCode_out;
}

// --------------------------------------------------------------

namespace TAO
{
  template <typename T> class SArg_Traits;

  /// Used in generated code if CORBA::TypeCode is an argument or
  /// return type.
  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::TypeCode>
    : public Object_SArg_Traits_T<CORBA::TypeCode_ptr,
                                  CORBA::TypeCode_var,
                                  CORBA::TypeCode_out,
                                  TAO::Any_Insert_Policy_Stream <CORBA::TypeCode_ptr>
                                 >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODE_SARG_TRAITS_H */
