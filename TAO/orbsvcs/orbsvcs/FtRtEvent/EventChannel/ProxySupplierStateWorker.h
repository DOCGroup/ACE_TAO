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
#include "FTEC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ProxySupplierStateWorker : public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  ProxySupplierStateWorker(FtRtecEventChannelAdmin::ProxySupplierStates& states);
  ~ProxySupplierStateWorker();

  virtual void set_size(size_t size);
  virtual void work(TAO_EC_ProxyPushSupplier* object
            ACE_ENV_ARG_DECL);
private:
  int index_;
  FtRtecEventChannelAdmin::ProxySupplierStates& supplierStates_;
};

#endif
