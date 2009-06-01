// -*- C++ -*-

//=============================================================================
/**
 *  @file   ProxyConsumerStateWorker.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef PROXYCONSUMERSTATEWORKER_H
#define PROXYCONSUMERSTATEWORKER_H

#include "orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxyConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ProxyConsumerStateWorker
  : public TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
{
public:
  ProxyConsumerStateWorker(FtRtecEventChannelAdmin::ProxyConsumerStates& states);
  ~ProxyConsumerStateWorker();

  virtual void set_size(size_t size);
  virtual void work(TAO_EC_ProxyPushConsumer* object);
private:
  int index_;
  FtRtecEventChannelAdmin::ProxyConsumerStates& consumerStates_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
