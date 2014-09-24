// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/Basic_Argument_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Arg_Traits_T.h"
#include "tao/Any_Insert_Policy_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Specialization for void return type.
   *
   */
  template<>
  class TAO_Export Arg_Traits<void>
  {
    public:
    typedef void        ret_type;

    typedef RetArgument ret_val;
    typedef Void_Return_Argument
                        clonable_ret_val;
  };

  /**
   *
   * @brief Specializations for basic stub arg types,
   *  except (w)char/boolean/octet.
   *
   */

  template<>
  class TAO_Export Arg_Traits<CORBA::Short>
    : public
        Basic_Arg_Traits_T<
          CORBA::Short,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::Long>
    : public
        Basic_Arg_Traits_T<
          CORBA::Long,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::UShort>
    : public
        Basic_Arg_Traits_T<
          CORBA::UShort,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::ULong>
    : public
        Basic_Arg_Traits_T<
          CORBA::ULong,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::Float>
    : public
        Basic_Arg_Traits_T<
          CORBA::Float,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::Double>
    : public
        Basic_Arg_Traits_T<
          CORBA::Double,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::LongLong>
    : public
        Basic_Arg_Traits_T<
          CORBA::LongLong,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::ULongLong>
    : public
        Basic_Arg_Traits_T<
          CORBA::ULongLong,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<CORBA::LongDouble>
    : public
        Basic_Arg_Traits_T<
          CORBA::LongDouble,
          TAO::Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

  template<>
  class TAO_Export Arg_Traits<std::string>
    : public
        Basic_Arg_Traits_T <
          std::string,
          Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };

#if !defined(ACE_LACKS_STD_WSTRING)
  template<>
  class TAO_Export Arg_Traits<std::wstring>
    : public
        Basic_Arg_Traits_T <
          std::wstring,
          Any_Insert_Policy_AnyTypeCode_Adapter>
  {
  };
#endif /* ACE_LACKS_STD_WSTRING */
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_ARGUMENTS_H */
