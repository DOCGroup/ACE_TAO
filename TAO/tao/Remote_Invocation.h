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
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"


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
   * @class Synch_Invocation
   *
   * @brief Base class for Twoway_Invocation and Oneway_Invocation.
   *
   */
  class TAO_Export Remote_Invocation
  {
  public:
    Remote_Invocation (Profile_Transport_Resolver &resolver,
                       TAO_Operation_Details &detail);

  protected:

    void init_target_spec (TAO_Target_Specification &spec
		           ACE_ENV_ARG_DECL);

    void write_header (TAO_Target_Specification &spec,
                       TAO_OutputCDR &out_stream
                       ACE_ENV_ARG_DECL);

    void marshal_data (Argument **args,
                       int args_number,
                       TAO_OutputCDR &cdr
                       ACE_ENV_ARG_DECL);

    void send_message (short message_semantics,
                       TAO_OutputCDR &cdr
                       ACE_ENV_ARG_DECL);
  protected:
    Profile_Transport_Resolver &resolver_;
    TAO_Operation_Details &detail_;
  };
}

#endif /*TAO_REMOTE_INVOCATION_H*/
