// -*- C++ -*-

//=============================================================================
/**
 *  @file    Asynch_Invocation.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_MESSAGING_ASYNCH_INVOCATION_H
#define TAO_MESSAGING_ASYNCH_INVOCATION_H
#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Synch_Invocation.h"
#include "tao/Asynch_Reply_Dispatcher_Base.h"
#include "ace/Global_Macros.h"
#include "ace/Auto_Functor.h"


ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Operation_Details;
class TAO_InputCDR;
class TAO_Asynch_Reply_Dispatcher_Base;

namespace TAO
{
  class Profile_Transport_Resolver;

  class TAO_Messaging_Export Asynch_Remote_Invocation
    : public Synch_Twoway_Invocation
  {
  public:
    Asynch_Remote_Invocation (CORBA::Object_ptr otarget,
                              Profile_Transport_Resolver &resolver,
                              TAO_Operation_Details &detail,
                              TAO_Asynch_Reply_Dispatcher_Base *rd,
                              bool response_expected = true);

    Invocation_Status remote_invocation (ACE_Time_Value *value);

  protected:
    // To prevent leaking memory from the reply dispatcher that we
    // are given
    ACE_Utils::Auto_Functor <TAO_Asynch_Reply_Dispatcher_Base,
                             ARDB_Refcount_Functor> safe_rd_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_MESSAGING_ASYNCH_INVOCATION_H*/
