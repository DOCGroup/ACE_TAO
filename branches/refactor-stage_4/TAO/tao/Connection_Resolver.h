// -*- C++ -*-

//=============================================================================
/**
 *  @file    Connection_Resolver.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_CONNECTION_RESOLVER_H
#define TAO_CONNECTION_RESOLVER_H
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Stub;

namespace TAO
{
  /**
   * @class Connection_Resolver
   *
   */
  class TAO_Export Default_Profile_Connection_Resolver
  {
  public:
    Profile_Connection_Resolver (TAO_Stub *);

    TAO_Transport *resolve (TAO_Invocation_Endpoint_Selector *s);

  private:
    TAO_Stub *stub_;

    TAO_Transport *transport_;
  };
}

#include "ace/post.h"
#endif /*TAO_CONNECTION_RESOLVER_H*/
