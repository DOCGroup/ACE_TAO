
//=============================================================================
/**
 *  @file    Object_Proxy_Broker.h
 *
 *  $Id$
 *
 *  This files contains the abstract class for the CORBA Object
 *  proxy brokers. Interface specific proxy broker are generated
 *  by the IDL compiler.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_PROXY_BROKER_H_
#define TAO_OBJECT_PROXY_BROKER_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#include "tao/Object_Proxy_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Object_Proxy_Broker
{
public:

  // -- Ctor/Dtor --
  virtual ~TAO_Object_Proxy_Broker (void);

  virtual TAO_Object_Proxy_Impl &select_proxy (CORBA::Object_ptr object
                                               ACE_ENV_ARG_DECL) = 0;

protected:
  TAO_Object_Proxy_Broker (void);
};



#include "ace/post.h"

#endif /* TAO_OBJECT_PROXY_BROKER_H_ */
