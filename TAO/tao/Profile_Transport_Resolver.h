// -*- C++ -*-

//=============================================================================
/**
 *  @file    Profile_Transport_Resolver.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_PROFILE_TRANSPORT_RESOLVER_H
#define TAO_PROFILE_TRANSPORT_RESOLVER_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "ace/Global_Macros.h"

class TAO_Stub;
class TAO_Profile;
class TAO_Transport;
class TAO_Endpoint;
class ACE_Time_Value;

namespace CORBA
{
  class SystemException;
  class Environment;
  class Object;
}
namespace TAO
{
  class Synch_Twoway_Invocation;

  /**
   * @class Connection_Resolver
   *
   */
  class TAO_Export Profile_Transport_Resolver
  {
  public:
    Profile_Transport_Resolver (CORBA::Object *p,
                                TAO_Stub *);

    ~Profile_Transport_Resolver (void);

    void resolve (ACE_Time_Value *val
                  ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void profile (TAO_Profile *pfile);

    TAO_Profile *profile (void) const;

    TAO_Stub *stub (void) const;

    CORBA::Object *object (void) const;

    bool try_connect (TAO_Endpoint *,
                      ACE_Time_Value *val
                      ACE_ENV_ARG_DECL);

    TAO_Transport *transport (void) const;

  private:

    bool get_connection_timeout (ACE_Time_Value &max_wait_time);

  private:
    mutable CORBA::Object *obj_;

    TAO_Stub *stub_;

    TAO_Transport *transport_;

    TAO_Profile *profile_;
  };
}

#include "ace/post.h"
#endif /*TAO_PROFILE_TRANSPORT_RESOLVER_H*/
