// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocated_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================
#ifndef TAO_COLLOCATED_INVOCATION_H
#define TAO_COLLOCATED_INVOCATION_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Collocation_Strategy.h"
#include "tao/Invocation_Base.h"
#include "tao/Exception.h"

class TAO_Operation_Details;
class TAO_Stub;

namespace TAO
{
  class Profile_Transport_Resolver;
  class Argument;
  class Collocation_Proxy_Broker;

  /**
   * @class Collocated_Invocation
   *
   * @brief Base class for ..
   *
   * @NOTE: Reliable oneways are not handled properly including use of
   *  interceptors.
   */
  class TAO_Export Collocated_Invocation : public Invocation_Base
  {
  public:
    Collocated_Invocation (CORBA::Object_ptr et,
                           CORBA::Object_ptr t,
                           TAO_Stub *stub,
                           TAO_Operation_Details &detail,
                           bool response_expected = true);

    Invocation_Status invoke (Collocation_Proxy_Broker *cpb,
                              Collocation_Strategy strat
                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::Exception));

  private:
    ACE_UNIMPLEMENTED_FUNC (Collocated_Invocation (void))

  };
}

#include /**/ "ace/post.h"
#endif /*TAO_COLLOCATED_INVOCATION_H*/
