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
#include "ace/pre.h"

#include "tao/Synch_Invocation.h"
#include "messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

class TAO_Operation_Details;
class TAO_InputCDR;
class ACE_Time_Value;
class TAO_Asynch_Reply_Dispatcher_Base;

namespace CORBA
{
  class SystemException;
}

namespace TAO
{
  class Profile_Transport_Resolver;

  class TAO_Messaging_Export Asynch_Remote_Invocation
    : protected Synch_Twoway_Invocation
  {
  public:
    Asynch_Remote_Invocation (CORBA::Object_ptr otarget,
                              Profile_Transport_Resolver &resolver,
                              TAO_Operation_Details &detail,
                              TAO_Asynch_Reply_Dispatcher_Base *rd,
                              bool response_expected = true);

    Invocation_Status remote_invocation (ACE_Time_Value *value
                                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));

  protected:
    TAO_Asynch_Reply_Dispatcher_Base *rd_;
  };
}


#include "ace/post.h"
#endif /*TAO_SYNCH_INVOCATION_H*/
