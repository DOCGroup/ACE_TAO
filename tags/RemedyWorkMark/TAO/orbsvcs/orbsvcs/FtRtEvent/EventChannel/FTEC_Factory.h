// -*- C++ -*-

//=============================================================================
/**
 *  @file   FTEC_Factory.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef FTEC_FACTORY_H
#define FTEC_FACTORY_H

#include "orbsvcs/Event/EC_Default_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Basic_Factory : public TAO_EC_Default_Factory
{
public:
    /// Constructor
  TAO_FTEC_Basic_Factory (void);
  virtual TAO_EC_ConsumerAdmin*
      create_consumer_admin (TAO_EC_Event_Channel_Base*);
  virtual TAO_EC_SupplierAdmin*
      create_supplier_admin (TAO_EC_Event_Channel_Base*);

  virtual TAO_EC_ProxyPushSupplier*
      create_proxy_push_supplier (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_supplier (TAO_EC_ProxyPushSupplier*);

  /// Create and destroy a ProxyPushConsumer
  virtual TAO_EC_ProxyPushConsumer*
      create_proxy_push_consumer (TAO_EC_Event_Channel_Base*);
  virtual void
      destroy_proxy_push_consumer (TAO_EC_ProxyPushConsumer*);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

