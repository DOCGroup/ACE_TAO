/**
 *  @file    Basic_SArguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_BASIC_SARGUMENTS_H
#define TAO_BASIC_SARGUMENTS_H

#include /**/ "ace/pre.h"

#include "tao/Basic_SArgument_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Arg_Traits_T.h"

namespace TAO
{
  /**
   *
   * @brief Specialization for void return type.
   *
   */
  template<>
  class TAO_Export SArg_Traits<void>
  {
    public:
    typedef void        ret_type;

    typedef Argument    ret_val;
  };

  /**
   *
   * @brief Specializations for basic skeleton arg types,
   *  except (w)char/boolean/octet.
   *
   */

  template<>
  class TAO_Export SArg_Traits<CORBA::Short>
    : public Basic_SArg_Traits_T<CORBA::Short>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::Long>
    : public Basic_SArg_Traits_T<CORBA::Long>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::UShort>
    : public Basic_SArg_Traits_T<CORBA::UShort>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::ULong>
    : public Basic_SArg_Traits_T<CORBA::ULong>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::Float>
    : public Basic_SArg_Traits_T<CORBA::Float>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::Double>
    : public Basic_SArg_Traits_T<CORBA::Double>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::LongLong>
    : public Basic_SArg_Traits_T<CORBA::LongLong>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::ULongLong>
    : public Basic_SArg_Traits_T<CORBA::ULongLong>
  {
  };

  template<>
  class TAO_Export SArg_Traits<CORBA::LongDouble>
    : public Basic_SArg_Traits_T<CORBA::LongDouble>
  {
  };
};

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_SARGUMENTS_H */
