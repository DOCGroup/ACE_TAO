// -*- C++ -*-

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
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OBJECT_PROXY_BROKER_H_
#define TAO_OBJECT_PROXY_BROKER_H_

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"


class TAO_Object_Proxy_Impl;

/**
 * @class TAO_Object_Proxy_Broker
 *
 * @brief TAO_Object_Proxy_Broker
 *
 * TAO_Object_Proxy_Broker
 */
class TAO_Export TAO_Object_Proxy_Broker
{
public:

  /// Destructor
  virtual ~TAO_Object_Proxy_Broker (void);

  virtual TAO_Object_Proxy_Impl &select_proxy (CORBA::Object_ptr object
                                               ACE_ENV_ARG_DECL) = 0;

};


#include "ace/post.h"

#endif /* TAO_OBJECT_PROXY_BROKER_H_ */
