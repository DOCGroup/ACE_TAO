// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Arguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_BASIC_ARGUMENTS_H
#define TAO_BASIC_ARGUMENTS_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Basic_Argument_T.h"

namespace TAO
{
  /**
   *
   * @brief Specializations for void return type.
   *
   */

  struct Void_Arg_Traits
  {
    typedef void        ret_type;

    typedef Argument    stub_ret_val;
    typedef Argument    skel_ret_val;

    typedef Basic_Tag   idl_tag;
  };

  /**
   *
   * @brief Typedefs for basic IDL arg types, except (w)char/boolean/octet.
   *
   */

  typedef Basic_Arg_Traits_T<CORBA::Short>        Short_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::Long>         Long_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::UShort>       UShort_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::ULong>        ULong_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::Float>        Float_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::Double>       Double_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::LongLong>     LongLong_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::ULongLong>    ULongLong_Arg_Traits;

  typedef Basic_Arg_Traits_T<CORBA::LongDouble>   LongDouble_Arg_Traits;
};

#include "ace/post.h"

#endif /* TAO_BASIC_ARGUMENTS_H */
