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


namespace  CORBA
{
  class Object;
}

namespace TAO
{
  /// Enums for inovcation types
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
                     TAO::Argument **args,
                     int arg_number,
                     char *operation,
                     int op_len,
                     TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
                     TAO::Invocation_Type mode = TAO_SYNCHRONOUS_INVOCATION);


    void invoke (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    /// Dont allow default initializations
    Invocation_Base (void);

    int oneway_invocation (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));


    int twoway_invocation (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    CORBA::Object *target_;

    TAO_Argument **args_;

    int args_number_;

    const char *operation_;

    int op_len_;

    TAO_Invocation_Type type_;

    TAO_Invocation_Mode mode_;
  };
} // End namespace TAO


#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
