// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Any_SArg_Traits.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef TAO_ANY_SARG_TRAITS_H
#define TAO_ANY_SARG_TRAITS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Var_Size_SArgument_T.h"
#include "tao/PortableServer/portableserver_export.h"

namespace CORBA
{
  class Any;
  class Any_var;
  class Any_out;
}

namespace TAO
{
  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Any>
    : public
        Var_Size_SArg_Traits_T<
            CORBA::Any,
            CORBA::Any_var,
            CORBA::Any_out
          >
  {
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ANY_ARG_TRAITS_H */
