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
#include "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Base.h"
#include "ace/CORBA_macros.h"


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
   */
  class TAO_Export Collocated_Invocation : protected Invocation_Base
  {
  public:
    Collocated_Invocation (TAO_Stub *stub,
                           TAO_Operation_Details &detail);

    Invocation_Status invoke (Collocation_Proxy_Broker *cpb,
                              CORBA::Object *obj,
                              Collocation_Strategy strat
                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    ACE_UNIMPLEMENTED_FUNC (Collocated_Invocation (void));
  private:
    CORBA::Object_var forwarded_;
  };
}
#include "ace/post.h"
#endif /*TAO_COLLOCATED_INVOCATION_H*/
