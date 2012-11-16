/* -*- C++ -*- $Id$ */
//=============================================================================
/**
 *  @file   Invocation_Retry_Params.h
 *
 *  $Id$
 *
 *  @author Byron Harris (harrisb@ociweb.com)
 */
//=============================================================================

#ifndef TAO_INVOCATION_RETRY_PARAMS_H
#define TAO_INVOCATION_RETRY_PARAMS_H

#include "tao/orbconf.h"
#include "ace/Array_Map.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @struct Invocation_Retry_Params
   *
   * @brief Contains the invocation retry parameters used when encountering
   * CORBA exceptions.
   * The parameters can be specified using either the Client_Strategy_Factory
   * service given in the service configuration file or through the command line.
   * Any command line parameter overrides the corresponding option in the service
   * configurator file.
   */
  struct Invocation_Retry_Params
  {
    Invocation_Retry_Params();

    typedef ACE_Array_Map<int, int> exception_limit_map_type;

    /**
     * The maximum number of retry attempts per exception type
     * when exceptions are encountered. The profiles are
     * cycled through during each attempt.
     */
    exception_limit_map_type forward_on_exception_limit_;

    /**
     * The maximum number of times to retry a an invocation
     * if the the connection to the server is closed when
     * trying to get a reply.
     */
    int forward_on_reply_closed_limit_;

    /**
     * The delay to use between cycles of base and forward profiles.
     */
    ACE_Time_Value init_retry_delay_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_INVOCATION_RETRY_PARAMS_H*/
