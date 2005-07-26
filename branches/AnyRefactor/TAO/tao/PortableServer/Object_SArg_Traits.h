// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_SArg_Traits.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman
 */
//=============================================================================

#ifndef TAO_OBJECT_SARG_TRAITS_H
#define TAO_OBJECT_SARG_TRAITS_H

#include /**/ "ace/pre.h"

#include "tao/CORBA_methods.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Object_SArgument_T.h"
#include "tao/Pseudo_VarOut_T.h"


namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;

  typedef TAO_Pseudo_Var_T<Object> Object_var;
  typedef TAO_Pseudo_Out_T<Object, Object_var> Object_out;
}

// --------------------------------------------------------------

namespace TAO
{
  template <typename T> class SArg_Traits;

  /// Used in generated code if CORBA::Object is an argument or
  /// return type.
  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Object>
    : public Object_SArg_Traits_T<CORBA::Object_ptr,
                                  CORBA::Object_var,
                                  CORBA::Object_out>
  {
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_OBJECT_SARG_TRAITS_H */
