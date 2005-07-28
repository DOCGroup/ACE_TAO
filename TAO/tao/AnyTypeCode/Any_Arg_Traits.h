// This may look like C, but it's really -*- C++ -*-

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

#include "tao/Var_Size_Argument_T.h"
#include "tao/Arg_Traits_T.h"

namespace CORBA
{
  class Any;
  class Any_var;
  class Any_out;
}

namespace TAO
{
  template<>
  class TAO_AnyTypeCode_Export Arg_Traits<CORBA::Any>
    : public
        Var_Size_Arg_Traits_T<
            CORBA::Any,
            CORBA::Any_var,
            CORBA::Any_out
          >
  {
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ANY_ARG_TRAITS_H */
