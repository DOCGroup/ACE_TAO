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

#include "orbsvcs/orbsvcs/Event/EC_SupplierAdmin.h"
#include "orbsvcs/orbsvcs/ESF/ESF_Worker.h"
#include "FTEC_ProxyConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ProxyConsumerStateWorker : public TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
{
public:
  ProxyConsumerStateWorker(FtRtecEventChannelAdmin::ProxyConsumerStates& states);
  ~ProxyConsumerStateWorker();

  virtual void set_size(size_t size);
  virtual void work(TAO_EC_ProxyPushConsumer* object
            ACE_ENV_ARG_DECL);
private:
  int index_;
  FtRtecEventChannelAdmin::ProxyConsumerStates& consumerStates_;
};

#endif
