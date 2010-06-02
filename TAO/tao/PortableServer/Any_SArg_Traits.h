// -*- C++ -*-

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
#include "tao/PortableServer/SArg_Traits_T.h"
#include "tao/PortableServer/portableserver_export.h"
#include "tao/AnyTypeCode/Any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Any;
}

namespace TAO
{
  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Any>
    : public
        Var_Size_SArg_Traits_T<
            CORBA::Any,
            TAO::Any_Insert_Policy_Stream
          >
  {
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ANY_ARG_TRAITS_H */
