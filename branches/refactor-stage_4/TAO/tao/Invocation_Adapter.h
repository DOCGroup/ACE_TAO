// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Adapter.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_INVOCATION_ADAPTER_H
#define TAO_INVOCATION_ADAPTER_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "ace/Global_Macros.h"


struct TAO_Exception_Data;
class TAO_Operation_Details;
class TAO_Stub;
class ACE_Time_Value;
namespace  CORBA
{
  class Object;
  class Environment;
  class SystemException;
}

namespace TAO
{
  class Argument;

  class Collocation_Proxy_Broker;

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
   * @class Invocation_Adapter
   *
   * @brief Generic interface for the invocation object visible to the
   * IDL compiler.
   *
   */
  class TAO_Export Invocation_Adapter
  {
  public:
    Invocation_Adapter (CORBA::Object *target,
                     Argument **args,
                     int arg_number,
                     char *operation,
                     int op_len,
                     Collocation_Proxy_Broker *b,
                     TAO::Invocation_Type type = TAO_TWOWAY_INVOCATION,
                     TAO::Invocation_Mode mode = TAO_SYNCHRONOUS_INVOCATION);


    void invoke (TAO_Exception_Data *ex,
                 unsigned long ex_count
                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    void invoke_collocated (TAO_Stub *,
                            TAO_Operation_Details &op
                            ACE_ENV_ARG_DECL);

    void invoke_remote (TAO_Stub *,
                        TAO_Operation_Details &op
                        ACE_ENV_ARG_DECL);

    bool get_timeout (ACE_Time_Value &val);

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter (void));

    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter & operator= (const Invocation_Adapter &));

  private:

    CORBA::Object *target_;

    Argument **args_;

    int number_args_;

    const char *operation_;

    int op_len_;

    Collocation_Proxy_Broker *cpb_;

    Invocation_Type type_;

    Invocation_Mode mode_;
  };
} // End namespace TAO


#include "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
