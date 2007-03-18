// -*- C++ -*-

//=============================================================================
/**
 *  @file   ProxySupplierStateWorker.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef PROXYSUPPLIERSTATEWORKER_H
#define PROXYSUPPLIERSTATEWORKER_H

#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class ProxySupplierStateWorker
  : public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  ProxySupplierStateWorker(FtRtecEventChannelAdmin::ProxySupplierStates& states);
  ~ProxySupplierStateWorker();

  virtual void set_size(size_t size);
  virtual void work(TAO_EC_ProxyPushSupplier* object);
private:
  int index_;
  FtRtecEventChannelAdmin::ProxySupplierStates& supplierStates_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
