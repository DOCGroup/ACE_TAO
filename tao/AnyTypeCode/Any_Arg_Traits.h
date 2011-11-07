// -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_Arg_Traits.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ANY_ARG_TRAITS_H
#define TAO_ANY_ARG_TRAITS_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/Any.h"
#include "tao/Var_Size_Argument_T.h"
#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<>
  class TAO_AnyTypeCode_Export Arg_Traits<CORBA::Any>
    : public
        Var_Size_Arg_Traits_T<
            CORBA::Any,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_ARG_TRAITS_H */
