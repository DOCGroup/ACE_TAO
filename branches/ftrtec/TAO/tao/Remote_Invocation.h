// -*- C++ -*-

//=============================================================================
/**
 *  @file    Remote_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_REMOTE_INVOCATION_H
#define TAO_REMOTE_INVOCATION_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Base.h"

class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;

namespace CORBA
{
  class Environment;
}

namespace TAO
{
  class Profile_Transport_Resolver;
  class Argument;

  /**
   * @class Remote_Invocation
   *
   * @brief Base class for remote invocations.
   *
   * This class encapulates some of the common functionalities used by
   * synchronous, asynchronous, DII and DSI invocations.
   *
   */
  class TAO_Export Remote_Invocation : public Invocation_Base
  {
  public:
    /**
     * @param otarget, The original target on which this invocation
     * was started. This is there to be passed up to its parent
     * class.
     *
     * @param resolver, The profile and transport holder.
     *
     * @param op, operation details of the invocation on the target
     *
     * @param response_expected, flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Remote_Invocation (CORBA::Object_ptr otarget,
                       Profile_Transport_Resolver &resolver,
                       TAO_Operation_Details &detail,
                       bool response_expected);

  protected:

    /// Initialize the @a spec.
    void init_target_spec (TAO_Target_Specification &spec
                           ACE_ENV_ARG_DECL);

    /// Write the GIOP header into the stream.
    void write_header (TAO_Target_Specification &spec,
                       TAO_OutputCDR &out_stream
                       ACE_ENV_ARG_DECL);

    /// Marshal the arguments into the stream.
    void marshal_data (TAO_OutputCDR &cdr
                       ACE_ENV_ARG_DECL);

    /// Write the message onto the socket
    Invocation_Status send_message (TAO_OutputCDR &cdr,
                                    short message_semantics,
                                    ACE_Time_Value *max_wait_time
                                    ACE_ENV_ARG_DECL);

  protected:
    /// Our resolver
    Profile_Transport_Resolver &resolver_;
  };
}

#include /**/ "ace/post.h"

#endif /*TAO_REMOTE_INVOCATION_H*/
