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
   * @brief Base class for Twoway_Invocation and Oneway_Invocation.
   *
   */
  class TAO_Export Remote_Invocation : protected Invocation_Base
  {
  public:
    Remote_Invocation (CORBA::Object_ptr otarget,
                       Profile_Transport_Resolver &resolver,
                       TAO_Operation_Details &detail,
                       bool response_expected);

  protected:

    void init_target_spec (TAO_Target_Specification &spec
                           ACE_ENV_ARG_DECL);

    void write_header (TAO_Target_Specification &spec,
                       TAO_OutputCDR &out_stream
                       ACE_ENV_ARG_DECL);

    void marshal_data (TAO_OutputCDR &cdr
                       ACE_ENV_ARG_DECL);

    Invocation_Status send_message (TAO_OutputCDR &cdr,
                                    short message_semantics,
                                    ACE_Time_Value *max_wait_time
                                    ACE_ENV_ARG_DECL);

  protected:
    Profile_Transport_Resolver &resolver_;
  };
}

#include /**/ "ace/post.h"

#endif /*TAO_REMOTE_INVOCATION_H*/
