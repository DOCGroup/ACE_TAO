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
#include "Arg_Traits_T.h"

namespace TAO
{
  /**
   *
   * @brief Specializations for void return type.
   *
   */

  template<>
  class TAO_Export Ret_Basic_Argument_T<void> : public Argument
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (CORBA::Any *);

    operator void () const;
  };

  template<>
  class TAO_Export Ret_Basic_SArgument_T<void> : public Argument
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void add_to_interceptor (CORBA::Any *);

    operator void () const;
  };

  template<>
  class Arg_Traits<void>
  {
    typedef void                          ret_type;

    typedef Ret_Basic_Argument_T<void>    stub_ret_val;
    typedef Ret_Basic_SArgument_T<void>   skel_ret_val;

    typedef Basic_Tag                     idl_tag;
  };

  /**
   *
   * @brief Specializations for most of the basic IDL types.
   *
   */

  template<>
  class Arg_Traits<CORBA::Short> : public Basic_Arg_Traits_T<CORBA::Short>
  {
  };

  template<>
  class Arg_Traits<CORBA::Long> : public Basic_Arg_Traits_T<CORBA::Long>
  {
  };

  template<>
  class Arg_Traits<CORBA::UShort> : public Basic_Arg_Traits_T<CORBA::UShort>
  {
  };

  template<>
  class Arg_Traits<CORBA::ULong> : public Basic_Arg_Traits_T<CORBA::ULong>
  {
  };

  template<>
  class Arg_Traits<CORBA::Float> : public Basic_Arg_Traits_T<CORBA::Float>
  {
  };

  template<>
  class Arg_Traits<CORBA::Double> : public Basic_Arg_Traits_T<CORBA::Double>
  {
  };

  template<>
  class Arg_Traits<CORBA::LongLong> : public Basic_Arg_Traits_T<CORBA::LongLong>
  {
  };

  template<>
  class Arg_Traits<CORBA::ULongLong> : public Basic_Arg_Traits_T<CORBA::ULongLong>
  {
  };

  template<>
  class Arg_Traits<CORBA::LongDouble> : public Basic_Arg_Traits_T<CORBA::LongDouble>
  {
  };
};

#include "ace/post.h"

#endif /* TAO_BASIC_ARGUMENTS_H */
