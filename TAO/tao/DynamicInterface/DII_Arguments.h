// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Arguments.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons
 */
//=============================================================================


#ifndef TAO_DII_ARGUMENTS_H
#define TAO_DII_ARGUMENTS_H

#include "ace/pre.h"

#include "tao/Argument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
   *
   */
  class NamedValue_Argument : public Argument
  {
  public:
    NamedValue_Argument (CORBA::NamedValue_ptr);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    virtual void interceptor_result (CORBA::Any *);

    int byte_order (void) const;

  private:
    CORBA::NamedValue_ptr x_;

    /// Byte order that the Request class will use
    int byte_order_;
  };

  /**
   * @class NamedValue_Argument
   *
   * @brief Class for the argument list of a CORBA::(Server)Request.
   *
   */
  class NVList_Argument : public Argument
  {
  public:
    NVList_Argument (CORBA::NVList_ptr,
                     bool lazy_eval);

    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

    // Not an override of a base class method, but a new one that
    // deals with the argument list as a whole.
    void interceptor_paramlist (Dynamic::ParameterList *);

  private:
    CORBA::NVList_ptr x_;

    bool lazy_evaluation_;
  };
}

#if defined (__ACE_INLINE__)
# include "DII_Arguments.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_DII_ARGUMENTS_H */
