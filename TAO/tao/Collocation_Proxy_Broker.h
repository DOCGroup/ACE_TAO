// -*- C++ -*-

//=============================================================================
/**
 *  @file   Collocation_Proxy_Broker.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_COLLOCATION_PROXY_BROKER_H
#define TAO_COLLOCATION_PROXY_BROKER_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class Collocation_Proxy_Broker
   */
  class TAO_Export Collocation_Proxy_Broker
  {

  public:

    virtual Collocation_Strategy get_strategy (CORBA::Object_ptr obj
                                               ACE_ENV_ARG_DECL) = 0;

    virtual void dispatch (CORBA::Object_ptr obj,
                           CORBA::Object_out forward_obj,
                           Argument ** args,
                           int num_args,
                           const char * op,
                           size_t op_len,
                           Collocation_Strategy strategy
                           ACE_ENV_ARG_DECL) = 0;
  };
}

#include "ace/post.h"

#endif /*TAO_COLLOCATION_PROXY_BROKER_H */
