// -*- C++ -*-

//=============================================================================
/**
 *  @file    LocateRequest_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_LOCATEREQUEST_INVOCATION_H
#define TAO_LOCATEREQUEST_INVOCATION_H

#include /**/ "ace/pre.h"
#include "ace/Global_Macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Synch_Invocation.h"

class TAO_Synch_Reply_Dispatcher;

namespace CORBA
{
  class SystemException;
}

namespace TAO
{
  class Profile_Transport_Resolver;

  /**
   * @class LocateRequest_Invocation
   *
   * @brief Object created by TAO::LocateRequest_Invocation_Adapter to
   * create and send LocateRequest invocation.
   *
   */
  class TAO_Export LocateRequest_Invocation
    : protected Synch_Twoway_Invocation
  {
  public:
    /**
     * @param otarget The target on which this invocation was
     *  started.
     *
     * @param resolver Container of the profile and transport on
     * which this invocation is on
     *
     * @param detail Operation details of the invocation on @a
     * target. This is of no practical importance since it is not used
     * in this class. This is just to keep our base class happy.
     *
     */
    LocateRequest_Invocation (
        CORBA::Object_ptr otarget,
        Profile_Transport_Resolver &resolver,
        TAO_Operation_Details &detail);

    /// Start the invocation.
    Invocation_Status invoke (ACE_Time_Value *max_wait_time
                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));

  private:
    /// Helper to check the reply status
    Invocation_Status check_reply (TAO_Synch_Reply_Dispatcher &rd
                                   ACE_ENV_ARG_DECL);

  };
}

#include /**/ "ace/post.h"

#endif /*TAO_LOCATEREQUEST_INVOCATION_H*/
