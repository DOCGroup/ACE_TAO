// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    SArgument_Base_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SARGUMENT_T_H
#define TAO_SARGUMENT_T_H

#include "ace/pre.h"

#include "SArgument.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class SArgument_Base
   *
   * @brief Template base class for most IDL basic type skeleton arguments.
   */
  template<typename T>
  class SArgument_Base : public SArgument
  {
  public:
    virtual void any_insert (CORBA::Any & any)
    {
      any <<= this->argval_;
    }

    T & arg (void)
    {
      return this->argval_;
    }

  private:
    T & argval_;
  };

  /**
   * @class In_Predefined_SArgument
   *
   * @brief Template class for most IDL basic type skeleton IN arguments.
   */
  template<typename T>
  class In_Predefined_SArgument : public SArgument_Base<T>
  {
  public:
    virtual CORBA::Boolean unmarshal (TAO_InputCDR &cdr)
    {
      return (cdr >> this->argval_);
    }
  };

  /**
   * @class Inout_Predefined_SArgument
   *
   * @brief Template class for most IDL basic type skeleton INOUT arguments.
   */
  template<typename T>
  class Inout_Predefined_SArgument : public SArgument_Base<T>
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr)
    {
      return (cdr << this->argval)_;
    }

    virtual CORBA::Boolean unmarshal (TAO_InputCDR &cdr)
    {
      return (cdr >> this->argval_);
    }
  };

  /**
   * @class Out_Predefined_SArgument
   *
   * @brief Template class for most IDL basic type skeleton OUT arguments.
   */
  template<typename T>
  class Out_Predefined_SArgument : public SArgument_Base<T>
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr)
    {
      return (cdr << this->argval_);
    }
  };

  /**
   * @class Retn_Predefined_SArgument
   *
   * @brief Template class for most IDL basic type skeleton return arguments.
   */
  template<typename T>
  class Retn_Predefined_SArgument : public SArgument_Base<T>
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR & cdr)
    {
      return (cdr << this->argval_);
    }
  };
};

#include "ace/post.h"

#endif /* TAO_SARGUMENT_T_H */

