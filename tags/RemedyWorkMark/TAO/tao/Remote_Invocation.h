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

#include "tao/Invocation_Base.h"
#include "tao/Transport.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;

namespace TAO
{
  class Profile_Transport_Resolver;
  class Argument;

  /**
   * @class Remote_Invocation
   *
   * @brief Base class for remote invocations.
   *
   * This class encapsulates some of the common functionalities used by
   * synchronous, asynchronous, DII and DSI invocations.
   *
   */
  class TAO_Export Remote_Invocation : public Invocation_Base
  {
  public:
    /**
     * @param otarget The original target on which this invocation
     * was started. This is there to be passed up to its parent
     * class.
     *
     * @param resolver The profile and transport holder.
     *
     * @param detail Pperation details of the invocation on the target
     *
     * @param response_expected Flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Remote_Invocation (CORBA::Object_ptr otarget,
                       Profile_Transport_Resolver &resolver,
                       TAO_Operation_Details &detail,
                       bool response_expected);

  protected:

    /// Initialize the @a spec.
    void init_target_spec (TAO_Target_Specification &spec, TAO_OutputCDR& output);

    /// Write the GIOP header into the stream.
    void write_header (TAO_OutputCDR &out_stream);

    /// Marshal the arguments into the stream.
    void marshal_data (TAO_OutputCDR &cdr);

    /// Write the message onto the socket
    Invocation_Status send_message (TAO_OutputCDR &cdr,
                                    TAO_Message_Semantics message_semantics,
                                    ACE_Time_Value *max_wait_time);

  protected:
    /// Our resolver
    Profile_Transport_Resolver &resolver_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_REMOTE_INVOCATION_H*/
