// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Base.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_INVOCATION_BASE_H
#define TAO_INVOCATION_BASE_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "ace/Global_Macros.h"


struct TAO_Exception_Data;

namespace  CORBA
{
  class Object;
  class Environment;
  class SystemException;
}

namespace TAO
{
  class Argument;

  /// Enums for invocation types
  enum Invocation_Type {
    TAO_ONEWAY_INVOCATION,
    TAO_TWOWAY_INVOCATION
    // , TAO_LOCATEREQUEST_INVOCATION
  };

  /// Enum for invocation modes
  enum Invocation_Mode {
    /// Standard synchronous twoway
    TAO_SYNCHRONOUS_INVOCATION,
    /// Asynchronous twoway with callback model for collecting the
    /// replies.
    TAO_ASYNCHRONOUS_CALLBACK_INVOCATION,
    /// Asynchronous twoway with poller model for collecting the
    /// replies.
    /// @NOTE: This is not supported in TAO now
    TAO_ASYNCHRONOUS_POLLER_INVOCATION
  };

  /**
   * @class Invocation_Base
   *
   * @brief Generic interface for the invocation object visible to the
   * IDL compiler.
   *
   */
  class TAO_Export Invocation_Base
  {
  public:
    Invocation_Base (CORBA::Object *target,
                     Argument **args,
                     int arg_number,
                     char *operation,
                     int op_len,
                     TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
                     TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);


    void invoke (TAO_Exception_Data *ex,
                 unsigned long ex_count
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (Invocation_Base (void));

    ACE_UNIMPLEMENTED_FUNC (Invocation_Base & operator= (const Invocation_Base &));

  private:

    CORBA::Object *target_;

    Argument **args_;

    int number_args_;

    const char *operation_;

    int op_len_;

    Invocation_Type type_;

    Invocation_Mode mode_;
  };
} // End namespace TAO


#if defined (__ACE_INLINE__)
# include "Invocation_Base.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
