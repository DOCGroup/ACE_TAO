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
#include "tao/Invocation_Utils.h"


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
                 ACE_ENV_ARG_DECL);

  protected:

    virtual void invoke_collocated (TAO_Stub *,
                                    TAO_Operation_Details &op
                                    ACE_ENV_ARG_DECL);

    virtual void invoke_remote (TAO_Stub *,
                                TAO_Operation_Details &op
                                ACE_ENV_ARG_DECL);

    bool get_timeout (ACE_Time_Value &val);

    TAO_Stub *get_stub (ACE_ENV_SINGLE_ARG_DECL) const;

  protected:

    CORBA::Object *target_;

    Argument **args_;

    int number_args_;

    const char *operation_;

    int op_len_;

    Collocation_Proxy_Broker *cpb_;

    Invocation_Type type_;

    Invocation_Mode mode_;

  private:
    /// Dont allow default initializations
    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter (void));

    ACE_UNIMPLEMENTED_FUNC (Invocation_Adapter & operator= (const Invocation_Adapter &));
  };
} // End namespace TAO


#include "ace/post.h"
#endif /*TAO_INVOCATION_ADAPTER_H*/
