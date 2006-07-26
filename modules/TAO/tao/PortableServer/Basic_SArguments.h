// -*- C++ -*-

//=============================================================================
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

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Basic_SArgument_T.h"
#include "tao/PortableServer/SArg_Traits_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   *
   * @brief Specialization for void return type.
   *
   */
  template<>
  class TAO_PortableServer_Export SArg_Traits<void>
  {
  public:

    typedef void        ret_type;
    typedef RetArgument ret_val;

  };

  /**
   *
   * @brief Specializations for basic skeleton arg types,
   *  except (w)char/boolean/octet.
   *
   */

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Short>
    : public Basic_SArg_Traits_T<CORBA::Short, TAO::Any_Insert_Policy_Stream <CORBA::Short> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Long>
    : public Basic_SArg_Traits_T<CORBA::Long, TAO::Any_Insert_Policy_Stream <CORBA::Long> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::UShort>
    : public Basic_SArg_Traits_T<CORBA::UShort, TAO::Any_Insert_Policy_Stream <CORBA::UShort> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::ULong>
    : public Basic_SArg_Traits_T<CORBA::ULong, TAO::Any_Insert_Policy_Stream <CORBA::ULong> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Float>
    : public Basic_SArg_Traits_T<CORBA::Float, TAO::Any_Insert_Policy_Stream <CORBA::Float> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::Double>
    : public Basic_SArg_Traits_T<CORBA::Double, TAO::Any_Insert_Policy_Stream <CORBA::Double> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::LongLong>
    : public Basic_SArg_Traits_T<CORBA::LongLong, TAO::Any_Insert_Policy_Stream <CORBA::LongLong> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::ULongLong>
    : public Basic_SArg_Traits_T<CORBA::ULongLong, TAO::Any_Insert_Policy_Stream <CORBA::ULongLong> >
  {
  };

  template<>
  class TAO_PortableServer_Export SArg_Traits<CORBA::LongDouble>
    : public Basic_SArg_Traits_T<CORBA::LongDouble, TAO::Any_Insert_Policy_Stream <CORBA::LongDouble> >
  {
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_SARGUMENTS_H */
