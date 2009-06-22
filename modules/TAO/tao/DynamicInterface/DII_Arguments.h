// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_Arguments.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_DII_ARGUMENTS_H
#define TAO_DII_ARGUMENTS_H

#include /**/ "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class NamedValue;
  typedef NamedValue *NamedValue_ptr;

  class NVList;
  typedef NVList *NVList_ptr;
}

namespace Dynamic
{
  class ParameterList;
}

namespace TAO
{
  /**
   * @class NamedValue_Argument
   *
   * @brief Class for the return value of a CORBA::(Server)Request.
   */
  class NamedValue_Argument : public RetArgument
  {
  public:
    NamedValue_Argument (CORBA::NamedValue_ptr);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_value (CORBA::Any *any) const;

    int byte_order (void) const;

  private:
    CORBA::NamedValue_ptr x_;

    /// Byte order that the Request class will use
    int byte_order_;
  };

  /**
   * @class NVList_Argument
   *
   * @brief Class for the argument list of a CORBA::(Server)Request.
   */
  class NVList_Argument : public InArgument
  {
  public:
    NVList_Argument (CORBA::NVList_ptr,
                     bool lazy_eval);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    // Not an override of a base class method, but a new one that
    // deals with the argument list as a whole.
    void interceptor_paramlist (Dynamic::ParameterList *);

    CORBA::NVList_ptr arg () const;

  private:
    CORBA::NVList_ptr x_;

    bool lazy_evaluation_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/DynamicInterface/DII_Arguments.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_DII_ARGUMENTS_H */
